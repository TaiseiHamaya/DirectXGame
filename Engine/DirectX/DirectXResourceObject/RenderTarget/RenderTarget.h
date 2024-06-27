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
	/// <summary>
	/// RTVのCPUHandleを取得
	/// </summary>
	/// <returns></returns>
	const D3D12_CPU_DESCRIPTOR_HANDLE& get_cpu_handle() const noexcept;

	/// <summary>
	/// リソースバリアの状態遷移
	/// </summary>
	virtual void change_resource_state();

	/// <summary>
	/// RTVの生成
	/// </summary>
	void create_view(DXGI_FORMAT format);

	/// <summary>
	/// 使用しているインデックスをRTVDescriptorHeapに返す
	/// </summary>
	virtual void release_index();

	/// <summary>
	/// widthの取得
	/// </summary>
	/// <returns></returns>
	UINT get_width() const;

	/// <summary>
	/// heightの取得
	/// </summary>
	/// <returns></returns>
	UINT get_height() const;

	const D3D12_RENDER_TARGET_VIEW_DESC& get_rtv_desc() const;

protected:
	bool isRendering = false;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvCPUHandle{};
	std::optional<std::uint32_t> rtvHeapIndex;
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
};
