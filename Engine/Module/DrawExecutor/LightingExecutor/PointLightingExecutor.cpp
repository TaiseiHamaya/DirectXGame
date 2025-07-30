#include "PointLightingExecutor.h"

#include "Engine/Assets/PrimitiveGeometry/PrimitiveGeometryLibrary.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/Module/World/Light/PointLight/PointLightInstance.h"

PointLightingExecutor::PointLightingExecutor(u32 maxInstance) {
	reinitialize(maxInstance);
}

void PointLightingExecutor::reinitialize(u32 maxInstance_) {
	asset = PrimitiveGeometryLibrary::GetPrimitiveGeometry("Ico3");
	maxInstance = maxInstance_;
	matrices.initialize(maxInstance);
	lightData.initialize(maxInstance);
}

void PointLightingExecutor::draw_command() const {
	if (!asset) {
		return;
	}

	auto& commandList = DxCommand::GetCommandList();
	commandList->IASetVertexBuffers(0, 1, &asset->get_vbv());
	commandList->IASetIndexBuffer(asset->get_p_ibv());
	commandList->SetGraphicsRootDescriptorTable(0, matrices.get_handle_gpu());
	commandList->SetGraphicsRootDescriptorTable(5, lightData.get_handle_gpu());

	commandList->DrawIndexedInstanced(asset->index_size(), instanceCounter, 0, 0, 0);
}

void PointLightingExecutor::write_to_buffer(Reference<const PointLightInstance> instance) {
	if (!instance) {
		return;
	}
	if (instanceCounter >= maxInstance) {
		return;
	}
	matrices[instanceCounter] = instance->transform_matrix();
	lightData[instanceCounter] = instance->light_data();
	++instanceCounter;
}
