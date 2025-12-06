#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "Engine/Module/Render/RenderPipeline/BaseRenderPipeline.h"

namespace szg {

class PrimitiveLinePipeline final : public BaseRenderPipeline {
public:
	PrimitiveLinePipeline();
	~PrimitiveLinePipeline() noexcept;

	PrimitiveLinePipeline(const PrimitiveLinePipeline&) = delete;
	PrimitiveLinePipeline& operator=(const PrimitiveLinePipeline&) = delete;
	PrimitiveLinePipeline(PrimitiveLinePipeline&&) = default;
	PrimitiveLinePipeline& operator=(PrimitiveLinePipeline&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

private:
	void create_pipeline_state();
};

}; // szg

#endif // _DEBUG
