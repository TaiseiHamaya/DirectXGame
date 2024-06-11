#include "VertexBuffer.h"

constexpr UINT VERTEX_DATA_SIZE = sizeof(VertexData);

VertexBuffer::VertexBuffer() : VertexBuffer(0) {
}

VertexBuffer::VertexBuffer(const std::vector<VertexData>& vertices_)
	: VertexBuffer(static_cast<std::uint32_t>(vertices_.size())) {
	std::memcpy(vertices.data, vertices_.data(), vertices.memorySize);
}

VertexBuffer::VertexBuffer(std::uint32_t size) noexcept(false) {
	vertices = { nullptr, size, size * VERTEX_DATA_SIZE };
	resource = CreateBufferResource(vertices.memorySize);
	vertexBufferView.BufferLocation = resource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = vertices.memorySize;
	vertexBufferView.StrideInBytes = VERTEX_DATA_SIZE;

	resource->Map(0, nullptr, reinterpret_cast<void**>(&vertices.data));
}

VertexBuffer::~VertexBuffer() noexcept {
	resource->Unmap(0, nullptr);
}

const D3D12_VERTEX_BUFFER_VIEW* const VertexBuffer::get_p_vbv() const noexcept {
	return &vertexBufferView;
}
