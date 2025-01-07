#include "CallbackManagerDemo.h"

#include <Engine/Module/World/Mesh/MeshInstance.h>

CallbackManagerDemo::CallbackManagerDemo() {
	callbackFunctions.emplace(
		CallbackMapKey("Parent", "Single"),
		CallbackFunctions{
			std::bind(SetColor, std::placeholders::_1, std::placeholders::_2, Color3{ 0.0f, 0.0f, 1.0f }),
			std::bind(SetColor, std::placeholders::_1, std::placeholders::_2, Color3{ 1.0f, 0.0f, 0.0f }),
			std::bind(SetColor, std::placeholders::_1, std::placeholders::_2, Color3{ 1.0f, 1.0f, 1.0f }),
		}
		);
	callbackFunctions.emplace(
		CallbackMapKey("Single", "Child"),
		CallbackFunctions{
			nullptr,
			std::bind(SetColor, std::placeholders::_1, std::placeholders::_2, Color3{ 0.0f, 1.0f, 0.0f }),
			std::bind(SetColor, std::placeholders::_1, std::placeholders::_2, Color3{ 1.0f, 1.0f, 1.0f }),
		}
		);
	callbackFunctions.emplace(
		CallbackMapKey("Single", "Single"),
		CallbackFunctions{
			nullptr,
			std::bind(SetColor, std::placeholders::_1, std::placeholders::_2, Color3{ 1.0f, 0.0f,0.0f }),
			nullptr
		}
	);
}

void CallbackManagerDemo::SetColor(const BaseCollider* const lhs, const BaseCollider* const rhs, Color3 color) {
	auto lhsParentAddr = const_cast<WorldInstance*>(lhs->get_parent_address().ptr());
	if (lhsParentAddr) {
		MeshInstance* mesh = dynamic_cast<MeshInstance*>(lhsParentAddr);
		if (mesh) {
			//mesh->get_materials()[0].color = color;
		}
	}
	auto rhsParentAddr = const_cast<WorldInstance*>(rhs->get_parent_address().ptr());
	if (rhsParentAddr) {
		MeshInstance* mesh = dynamic_cast<MeshInstance*>(rhsParentAddr);
		if (mesh) {
			//mesh->get_materials()[0].color = color;
		}
	}
}
