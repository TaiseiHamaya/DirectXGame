#include "SpriteNode.h"

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"
#include "Engine/WinApp.h"
#include "Engine/DirectX/DirectXResourceObject/IndexBuffer/IndexBuffer.h"
#include "Engine/DirectX/DirectXResourceObject/VertexBuffer/VertexBuffer.h"

SpriteNode::SpriteNode() = default;

SpriteNode::~SpriteNode() noexcept = default;

void SpriteNode::initialize() {
	create_pipeline_state();
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void SpriteNode::begin() {
	SingleRenderTargetNode::begin();

	if (backgroundTextureGPUHandle.has_value()) {
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList = DirectXCommand::GetCommandList();
		// 設定したデータをコマンドに積む
		commandList->IASetVertexBuffers(0, 1, vertices->get_p_vbv()); // VBV
		commandList->IASetIndexBuffer(indexes->get_p_ibv());
		commandList->SetGraphicsRootConstantBufferView(0, transformMatrix->get_resource()->GetGPUVirtualAddress()); // Matrix
		commandList->SetGraphicsRootConstantBufferView(1, material->get_resource()->GetGPUVirtualAddress()); // Color,UV
		commandList->SetGraphicsRootDescriptorTable(2, backgroundTextureGPUHandle.value()); // Texture
		commandList->DrawIndexedInstanced(indexes->index_size(), 1, 0, 0, 0); // 描画コマンド
	}
}

void SpriteNode::set_background_texture(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_) {
	backgroundTextureGPUHandle = textureGPUHandle_;
	create_background_draw_data();
}

void SpriteNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 0);
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		0,
		D3D12_FILTER_ANISOTROPIC
	);

	InputLayoutBuilder inputLayoutBuilder;
	inputLayoutBuilder.add_element("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	inputLayoutBuilder.add_element("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);

	ShaderBuilder shaderBuilder;
	shaderBuilder.initialize(
		"Engine/Render/RenderNode/Sprite/Sprite.VS.hlsl",
		"Engine/Render/RenderNode/Sprite/Sprite.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate(BlendMode::Normal);
	psoBuilder->inputlayout(inputLayoutBuilder.build());
	psoBuilder->rasterizerstate(D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderBuilder);
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<PipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());

}

void SpriteNode::create_background_draw_data() {
	std::vector<std::uint32_t> indexData{ 0,1,2,1,3,2 };
	indexes = std::make_unique<IndexBuffer>(indexData);

	Vector2 base = { static_cast<float>(WinApp::GetClientHight()), static_cast<float>(WinApp::GetClientWidth()) };
	std::vector<VertexData> vertexData(4);
	vertexData[0] = {
		VertexData::Vector4{ Vector2{-1, 1}.convert(0), 1},
		CVector2::ZERO
	};
	vertexData[1] = {
		VertexData::Vector4{ Vector2{1, 1}.convert(0.0f), 1},
		CVector2::BASIS_X
	};
	vertexData[2] = {
		VertexData::Vector4{ Vector2{-1, -1}.convert(0), 1},
		CVector2::BASIS_Y
	};
	vertexData[3] = {
		VertexData::Vector4{ Vector2{1, -1}.convert(0), 1},
		CVector2::BASIS
	};

	vertices = std::make_unique<VertexBuffer>(vertexData);

	material = std::make_unique<ConstantBuffer<SpriteMaterial>>(SpriteMaterial{ Color{ 1.0f,1.0f,1.0f,1.0f }, CMatrix4x4::IDENTITY });
	transformMatrix = std::make_unique<ConstantBuffer<Matrix4x4>>(CMatrix4x4::IDENTITY);

}
