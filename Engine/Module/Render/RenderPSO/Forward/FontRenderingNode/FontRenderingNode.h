#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"
#include "Engine/Module/Render/RenderPSO/BaseRenderNode.h"

enum class PsoBlendMode : u32;

class FontRenderingNode final : public BaseRenderNode {
public:
	FontRenderingNode();
	~FontRenderingNode() noexcept;

	__CLASS_NON_COPYABLE(FontRenderingNode)

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
