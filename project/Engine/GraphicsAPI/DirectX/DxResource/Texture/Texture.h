#pragma once

#include <cstdint>
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
	void set_name(const std::string& fileName);

	const D3D12_GPU_DESCRIPTOR_HANDLE& get_gpu_handle() const;
	const std::uint32_t& get_texture_width() const noexcept;
	const std::uint32_t& get_texture_height() const noexcept;

private: // private function
	void create_texture_resource(const DirectX::TexMetadata& metadata);
	[[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Resource> upload_texture_data(const DirectX::ScratchImage& mipImages);

private: // private value
	std::optional<std::uint32_t> heapIndex;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	std::uint32_t width = 0;
	std::uint32_t height = 0;

private: // static function
	static std::variant<HRESULT, DirectX::ScratchImage>  LoadTextureData(const std::filesystem::path& filePath);

public: // getter
};

