#include "Rail.h"

#include <Engine/Module/PolygonMesh/PolygonMeshManager.h>

#include <Engine/Utility/SmartPointer.h>
#include <Engine/Math/Definition.h>

#include "Game/GameScene/Rail/CatmullRom.h"

void Rail::initialize() {
	load_rail("");
	if (railPoints.empty()) {
		create_rail_point({0,0,-3});
		create_rail_point(CVector3::ZERO);
		create_rail_point({ 4,0,3 }, -PI/6);
		create_rail_point({ 5,3,0 }, 0.0f);
		create_rail_point({ 15,1,1 }, PI);
	}
#ifdef _DEBUG
	float upwardAngle = 0;
	Vector3 forward;
	Quaternion rotation;

	RailPoint& front = railPoints.front();
	front.debugDrawObj->get_transform().set_rotate(rotation);
	front.debugDrawObj->begin_rendering();
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
		forward =
			(next.position - prev.position).normalize_safe(1e4f, CVector3::BASIS_Z);
		
		rotation = Quaternion::LookForward(forward) *
			Quaternion::AngleAxis(CVector3::BASIS_Z, upwardAngle);

		railPoint.debugDrawObj->get_transform().set_rotate(rotation);
		railPoint.debugDrawObj->begin_rendering();
	}
	RailPoint& end = railPoints.back();
	end.debugDrawObj->get_transform().set_rotate(rotation);
	end.debugDrawObj->begin_rendering();
	if (!end.upwardAngle.has_value()) {
		end.upwardAngle = upwardAngle;
	}
#endif // _DEBUG

	create_rail();
}

void Rail::load_rail(const std::string& filename) {
}

void Rail::begin_rendering() {
	for (GameObject& railMesh : railDrawMesh) {
		railMesh.begin_rendering();
	}
}

void Rail::draw() const {
	for (const GameObject& railMesh : railDrawMesh) {
		railMesh.draw();
	}
}

void Rail::debug_draw() {
	for (RailPoint& railPoint : railPoints) {
		railPoint.debugDrawObj->begin_rendering();
		railPoint.debugDrawObj->draw();
	}
}

void Rail::create_rail_point(const Vector3& position, const std::optional<float>& upward) {
	std::unique_ptr<GameObject> temp = eps::CreateUnique<GameObject>("RailPoint.obj");
	temp->initialize();
	temp->get_transform().set_translate(position);
	railPoints.emplace_back(
		position, upward, std::move(temp)
	);
}

struct Control {
	Vector3 point;
	Quaternion zAngle;
};

void Rail::create_rail() {
	std::vector<Control> controls;
	float offset = 0;
	for (uint32_t baseIndex = 0; baseIndex + 1 < railPoints.size(); ++baseIndex) {
		// 長さ
		float length = Vector3::Length(railPoints[baseIndex].position, railPoints[baseIndex + 1].position);
		// レール数
		uint32_t numRail = static_cast<uint32_t>(std::ceil(length - offset));

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
				)
			);
		}
		// オーバーした位置に重ならないようにoffsetを算出
		offset = numRail + offset - length;
	}

	// メモリ確保
	railDrawMesh.reserve(controls.size());

	// 先頭要素
	GameObject& startMesh = railDrawMesh.emplace_back("Rail.obj");
	startMesh.get_transform().set_translate(controls[0].point);
	Quaternion forward = Quaternion::LookForward((controls[1].point - controls[0].point).normalize_safe());
	startMesh.get_transform().set_rotate(forward);
	Vector3 nextStart = controls[0].point + CVector3::BASIS_Z * forward;
	for (int i = 1; i + 1 < controls.size(); ++i) {
		GameObject& newMesh = railDrawMesh.emplace_back("Rail.obj");
		// 連続する位置にする
		newMesh.get_transform().set_translate(nextStart);
		// 回転算出
		forward = 
			Quaternion::LookForward((controls[i + 1].point - nextStart).normalize_safe()) * controls[i].zAngle;
		newMesh.get_transform().set_rotate(forward);
		// 次の開始位置位置
		nextStart = nextStart + CVector3::BASIS_Z * forward;
	}
}

void Rail::LoadMesh() {
	PolygonMeshManager::RegisterLoadQue("Resources/Model", "RailPoint.obj");
	PolygonMeshManager::RegisterLoadQue("Resources/Model", "Rail.obj");
}
