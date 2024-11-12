#include "Rail.h"

#define NOMINMAX

#include <fstream>
#include <algorithm>

#include <Windows.h>

#include <Engine/Resources/PolygonMesh/PolygonMeshManager.h>
#include <Engine/Runtime/WorldClock/WorldClock.h>
#include <Engine/Utility/Tools/SmartPointer.h>
#include <Library/Math/Definition.h>
#include <Engine/Debug/Output.h>

#include "Game/GameScene/Rail/CatmullRom.h"

#include <Library/Externals/nlohmann/json.hpp>
using json = nlohmann::json;

void Rail::initialize() {
	load_rail();
	if (railPoints.empty()) {
		create_rail_point({ 0,0,-3 }, 0.5f, 5.0f);
		create_rail_point(CVector3::ZERO, 0.5f, 5.0f);
		create_rail_point({ 4,0,3 }, 0.2f, 5.0f, -PI / 6);
		create_rail_point({ 5,3,0 }, 0.5f, 5.0f, 0.0f);
		create_rail_point({ 15,1,1 }, 0.5f, 5.0f, PI);
	}

	apply_rail_point();

	create_rail();

#ifdef _DEBUG
	editor = eps::CreateUnique<RailEditor>();
	editor->initialize(this);
#endif // _DEBUG

}

void Rail::begin_rendering() {
	for (RailMesh& railMesh : railDrawMesh) {
		railMesh.mesh->begin_rendering();
	}
}

void Rail::draw() const {
	for (const RailMesh& railMesh : railDrawMesh) {
		railMesh.mesh->draw();
	}
}

float Rail::rail_length() const {
	return railLength;
}

void Rail::transform_from_mileage(WorldInstance& worldInstance, float mileage) const {
	// Index算出
	int index = static_cast<int>(mileage);
	float parametric = mileage - std::floor(mileage);
	std::optional<int> nextIndex;
	if (index + 1 < railLength) {
		nextIndex = index + 1;
	}
	// Translate算出
	Vector3 internal = railDrawMesh[index].mesh->world_position();
	Vector3 terminal =
		nextIndex.has_value() ?
		railDrawMesh[nextIndex.value()].mesh->world_position() :
		railPoints.back().position;
	worldInstance.get_transform().set_translate(Vector3::Lerp(internal, terminal, parametric));
	// Quaternion
	Quaternion internalRotation = railDrawMesh[index].mesh->get_transform().get_quaternion();
	Quaternion terminalRotation;
	if (nextIndex.has_value()) {
		terminalRotation = railDrawMesh[nextIndex.value()].mesh->get_transform().get_quaternion();
	}
	else {
		Vector3 forward = (terminal - internal).normalize_safe();
		terminalRotation = Quaternion::LookForward(forward) * Quaternion::AngleAxis(CVector3::BASIS_Z, railPoints.back().upwardAngle.value());
	}
	worldInstance.get_transform().set_quaternion(
		Quaternion::Slerp(internalRotation, terminalRotation, parametric)
	);
}

void Rail::update_speed_from_mileage(float& speed, float mileage) const {
	// Index算出
	int index = static_cast<int>(mileage);
	float parametric = mileage - std::floor(mileage);
	std::optional<int> nextIndex;
	if (index + 1 < railLength) {
		nextIndex = index + 1;
	}
	Vector3 internal = railDrawMesh[index].mesh->world_position();
	Vector3 terminal =
		nextIndex.has_value() ?
		railDrawMesh[nextIndex.value()].mesh->world_position() :
		railPoints.back().position;
	if (nextIndex.has_value()) {
		Vector3 forward = (terminal - internal).normalize_safe();
		constexpr Vector3 GRAVITY = { 0.0f, -9.8f, 0.0f };
		float dot = Vector3::DotProduct(forward, CVector3::BACK);
		speed += dot * GRAVITY.length() * WorldClock::DeltaSeconds();
		speed = std::clamp(speed, railDrawMesh[index].minSpeed, railDrawMesh[index].maxSpeed);
	}
	else {
		speed = 0;
	}
}

