#pragma once

#include <optional>

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"

class RenderTarget : public DirectXResourceObject {
public:
	RenderTarget() noexcept = default;
	virtual ~RenderTarget() noexcept = default;

	RenderTarget(RenderTarget&&) noexcept = default;
	RenderTarget& operator=(RenderTarget&&) noexcept = default;

public:
	const D3D12_CPU_DESCRIPTOR_HANDLE& get_cpu_handle() const noexcept;
	virtual void change_resource_state();
	void create_view();
	virtual void release_index() const;
	UINT get_width() const;
	UINT get_height() const;

protected:
	bool isRendering = false;
	D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandleCPU{};
	std::optional<std::uint32_t> rtvHeapIndex;
};
