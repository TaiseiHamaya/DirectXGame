#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"

enum class PsoBlendMode : u32;

class Rect3dNode final : public SingleRenderTargetNode {
public:
	Rect3dNode();
	~Rect3dNode() noexcept;

	__CLASS_NON_COPYABLE(Rect3dNode)

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void initialize(BlendMode blendMode);

	void preprocess() override {};

private:
	void create_pipeline_state(PsoBlendMode blendMode);
};
