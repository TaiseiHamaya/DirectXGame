#pragma once

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"

#include <cstdint>
#include <vector>

#include "Engine/DirectX/DirectXResourceObject/BufferObjects.h"


class VertexBuffer final : public DirectXResourceObject {
public:
	VertexBuffer();
	VertexBuffer(const std::vector<VertexData>& vertices_);
	VertexBuffer(std::uint32_t size);
	~VertexBuffer();

public:
	const D3D12_VERTEX_BUFFER_VIEW* const get_p_vbv() const;

private:

private:
	struct Vertices {
		VertexData* data;
		std::uint32_t size;
		UINT memorySize;
	} vertices;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
};
