#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <variant>

#include <d3d12.h>
#include <DirectXTex.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/DxResource.h"

class Texture final : public DxResource {
public: // constructor
	Texture() noexcept;
	~Texture() noexcept;

private:
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

public: // public function
	void release_srv_heap();

	[[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Resource> load(const std::filesystem::path& filePath);
	void create_resource_view();

public: // getter
	void set_name(const std::string& fileName);
	const std::string& name() const;
	const D3D12_GPU_DESCRIPTOR_HANDLE& get_gpu_handle() const;
	const u32& get_texture_width() const noexcept;
	const u32& get_texture_height() const noexcept;
	const std::optional<u32>& index() const;

private: // private function
	void create_texture_resource(const DirectX::TexMetadata& metadata);
	[[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Resource> upload_texture_data(const DirectX::ScratchImage& mipImages);

private: // private value
	std::optional<u32> heapIndex;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	u32 width = 0;
	u32 height = 0;

	std::string name_;

private: // static function
	static std::variant<HRESULT, DirectX::ScratchImage>  LoadTextureData(const std::filesystem::path& filePath);
};

