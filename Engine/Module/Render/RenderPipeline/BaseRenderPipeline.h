#pragma once

#include <memory>

#include <d3dcommon.h>

#include <Library/Utility/Tools/ConstructorMacro.h>

namespace szg {

class DxPipelineState;

class BaseRenderPipeline {
public:
	BaseRenderPipeline();
	virtual ~BaseRenderPipeline() noexcept;

	SZG_CLASS_MOVE_ONLY(BaseRenderPipeline)

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
	void stack_pso_command();

protected:
	std::unique_ptr<DxPipelineState> pipelineState;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology{};
};

}; // szg
