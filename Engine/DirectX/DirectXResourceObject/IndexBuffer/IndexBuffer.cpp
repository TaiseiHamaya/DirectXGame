#include "IndexBuffer.h"

constexpr UINT INDEX_DATA_SIZE = sizeof(std::uint32_t);

IndexBuffer::IndexBuffer(const std::vector<std::uint32_t>& indexes_)
	: IndexBuffer(static_cast<std::uint32_t>(indexes_.size())) {
	std::memcpy(data, indexes_.data(), memorySize);
}

IndexBuffer::IndexBuffer(std::uint32_t size_) noexcept(false) {
	size = size_;
	memorySize = size * INDEX_DATA_SIZE;
	resource = CreateBufferResource(memorySize);
	indexBufferView.BufferLocation = resource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = memorySize;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	resource->Map(0, nullptr, reinterpret_cast<void**>(&data));
}

IndexBuffer::~IndexBuffer() noexcept {
	resource->Unmap(0, nullptr);
}

const D3D12_INDEX_BUFFER_VIEW* const IndexBuffer::get_p_ibv() const noexcept {
	return &indexBufferView;
}

const std::uint32_t IndexBuffer::index_size() const noexcept {
	return size;
}
