#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

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

	void preprocess() override {};

private:
	void create_pipeline_state();
};
