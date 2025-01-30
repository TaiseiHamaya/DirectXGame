#include "IndexBuffer.h"

constexpr UINT INDEX_DATA_SIZE = sizeof(std::uint32_t);

IndexBuffer::IndexBuffer(std::uint32_t size_) noexcept(false) {
	set_size(size_);
	create_resource();
}

const D3D12_INDEX_BUFFER_VIEW* const IndexBuffer::get_p_ibv() const noexcept {
	return &indexBufferView;
}

const std::uint32_t IndexBuffer::index_size() const noexcept {
	return size;
}

void IndexBuffer::set_size(std::uint32_t size_) {
	size = size_;
	memorySize = size * INDEX_DATA_SIZE;
}

void IndexBuffer::create_resource() {
	resource = CreateBufferResource(memorySize);
	indexBufferView.BufferLocation = resource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = memorySize;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
}

void IndexBuffer::unmap() {
	if (data) {
		resource->Unmap(0, nullptr);
		data = nullptr;
	}
}
