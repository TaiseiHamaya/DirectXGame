#pragma once

#include <memory>
#include <utility>

#include <d3d12.h>

#include "./IRenderNode.h"

#include <Library/Utility/Template/Reference.h>

#include <Engine/Module/Render/RenderPSO/PostEffectPSO.h>
#include <Engine/Module/Render/RenderTargetGroup/BaseRenderTargetGroup.h>

class PostEffectNode final : public IRenderNode {
public:
	struct Data {
		Reference<BaseRenderTargetGroup> outRenderTargetGroup;

		RECT rect;
		D3D12_VIEWPORT viewport;

		std::unique_ptr<PostEffectPSO> postEffectPSO;
	};

public:
	PostEffectNode() = default;
	~PostEffectNode() noexcept = default;

	__CLASS_NON_COPYABLE(PostEffectNode)

public:
	void initialize();

	void stack_command() override;

public:
	void set_data(Data&& data_) { data = std::move(data_); };
	const Data& get_data() const { return data; };

private:
	Data data;
};
