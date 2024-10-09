#include "Rail.h"

#include <Engine/Module/PolygonMesh/PolygonMeshManager.h>

#include <Engine/Utility/SmartPointer.h>

void Rail::initialize() {
	load_rail("");
	if (railPoints.empty()) {
		create_rail_point(CVector3::ZERO, std::nullopt);
	}
	if (railPoints.size() == 1) {
		create_rail_point(CVector3::BASIS_Z, std::nullopt);
	}
#ifdef _DEBUG
	Vector3 upward = CVector3::BASIS_Y;
	for (int i = 0; i + 1 < static_cast<int>(railPoints.size()); ++i) {
		RailPoint& railPoint = railPoints[i];
		RailPoint& next = railPoints[i + 1];
		if (railPoint.upward.has_value()) {
			upward = railPoint.upward.value();
		}
		Vector3 forward =
			(next.position - railPoint.position).normalize_safe(1e4f, CVector3::BASIS_Z);
		railPoint.debugDrawObj->get_transform().set_rotate(
			Quaternion::LookForward(forward, upward)
		);
		railPoint.debugDrawObj->begin_rendering();
	}
#endif // _DEBUG


}

void Rail::load_rail(const std::string& filename) {
}

void Rail::debug_draw() {
	for (RailPoint& railPoint : railPoints) {
		railPoint.debugDrawObj->begin_rendering();
		railPoint.debugDrawObj->draw();
	}
}

void Rail::create_rail_point(const Vector3& position, const std::optional<Vector3>& upward) {
	std::unique_ptr<GameObject> temp = eps::CreateUnique<GameObject>("RailPoint.obj");
	temp->initialize();
	temp->get_transform().set_translate(position);
	railPoints.emplace_back(
		position, upward, std::move(temp)
	);
}

void Rail::LoadMesh() {
	PolygonMeshManager::RegisterLoadQue("Resources/Model", "RailPoint.obj");
	PolygonMeshManager::RegisterLoadQue("Resources/Model", "Rail.obj");
}
