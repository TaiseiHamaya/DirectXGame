#include "StaticTexture.h"

#include <format>

StaticTexture::~StaticTexture() {
	srv.release();
}

void StaticTexture::initialize(Microsoft::WRL::ComPtr<ID3D12Resource> resource_) {
	resource = resource_;
	D3D12_RESOURCE_DESC desc = resource->GetDesc();
	width = static_cast<u32>(desc.Width);
	height = desc.Height;
	srv.create(this, desc.Format);

	state = D3D12_RESOURCE_STATE_GENERIC_READ;
}

void StaticTexture::set_name(const std::wstring& name) {
	resource->SetName(std::format(L"StaticTexture-SRV{}({})", srv.get_index().value(), name).c_str());
}
