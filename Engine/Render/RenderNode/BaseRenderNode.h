#pragma once

struct D3D12_CPU_DESCRIPTOR_HANDLE;
class PipelineState;
class PSOBuilder;
class DepthStencil;
struct D3D12_VIEWPORT;
struct tagRECT;
enum D3D_PRIMITIVE_TOPOLOGY;
class Color;

#include <memory>
#include <vector>

class BaseRenderNode {
public:
	BaseRenderNode();
	virtual ~BaseRenderNode() noexcept;

	BaseRenderNode(const BaseRenderNode&) = delete;
	BaseRenderNode& operator=(const BaseRenderNode&) = delete;
	BaseRenderNode(BaseRenderNode&&) = default;
	BaseRenderNode& operator=(BaseRenderNode&&) = default;

public:
	virtual void initialize() = 0;
	virtual void begin();
	virtual void end();

protected:
	void create_view_port(std::uint32_t width, std::uint32_t height);

protected:
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> renderTargetHandles;
	std::unique_ptr<PipelineState> pipelineState;
	std::shared_ptr<DepthStencil> depthStencil;
	std::unique_ptr<D3D12_VIEWPORT> viewPort;
	std::unique_ptr<tagRECT> scissorRect;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology{};
	std::unique_ptr<Color> clearColor;
};
