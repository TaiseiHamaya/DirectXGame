#ifdef _DEBUG

#include "PrimitiveGeometryDrawExecutor.h"

#include "Engine/Assets/PrimitiveGeometry/PrimitiveGeometryLibrary.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"

void PrimitiveGeometryDrawExecutor::reinitialize(const std::string& primitiveGeometryName, uint32_t maxInstance) {
	resource = PrimitiveGeometryLibrary::GetPrimitiveGeometry(primitiveGeometryName);
	matrices.initialize(maxInstance);
}

void PrimitiveGeometryDrawExecutor::draw_command(uint32_t InstanceCount) const {
	auto& commandList = DxCommand::GetCommandList();
	if (resource) {
		commandList->IASetVertexBuffers(0, 1, &resource->get_vbv());
		commandList->IASetIndexBuffer(resource->get_p_ibv());
		commandList->SetGraphicsRootDescriptorTable(0, matrices.get_handle_gpu());

		commandList->DrawIndexedInstanced(resource->index_size(), InstanceCount, 0, 0, 0);
	}
}

void PrimitiveGeometryDrawExecutor::write_to_buffer(uint32_t index, const Matrix4x4& worldMatrix) {
	if (index < matrices.get_array().size()) {
		matrices.get_array()[index] = worldMatrix;
	}
}

#endif // _DEBUG
