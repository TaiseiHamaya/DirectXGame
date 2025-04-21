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

void DirectionalLightingExecutor::set_command(uint32_t paramIndex) const {
	auto&& command = DxCommand::GetCommandList();
	command->SetGraphicsRootDescriptorTable(paramIndex, lightData.get_handle_gpu());
}

void DirectionalLightingExecutor::write_to_buffer(Reference<const DirectionalLightInstance> instance) {
	if (!instance || !instance->is_active()) {
		return;
	}
	if (instanceCounter >= maxInstance) {
		return;
	}

	lightData[instanceCounter] = instance->light_data();
	++instanceCounter;
}
