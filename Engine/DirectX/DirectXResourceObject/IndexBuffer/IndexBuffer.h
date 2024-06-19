#pragma once

#include <cstdint>
#include <vector>

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"

class IndexBuffer final : public DirectXResourceObject {
public:
	IndexBuffer() noexcept = default;
	IndexBuffer(const std::vector<std::uint32_t>& indexes_) noexcept(false);
	IndexBuffer(std::uint32_t size) noexcept(false);
	~IndexBuffer() noexcept;

	IndexBuffer(IndexBuffer&&) = default;
	IndexBuffer& operator=(IndexBuffer&&) = default;

public:
	const D3D12_INDEX_BUFFER_VIEW* const get_p_ibv() const noexcept;
	const std::uint32_t index_size() const noexcept;

private:

private:
	std::uint32_t* data;
	std::uint32_t size;
	UINT memorySize;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
};
