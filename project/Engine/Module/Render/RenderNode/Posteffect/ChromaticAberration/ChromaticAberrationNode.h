#pragma once

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

#include <Library/Math/Vector2.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h"

class ChromaticAberrationNode : public SingleRenderTargetNode {
public:
	ChromaticAberrationNode();
	virtual ~ChromaticAberrationNode() noexcept;

	ChromaticAberrationNode(const ChromaticAberrationNode&) = delete;
	ChromaticAberrationNode& operator=(const ChromaticAberrationNode&) = delete;
	ChromaticAberrationNode(ChromaticAberrationNode&&) = default;
	ChromaticAberrationNode& operator=(ChromaticAberrationNode&&) = default;

public:
	void initialize() override;

	void preprocess() override {};

	/// <summary>
	/// 描画
	/// </summary>
	void draw();

	/// <summary>
	/// 描画時に使用するテクスチャリソースを設定
	/// </summary>
	/// <param name="textureGPUHandle_">テクスチャのSRVGPUハンドル</param>
	void set_texture_resource(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_);

private:
	/// <summary>
	/// PSO生成
	/// </summary>
	void create_pipeline_state();

#ifdef _DEBUG
public:
	void debug_gui();
#endif // DEBUG

private:
	D3D12_GPU_DESCRIPTOR_HANDLE textureGPUHandle{};
	ConstantBuffer<Vector2> aberrationLevel{};
};
