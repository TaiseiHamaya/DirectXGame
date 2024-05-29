#pragma once

#include <cstdint>
#include <vector>

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"

class IndexBuffer final : public DirectXResourceObject {
public:
	IndexBuffer();
	IndexBuffer(const std::vector<std::uint32_t>& indexes_);
	IndexBuffer(std::uint32_t size);
	~IndexBuffer();

public:
	const D3D12_INDEX_BUFFER_VIEW* const get_p_ibv() const;
	const std::uint32_t get_index_size() const;

private:

private:
	struct Indexes {
		std::uint32_t* data;
		std::uint32_t size;
		UINT memorySize;
	} indexes;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
};
