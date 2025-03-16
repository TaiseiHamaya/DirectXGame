#include "DirectionalLightingExecutor.h"

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h"

DirectionalLightingExecutor::DirectionalLightingExecutor(uint32_t maxInstance_) {
	reinitialize(maxInstance_);
}

void DirectionalLightingExecutor::reinitialize(uint32_t maxInstance_) {
	maxInstance = maxInstance_;
	lightData.initialize(maxInstance);
}

void DirectionalLightingExecutor::draw_command() const {
	auto&& command = DxCommand::GetCommandList();
	command->SetGraphicsRootDescriptorTable(0, lightData.get_handle_gpu());
	command->DrawInstanced(3, instanceCounter, 0, 0);
}

void DirectionalLightingExecutor::write_to_buffer(const DirectionalLightData& lightData_) {
	if (instanceCounter >= maxInstance) {
		return;
	}

	lightData[instanceCounter] = lightData_;
	++instanceCounter;
}
