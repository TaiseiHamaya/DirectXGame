#pragma once

#include <d3d12.h>

/// <summary>
/// BlendState
/// </summary>
class BlendState final {
public:
	BlendState() noexcept = default;
	~BlendState() noexcept = default;

private:
	BlendState(const BlendState&) = delete;
	BlendState operator=(const BlendState&) = delete;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() noexcept;

	/// <summary>
	/// 設定の取得
	/// </summary>
	/// <returns>BLEND_DESC</returns>
	const D3D12_BLEND_DESC& get_desc() const noexcept { return blendDesc; }

private:
	void create_blend_state() noexcept;

private:
	D3D12_BLEND_DESC blendDesc{};
};
