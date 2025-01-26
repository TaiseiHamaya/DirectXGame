#include "DirectionalLightInstance.h"

#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"

void DirectionalLightInstance::begin_rendering() {
	auto lightData = lightBuffer.get_data();
}

void DirectionalLightInstance::draw_deferred() const {
	auto&& command = DirectXCommand::GetCommandList();
	command->SetGraphicsRootConstantBufferView(0, lightBuffer.get_resource()->GetGPUVirtualAddress());
	command->DrawInstanced(3, 1, 0, 0);
}

#ifdef _DEBUG
#include <imgui.h>
#include <format>
void DirectionalLightInstance::debug_gui() {
	auto lightData = lightBuffer.get_data();

	if (!lightData) {
		return;
	}
	// 色
	lightData->color.debug_gui();
	// 強度
	ImGui::DragFloat("Intensity", &lightData->intensity, 0.01f, 0.0f, (std::numeric_limits<float>::max)());
	// 向き
	Vector3 rotate = CVector3::ZERO;
	ImGui::Text(std::format("X : {:.3}, Y : {:.3}, Z : {:.3}", lightData->direction.x, lightData->direction.y, lightData->direction.z).c_str());
	if (ImGui::DragFloat3("DirectionRotate", &rotate.x, 1.0f, -180.0f, 180.0f)) {
		lightData->direction = lightData->direction * Quaternion::EulerDegree(rotate);
	}
	// 向きリセットボタン
	ImGui::Text("ResetDirection");
	if (ImGui::Button("X")) {
		lightData->direction = CVector3::BASIS_X;
	}
	ImGui::SameLine();
	if (ImGui::Button("Y")) {
		lightData->direction = CVector3::BASIS_Y;
	}
	ImGui::SameLine();
	if (ImGui::Button("Z")) {
		lightData->direction = CVector3::BASIS_Z;
	}
	ImGui::SameLine();
	if (ImGui::Button("-X")) {
		lightData->direction = -CVector3::BASIS_X;
	}
	ImGui::SameLine();
	if (ImGui::Button("-Y")) {
		lightData->direction = -CVector3::BASIS_Y;
	}
	ImGui::SameLine();
	if (ImGui::Button("-Z")) {
		lightData->direction = -CVector3::BASIS_Z;
	}
}
#endif // _DEBUG
