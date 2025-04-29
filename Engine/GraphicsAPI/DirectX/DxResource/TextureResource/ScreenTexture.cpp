#include "ScreenTexture.h"

#include "Engine/Application/EngineSettings.h"
#include "Engine/GraphicsAPI/DirectX/DxSystemValues.h"

ScreenTexture::~ScreenTexture() {
	rtv.release();
}

void ScreenTexture::initialize(Microsoft::WRL::ComPtr<ID3D12Resource> resource_) {
	resource = resource_;
	width = EngineSettings::CLIENT_WIDTH;
	height = EngineSettings::CLIENT_HEIGHT;
	rtv.create(this, DxSystemValues::SCREEN_RTV_FORMAT);

	state = D3D12_RESOURCE_STATE_PRESENT;

	set_name(std::format(L"ScreenTexture-RTV{}", rtv.get_index().value()));
}

void ScreenTexture::start_write() {
	change_resource_state(rtv.use_state());
}

void ScreenTexture::start_present() {
	change_resource_state(D3D12_RESOURCE_STATE_PRESENT);
}
