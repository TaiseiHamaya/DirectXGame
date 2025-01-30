#include "PipelineState.h"

#include <Library/Utility/Tools/ConvertString.h>

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"

#include <format>

void PipelineState::initialize(Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_, Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_) {
	rootSignature = rootSignature_;
	graphicsPipelineState = graphicsPipelineState_;
}

void PipelineState::set_root_signature() {
	DxCommand::GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
}

void PipelineState::set_graphics_pipeline_state() {
	DxCommand::GetCommandList()->SetPipelineState(graphicsPipelineState.Get());
}

void PipelineState::set_name(const std::string& name) {
	rootSignature->SetName(ConvertString(std::format("RootSignature-({})", name)).c_str());
	graphicsPipelineState->SetName(ConvertString(std::format("PSO-({})", name)).c_str());
}
