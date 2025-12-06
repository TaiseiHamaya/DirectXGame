#pragma once

#include "Engine/Module/Render/RenderPipeline/BaseRenderPipeline.h"

namespace szg {

class LinePipeline final : public BaseRenderPipeline {
public:
	LinePipeline();
	~LinePipeline() noexcept;

	LinePipeline(const LinePipeline&) = delete;
	LinePipeline& operator=(const LinePipeline&) = delete;
	LinePipeline(LinePipeline&&) = default;
	LinePipeline& operator=(LinePipeline&&) = default;

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