void Rail::load_rail() {
	// open file
	const std::filesystem::path settingFile = LoadPath.string() + "stage.json";
	std::ifstream ifstream{ settingFile };

	// 開けなかったらログを出す
	if (ifstream.fail()) {
		std::string message = std::format("[Rail] Failed open stage file. \'{}\'\n", settingFile.string());
		ConsoleA(message);
		MessageBoxA(nullptr, message.c_str(), "Rail", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// Json読み込み
	json root{ json::parse(ifstream) };
	// 閉じる
	ifstream.close();

	json& data = root.at("Stage");

	for (const auto& railPointData : data) {
		bool haveAngle = !railPointData["Angle"].is_null();
		create_rail_point(
			{ railPointData["Position"].at(0), railPointData["Position"].at(1),railPointData["Position"].at(2) },
			railPointData["MinSpeed"], railPointData["MaxSpeed"],
			(haveAngle ? static_cast<std::optional<float>>(railPointData["Angle"].get<float>()) : std::nullopt)
		);
	}
}

void Rail::create_rail_point(const Vector3& position, float min, float max, const std::optional<float>& upward) {
	railPoints.emplace_back(
		position, upward, min, max
#ifdef _DEBUG
		, nullptr
#endif // _DEBUG
	);
}

void Rail::apply_rail_point() {
#ifdef _DEBUG
	for (auto& railPoint : railPoints) {
		std::unique_ptr<MeshInstance> temp = eps::CreateUnique<MeshInstance>("RailPoint.obj");
		temp->initialize();
		temp->get_transform().set_translate(railPoint.position);
		railPoint.debugDrawObj = std::move(temp);
	}
#endif // _DEBUG

	float upwardAngle = 0;
	Vector3 forward;
	Quaternion rotation;

	RailPoint& front = railPoints.front();
#ifdef _DEBUG
	front.debugDrawObj->get_transform().set_quaternion(rotation);
	front.debugDrawObj->begin_rendering();
#endif // _DEBUG
	if (!front.upwardAngle.has_value()) {
		front.upwardAngle = upwardAngle;
	}

	for (int i = 1; i + 1 < static_cast<int>(railPoints.size()); ++i) {
		RailPoint& railPoint = railPoints[i];
		RailPoint& next = railPoints[i + 1];
		RailPoint& prev = railPoints[i - 1];
		if (railPoint.upwardAngle.has_value()) {
			upwardAngle = railPoint.upwardAngle.value();
		}
		else {
			railPoint.upwardAngle = upwardAngle;
		}
#ifdef _DEBUG
		forward =
			(next.position - prev.position).normalize_safe(1e4f, CVector3::BASIS_Z);

		rotation = Quaternion::LookForward(forward) *
			Quaternion::AngleAxis(CVector3::BASIS_Z, upwardAngle);

		railPoint.debugDrawObj->get_transform().set_quaternion(rotation);
		railPoint.debugDrawObj->begin_rendering();
#endif // _DEBUG
	}
	RailPoint& end = railPoints.back();
#ifdef _DEBUG
	end.debugDrawObj->get_transform().set_quaternion(rotation);
	end.debugDrawObj->begin_rendering();
#endif // _DEBUG
	if (!end.upwardAngle.has_value()) {
		end.upwardAngle = upwardAngle;
	}
}

struct Control {
	Vector3 point;
	Quaternion zAngle;
	float minSpeed;
	float maxSpeed;
};

void Rail::create_rail() {
	std::vector<Control> controls;
	railDrawMesh.clear();

	float offset = 0;
	for (uint32_t baseIndex = 0; baseIndex + 1 < railPoints.size(); ++baseIndex) {
		constexpr float RAIL_LENGTH = 1;
		// 区間の長さ
		float length = Vector3::Length(railPoints[baseIndex].position, railPoints[baseIndex + 1].position);
		// 区間のレール数
		uint32_t numRail = static_cast<uint32_t>(std::ceil(length - offset) / RAIL_LENGTH);

		// インデックス
		uint32_t index0 = baseIndex != 0 ? baseIndex - 1 : 0;
		uint32_t index1 = baseIndex;
		uint32_t index2 = baseIndex + 1;
		uint32_t index3 = index2 < baseIndex ? baseIndex + 2 : index2;

		for (uint32_t index = 0; index < numRail; ++index) {
			// 全体の長さから今の媒介変数を算出
			float parametric = (index + offset) / length;

			// 位置追加
			controls.emplace_back(
				CatmullRom(
					railPoints[index0].position,
					railPoints[index1].position,
					railPoints[index2].position,
					railPoints[index3].position,
					parametric
				),
				Quaternion::Slerp(
					Quaternion::AngleAxis(CVector3::BASIS_Z, railPoints[index1].upwardAngle.value()),
					Quaternion::AngleAxis(CVector3::BASIS_Z, railPoints[index2].upwardAngle.value()),
					parametric
				),
				railPoints[index1].minSpeed,
				railPoints[index1].maxSpeed
			);
		}
		// オーバーした位置に重ならないようにoffsetを算出
		offset = numRail + offset - length;
	}

	// メモリ確保
	railDrawMesh.reserve(controls.size());

	// 先頭要素
	RailMesh& startMesh = railDrawMesh.emplace_back(RailMesh{ eps::CreateUnique<MeshInstance>("Rail.obj"), controls[0].minSpeed, controls[0].maxSpeed });
	startMesh.mesh->get_transform().set_translate(controls[0].point);
	Quaternion forward = Quaternion::LookForward((controls[1].point - controls[0].point).normalize_safe());
	startMesh.mesh->get_transform().set_quaternion(forward);
	Vector3 nextStart = controls[0].point + CVector3::BASIS_Z * forward;
	for (uint32_t i = 1; i + 1 < controls.size(); ++i) {
		RailMesh& newMesh = railDrawMesh.emplace_back(RailMesh{ eps::CreateUnique<MeshInstance>("Rail.obj"), controls[i].minSpeed, controls[i].maxSpeed });
		// 連続する位置にする
		newMesh.mesh->get_transform().set_translate(nextStart);
		// 回転算出
		forward =
			Quaternion::LookForward((controls[i + 1].point - nextStart).normalize_safe()) * controls[i].zAngle;
		newMesh.mesh->get_transform().set_quaternion(forward);
		// 次の開始位置位置
		nextStart = nextStart + CVector3::BASIS_Z * forward;
	}


	// 中間の回転
	for (uint32_t i = 1; i + 1 < railDrawMesh.size(); ++i) {
		RailMesh& mesh = railDrawMesh[i];
		RailMesh& next = railDrawMesh[i + 1];
		// 回転算出
		// 直近のupward方向を基準にforwardを決めることで、正しく回転させる
		Vector3 preUpward = CVector3::BASIS_Y * railDrawMesh[i - 1].mesh->get_transform().get_quaternion();
		Vector3 forward = (next.mesh->get_transform().get_translate() - mesh.mesh->get_transform().get_translate()).normalize_safe();
		Quaternion rotation =
			Quaternion::LookForward(forward, preUpward) * controls[i].zAngle;
		mesh.mesh->get_transform().set_quaternion(rotation);
	}

	// 終点の回転
	railDrawMesh.back().mesh->get_transform().set_quaternion(
		(railDrawMesh.rbegin() + 1)->mesh->get_transform().get_quaternion()
	);

	railLength = static_cast<float>(railDrawMesh.size());
}

#ifdef _DEBUG
void Rail::debug_draw() {
	for (RailPoint& railPoint : railPoints) {
		railPoint.debugDrawObj->begin_rendering();
		railPoint.debugDrawObj->draw();
	}
}

void Rail::editor_gui() {
	editor->debug_gui();
}
#endif // _DEBUG

void Rail::LoadMesh() {
	PolygonMeshManager::RegisterLoadQue("Resources/Model", "RailPoint.obj");
	PolygonMeshManager::RegisterLoadQue("Resources/Model", "Rail.obj");
}
