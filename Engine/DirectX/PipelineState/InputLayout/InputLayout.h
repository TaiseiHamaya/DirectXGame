#pragma once

#include <d3d12.h>

class InputLayout {
public:
	InputLayout() = default;
	~InputLayout() = default;

private:
	InputLayout(const InputLayout&) = delete;
	InputLayout operator=(const InputLayout&) = delete;

public:
	void initialize();
	const D3D12_INPUT_LAYOUT_DESC& get_desc() const { return inputLayoutDesc; }

private:
	void create_input_layout();

private:
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
};
