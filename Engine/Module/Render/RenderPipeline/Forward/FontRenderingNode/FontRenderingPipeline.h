#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"
#include "Engine/Module/Render/RenderPipeline/BaseRenderPipeline.h"

namespace szg {

enum class PsoBlendMode : u32;

class FontRenderingPipeline final : public BaseRenderPipeline {
public:
	FontRenderingPipeline();
	~FontRenderingPipeline() noexcept;

	SZG_CLASS_MOVE_ONLY(FontRenderingPipeline)

public:
	static void BeginLoadShader();

	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void initialize(BlendMode blendMode);

	void preprocess() override {};

private:
	void create_pipeline_state(PsoBlendMode blendMode);

private:
};

}; // szg
