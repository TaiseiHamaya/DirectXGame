#include "VertexBuffer.h"

//constexpr UINT VERTEX_DATA_SIZE = sizeof(VertexData);
//
//VertexBuffer::VertexBuffer(std::uint32_t size_) noexcept(false) {
//	set_size(size_);
//	create_resource();
//}
//
//const D3D12_VERTEX_BUFFER_VIEW* const VertexBuffer::get_p_vbv() const noexcept {
//	return &vertexBufferView;
//}
//
//const std::uint32_t VertexBuffer::vertex_size() const noexcept {
//	return size;
//}
//
//void VertexBuffer::set_size(std::uint32_t size_) {
//	size = size_;
//	memorySize = size * VERTEX_DATA_SIZE;
//}
//
//void VertexBuffer::create_resource() {
//	resource = CreateBufferResource(memorySize);
//	vertexBufferView.BufferLocation = resource->GetGPUVirtualAddress();
//	vertexBufferView.SizeInBytes = memorySize;
//	vertexBufferView.StrideInBytes = VERTEX_DATA_SIZE;
//}
//
//void VertexBuffer::unmap() {
//	if (data) {
//		resource->Unmap(0, nullptr);
//		data = nullptr;
//	}
//}
