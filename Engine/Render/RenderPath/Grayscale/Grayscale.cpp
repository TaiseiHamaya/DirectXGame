#include "Grayscale.h"

#include "Engine/RenderingPathManager/RenderingParameters/RenderingParameters.h"
#include "Engine/DirectX/DirectXResourceObject/OffscreenRender/OffscreenRender.h"
#include "Engine/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"

void Grayscale::initialize() {
	parameter = std::make_shared<RenderingParameters>();
	OffscreenRender renderTarget;
	renderTarget.initialize();
	drawFunc = std::bind(&OffscreenRender::draw, &renderTarget, 0);

	std::unique_ptr<PSOBuilder> builder;
	create_builder(builder);

	parameter->initialize(
		std::move(renderTarget),
		builder,
		D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
}

void Grayscale::create_builder(std::unique_ptr<PSOBuilder>& builder) {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.descriptor_range();
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignatureBuilder.sampler(
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_COMPARISON_FUNC_NEVER,
		D3D12_SHADER_VISIBILITY_PIXEL,
		0
	);

	InputLayoutBuillder inputLayoutBuillder;
	inputLayoutBuillder.add_cbv("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	inputLayoutBuillder.add_cbv("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);
	inputLayoutBuillder.add_cbv("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT);

	ShaderBuilder shaderManager;
	shaderManager.initialize(
		"Engine/HLSL/PostEffect/PostEffectTest.VS.hlsl",
		"Engine/HLSL/PostEffect/PostEffectTest.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder= std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->inputlayout(inputLayoutBuillder.build());
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderManager);
	psoBuilder->primitivetopologytype();
	builder = std::move(psoBuilder);
}
