#include "PipelineState.h"

#include <cassert>

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/DirectX/PipelineState/BlendState/BlendState.h"
#include "Engine/DirectX/PipelineState/InputLayout/InputLayout.h"
#include "Engine/DirectX/PipelineState/RasterizerState/RasterizerState.h"
#include "Engine/DirectX/PipelineState/RootSignature/RootSignature.h"
#include "Engine/DirectX/PipelineState/ShaderManager/ShaderManager.h"

PipelineState::PipelineState() noexcept = default;

PipelineState::~PipelineState() noexcept = default;

void PipelineState::initialize() {
	create_pipeline_state();
}

void PipelineState::set_root_signature() {
	DirectXCommand::GetCommandList()->SetGraphicsRootSignature(rootSignature->get_root_signature().Get());
}

void PipelineState::set_graphics_pipeline_state() {
	DirectXCommand::GetCommandList()->SetPipelineState(graphicsPipelineState.Get());
}

void PipelineState::create_pipeline_state() {
	rootSignature = std::make_unique<RootSignature>();
	rootSignature->initialize();
	inputLayout = std::make_unique<InputLayout>();
	inputLayout->initialize();
	blendState = std::make_unique<BlendState>();
	blendState->initialize();
	rasterizerState = std::make_unique<RasterizerState>();
	rasterizerState->initialize();
	shaderManger = std::make_unique<ShaderManager>();
	shaderManger->initialize();

	// これまでの設定をまとめる
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature->get_root_signature().Get();
	graphicsPipelineStateDesc.InputLayout = inputLayout->get_desc();
	graphicsPipelineStateDesc.VS = shaderManger->get_vs_bytecode();
	graphicsPipelineStateDesc.PS = shaderManger->get_ps_bytecode();
	graphicsPipelineStateDesc.BlendState = blendState->get_desc();
	graphicsPipelineStateDesc.RasterizerState = rasterizerState->get_desc();
	graphicsPipelineStateDesc.DepthStencilState = DirectXSwapChain::GetDepthStencil().get_desc();
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // 使用するトポロジーのタイプ
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	HRESULT hr;
	hr = DirectXDevice::GetDevice()
		->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(graphicsPipelineState.GetAddressOf())); // PSOの生成
	assert(SUCCEEDED(hr));
}
