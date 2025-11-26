#include "GrayscaleNode.h"

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/PSOBuilder/PSOBuilder.h"
#include "Engine/Module/Manager/RuntimeStorage/RuntimeStorage.h"

#ifdef DEBUG_FEATURES_ENABLE
#include <imgui.h>
#endif // _DEBUG

GrayscaleNode::GrayscaleNode() = default;

GrayscaleNode::~GrayscaleNode() = default;

void GrayscaleNode::initialize() {
	create_pipeline_state();
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	*isGray.get_data() = false;
}

void GrayscaleNode::preprocess() {
	if (!groupName.has_value()) {
		return;
	}

	Reference<const std::any> strangeValue = RuntimeStorage::GetValueImm("PostEffect", groupName.value());
	if (strangeValue.is_null()) {
		return;
	}
	*isGray.get_data() = std::any_cast<bool>(*strangeValue);
}

void GrayscaleNode::execute_effect_command() {
	baseTexture->start_read();

	auto&& command = DxCommand::GetCommandList();
	baseTexture->get_as_srv()->use(1);
	command->SetGraphicsRootConstantBufferView(0, isGray.get_resource()->GetGPUVirtualAddress());
	command->DrawInstanced(3, 1, 0, 0);
}

void GrayscaleNode::set_shader_texture(Reference<RenderTexture> baseTexture_) {
	baseTexture = baseTexture_;
}

void GrayscaleNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		0
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(ShaderType::Vertex, "FullscreenShader.VS.hlsl");
	psoBuilder->shaders(ShaderType::Pixel, "Grayscale.PS.hlsl");
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}

#ifdef DEBUG_FEATURES_ENABLE
void GrayscaleNode::debug_gui() {
	ImGui::Checkbox("IsGray", reinterpret_cast<bool*>(isGray.get_data()));
}
#endif // _DEBUG
