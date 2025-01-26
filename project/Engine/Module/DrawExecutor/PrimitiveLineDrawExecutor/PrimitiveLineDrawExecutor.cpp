#ifdef _DEBUG

#include "PrimitiveLineDrawExecutor.h"

#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/Resources/PrimitiveGeometry/PrimitiveGeometryManager.h"

PrimitiveLineDrawExecutor::PrimitiveLineDrawExecutor(const std::string& primitiveGeometryName, uint32_t maxInstance) {
	reinitialize(primitiveGeometryName, maxInstance);
}

void PrimitiveLineDrawExecutor::reinitialize(const std::string& primitiveGeometryName, uint32_t maxInstance) {
	primitiveGeometry = PrimitiveGeometryManager::GetPrimitiveGeometry(primitiveGeometryName);
	matrices.initialize(maxInstance);
}

void PrimitiveLineDrawExecutor::draw_command(size_t InstanceCount) const {
	auto& commandList = DirectXCommand::GetCommandList();
	if (primitiveGeometry) {
		commandList->IASetVertexBuffers(0, 1, &primitiveGeometry->get_vbv());
		commandList->IASetIndexBuffer(primitiveGeometry->get_p_ibv());
		commandList->SetGraphicsRootDescriptorTable(0, matrices.get_handle_gpu());

		commandList->DrawIndexedInstanced(primitiveGeometry->index_size(), static_cast<UINT>(InstanceCount), 0, 0, 0);
	}
}

void PrimitiveLineDrawExecutor::write_to_buffer(uint32_t index, const Matrix4x4& worldMatrix) {
	if (index < matrices.get_array().size()) {
		matrices.get_array()[index] = worldMatrix;
	}
}

#endif // _DEBUG
