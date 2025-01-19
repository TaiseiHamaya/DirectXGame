#ifdef _DEBUG

#include "LineGroupDrawExecutor.h"

#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/Resources/Debug/LineGroup/LineGroupManager.h"

LineGroupDrawExecutor::LineGroupDrawExecutor(const std::string& lineGroupName, uint32_t maxInstance) {
	reinitialize(lineGroupName, maxInstance);
}

void LineGroupDrawExecutor::reinitialize(const std::string& lineGroupName, uint32_t maxInstance) {
	lineGroup = LineGroupManager::GetLineGroup(lineGroupName);
	matrices.initialize(maxInstance);
}

void LineGroupDrawExecutor::draw_command(size_t InstanceCount) const {
	auto& commandList = DirectXCommand::GetCommandList();
	if (lineGroup) {
		commandList->IASetVertexBuffers(0, 1, &lineGroup->get_vbv());
		commandList->IASetIndexBuffer(lineGroup->get_p_ibv());
		commandList->SetGraphicsRootDescriptorTable(0, matrices.get_handle_gpu());

		commandList->DrawIndexedInstanced(lineGroup->index_size(), static_cast<UINT>(InstanceCount), 0, 0, 0);
	}
}

void LineGroupDrawExecutor::write_to_buffer(uint32_t index, const Matrix4x4& worldMatrix) {
	if (index < matrices.get_array().size()) {
		matrices.get_array()[index] = worldMatrix;
	}
}

#endif // _DEBUG
