#include "DirectionalLightingExecutor.h"

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h"

DirectionalLightingExecutor::DirectionalLightingExecutor(uint32_t maxInstance) {
	reinitialize(maxInstance);
}

void DirectionalLightingExecutor::reinitialize(uint32_t maxInstance) {
	lightData.initialize(maxInstance);
}

void DirectionalLightingExecutor::draw_command(uint32_t InstanceCount) const {
	auto&& command = DxCommand::GetCommandList();
	command->SetGraphicsRootDescriptorTable(0, lightData.get_handle_gpu());
	command->DrawInstanced(3, InstanceCount, 0, 0);
}

void DirectionalLightingExecutor::write_to_buffer(uint32_t index, const DirectionalLightData& lightData_) {
	if (index < lightData.get_array().size()) {
		lightData.get_array()[index] = lightData_;
	}
}
