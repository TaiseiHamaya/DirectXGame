#pragma once

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"

#include <cstdint>
#include <vector>

#include "Engine/DirectX/DirectXResourceObject/BufferObjects.h"


class VertexBuffer final : public DirectXResourceObject {
public:
	VertexBuffer() noexcept = default;
	VertexBuffer(const std::vector<VertexData>& vertices_) noexcept(false);
	VertexBuffer(std::uint32_t size) noexcept(false);
	~VertexBuffer() noexcept = default;

	VertexBuffer(VertexBuffer&&) = default;
	VertexBuffer& operator=(VertexBuffer&&) = default;

public:
	const D3D12_VERTEX_BUFFER_VIEW* const get_p_vbv() const noexcept;

private:
	VertexData* data;
	std::uint32_t size;
	UINT memorySize;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
};
