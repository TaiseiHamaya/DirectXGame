#include "CallbackManagerDemo.h"

#include <Engine/Module/World/Mesh/MeshInstance.h>
#include <Library/Math/Color.h>

CallbackManagerDemo::CallbackManagerDemo() {
	callbackFunctions.emplace(
		CallbackMapKey("Parent", "Single"),
		CallbackFunctions{
			std::bind(SetColor, std::placeholders::_1, std::placeholders::_2, Color{0.0f, 0.0f, 1.0f, 1.0f}),
			std::bind(SetColor, std::placeholders::_1, std::placeholders::_2, Color{1.0f, 0.0f, 0.0f, 1.0f}),
			std::bind(SetColor, std::placeholders::_1, std::placeholders::_2, Color{1.0f, 1.0f, 1.0f, 1.0f}),
		}
	);
	callbackFunctions.emplace(
		CallbackMapKey("Single", "Child"),
		CallbackFunctions{
			nullptr,
			std::bind(SetColor, std::placeholders::_1, std::placeholders::_2, Color{0.0f, 1.0f, 0.0f, 1.0f}),
			std::bind(SetColor, std::placeholders::_1, std::placeholders::_2, Color{1.0f, 1.0f, 1.0f, 1.0f}), 
		}
	);
	callbackFunctions.emplace(
		CallbackMapKey("Single", "Single"),
		CallbackFunctions{
			nullptr,
			std::bind(SetColor, std::placeholders::_1, std::placeholders::_2, Color{1.0f, 0.0f,0.0f,1.0f}),
			nullptr
		}
	);
}

void CallbackManagerDemo::SetColor(const BaseCollider* const lhs, const BaseCollider* const rhs, Color color) {
	auto lhsParentAddr = const_cast<WorldInstance*>(lhs->get_parent_address());
	if (lhsParentAddr) {
		MeshInstance* mesh = dynamic_cast<MeshInstance*>(lhsParentAddr);
		if (mesh) {
			mesh->get_materials()[0].color = color;
		}
	}
	auto rhsParentAddr = const_cast<WorldInstance*>(rhs->get_parent_address());
	if (rhsParentAddr) {
		MeshInstance* mesh = dynamic_cast<MeshInstance*>(rhsParentAddr);
		if (mesh) {
			mesh->get_materials()[0].color = color;
		}
	}
}
