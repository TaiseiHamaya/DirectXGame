#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Module/Render/RenderPipeline/BaseRenderPipeline.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"

enum class PsoBlendMode : u32;

class Rect3dPipeline final : public BaseRenderPipeline {
public:
	Rect3dPipeline();
	~Rect3dPipeline() noexcept;

	__CLASS_NON_COPYABLE(Rect3dPipeline)

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
