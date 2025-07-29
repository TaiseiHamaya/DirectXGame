#include "RadialBlurNode.h"

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/PSOBuilder/PSOBuilder.h"

#ifdef DEBUG_FEATURES_ENABLE
#include <imgui.h>
#endif // _DEBUG

RadialBlurNode::RadialBlurNode() = default;

RadialBlurNode::~RadialBlurNode() noexcept = default;

void RadialBlurNode::initialize() {
	create_pipeline_state();
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	*blurInfo.get_data() = BlurInfo{
		Vector2{0.5f, 0.5f},
		0.4f,
		0.1f,
		1
	};
}

void RadialBlurNode::draw() {
	baseTexture->start_read();

	auto&& command = DxCommand::GetCommandList();
	command->SetGraphicsRootConstantBufferView(0, blurInfo.get_resource()->GetGPUVirtualAddress());
	baseTexture->get_as_srv()->use(1);
	command->DrawInstanced(3, 1, 0, 0);
}

void RadialBlurNode::set_texture_resource(Reference<RenderTexture> baseTexture_) {
	baseTexture = baseTexture_;
}

void RadialBlurNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		0, 0,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(ShaderType::Vertex, "FullscreenShader.VS.hlsl");
	psoBuilder->shaders(ShaderType::Pixel, "RadialBlur.PS.hlsl");
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());

}

#ifdef DEBUG_FEATURES_ENABLE
void RadialBlurNode::debug_gui() {
	ImGui::DragFloat2("Center", &blurInfo.get_data()->center.x, 0.01f, 0.0f, 1.0f, "%.4f");
	ImGui::DragFloat("Weight", &blurInfo.get_data()->weight, 0.001f, 0.0f, 1.0f, "%.4f");
	ImGui::DragFloat("Length", &blurInfo.get_data()->length, 0.001f, 0.0f, 1.0f, "%.4f");
	u32 min = 1, max = 16;
	ImGui::DragScalar("SampleCount", ImGuiDataType_U32, &blurInfo.get_data()->sampleCount, 0.02f, &min, &max);
}
#endif // _DEBUG