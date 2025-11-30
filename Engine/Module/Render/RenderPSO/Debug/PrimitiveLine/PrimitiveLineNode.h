#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "Engine/Module/Render/RenderPSO/BaseRenderNode.h"

class PrimitiveLineNode final : public BaseRenderNode {
public:
	PrimitiveLineNode();
	~PrimitiveLineNode() noexcept;

	PrimitiveLineNode(const PrimitiveLineNode&) = delete;
	PrimitiveLineNode& operator=(const PrimitiveLineNode&) = delete;
	PrimitiveLineNode(PrimitiveLineNode&&) = default;
	PrimitiveLineNode& operator=(PrimitiveLineNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

private:
	void create_pipeline_state();
};

#endif // _DEBUG
