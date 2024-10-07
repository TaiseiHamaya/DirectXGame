#include "VertexBuffer.h"

constexpr UINT VERTEX_DATA_SIZE = sizeof(VertexData);

VertexBuffer::VertexBuffer(const std::vector<VertexData>& vertices_)
	: VertexBuffer(static_cast<std::uint32_t>(vertices_.size())) {
	std::memcpy(data, vertices_.data(), memorySize);
	resource->Unmap(0, nullptr);
}

VertexBuffer::VertexBuffer(std::uint32_t size_) noexcept(false) {
	size = size_;
	memorySize = size * VERTEX_DATA_SIZE;
	resource = CreateBufferResource(memorySize);
	vertexBufferView.BufferLocation = resource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = memorySize;
	vertexBufferView.StrideInBytes = VERTEX_DATA_SIZE;

	resource->Map(0, nullptr, reinterpret_cast<void**>(&data));
}

const D3D12_VERTEX_BUFFER_VIEW* const VertexBuffer::get_p_vbv() const noexcept {
	return &vertexBufferView;
}
