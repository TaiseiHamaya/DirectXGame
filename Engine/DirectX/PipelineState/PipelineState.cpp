#include "PipelineState.h"

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"

PipelineState::PipelineState() noexcept = default;

PipelineState::~PipelineState() noexcept = default;

void PipelineState::initialize(Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_, Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_) {
	rootSignature = rootSignature_;
	graphicsPipelineState = graphicsPipelineState_;
}

void PipelineState::set_root_signature() {
	DirectXCommand::GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
}

void PipelineState::set_graphics_pipeline_state() {
	DirectXCommand::GetCommandList()->SetPipelineState(graphicsPipelineState.Get());
}
