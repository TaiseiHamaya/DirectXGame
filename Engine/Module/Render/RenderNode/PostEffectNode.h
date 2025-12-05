#pragma once

#include <memory>
#include <utility>

#include <d3d12.h>

#include "./IRenderNode.h"

#include <Library/Utility/Template/Reference.h>

#include "Engine/Module/Render/RenderPipeline/IPostEffectPipeline.h"
#include "Engine/Module/Render/RenderTargetGroup/BaseRenderTargetGroup.h"

class PostEffectNode final : public IRenderNode {
public:
	struct Data {
		Reference<BaseRenderTargetGroup> outputRenderTargetGroup;

		RECT rect;
		D3D12_VIEWPORT viewport;

		std::unique_ptr<IPostEffectPipeline> postEffectPSO;
	};

public:
	PostEffectNode() = default;
	~PostEffectNode() noexcept = default;

	__CLASS_NON_COPYABLE(PostEffectNode)

public:
	void stack_command() override;

public:
	void set_data(Data&& data_) { data = std::move(data_); };
	const Data& get_data() const { return data; };

private:
	Data data;
};
