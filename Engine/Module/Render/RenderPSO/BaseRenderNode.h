#pragma once

#include <memory>

#include <Library/Utility/Template/bitflag.h>

class DepthStencilTexture;
class DxPipelineState;
enum D3D_PRIMITIVE_TOPOLOGY;

enum class RenderNodeConfig : u8 {
	Default = 0,
	NoClearRenderTarget = 1 << 0,
	NoClearDepth = 1 << 1,
};

// BitFlag型を使用
__USE_BITFLAG(RenderNodeConfig)

class BaseRenderNode {
public:
	BaseRenderNode();
	virtual ~BaseRenderNode() noexcept;

	BaseRenderNode(const BaseRenderNode&) = delete;
	BaseRenderNode& operator=(const BaseRenderNode&) = delete;
	BaseRenderNode(BaseRenderNode&&) = default;
	BaseRenderNode& operator=(BaseRenderNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void initialize() = 0;

	/// <summary>
	/// 使用決定時処理
	/// </summary>
	virtual void preprocess() = 0;

	/// <summary>
	/// 描画開始
	/// </summary>
	virtual void begin();

protected:
	std::unique_ptr<DxPipelineState> pipelineState;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology{};
};
