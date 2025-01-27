#include "PointLightingExecutor.h"

#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/Resources/PrimitiveGeometry/PrimitiveGeometryManager.h"
#include "Engine/Module/World/Light/PointLight/PointLightInstance.h"

void PointLightingExecutor::reinitialize(const std::string& primitiveGeometryName, uint32_t maxInstance) {
	resource = PrimitiveGeometryManager::GetPrimitiveGeometry(primitiveGeometryName);
	matrices.initialize(maxInstance);
	lightData.initialize(maxInstance);
}

void PointLightingExecutor::draw_command(uint32_t InstanceCount) const {
	auto& commandList = DirectXCommand::GetCommandList();
	if (resource) {
		commandList->IASetVertexBuffers(0, 1, &resource->get_vbv());
		commandList->IASetIndexBuffer(resource->get_p_ibv());
		commandList->SetGraphicsRootDescriptorTable(0, matrices.get_handle_gpu());
		commandList->SetGraphicsRootDescriptorTable(5, lightData.get_handle_gpu());

		commandList->DrawIndexedInstanced(resource->index_size(), InstanceCount, 0, 0, 0);
	}
}

void PointLightingExecutor::write_to_buffer(uint32_t index, const Matrix4x4& worldMatrix, const PointLightData& lightData_) {
	if (index < matrices.get_array().size()) {
		matrices.get_array()[index] = worldMatrix;
		lightData.get_array()[index] = lightData_;
	}
}
