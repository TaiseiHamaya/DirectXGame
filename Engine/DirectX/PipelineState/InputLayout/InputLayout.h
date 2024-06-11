#pragma once

#include <d3d12.h>

class InputLayout final {
public:
	InputLayout() noexcept = default;
	~InputLayout() noexcept = default;

private:
	InputLayout(const InputLayout&) = delete;
	InputLayout operator=(const InputLayout&) = delete;

public:
	void initialize() noexcept;
	const D3D12_INPUT_LAYOUT_DESC& get_desc() const noexcept { return inputLayoutDesc; }

private:
	void create_input_layout() noexcept;

private:
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
};
