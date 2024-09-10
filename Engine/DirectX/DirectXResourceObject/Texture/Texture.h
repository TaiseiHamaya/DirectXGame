#pragma once
#include <cstdint>
#include <optional>
#include <string>

#include <d3d12.h>
#include <DirectXTex.h>

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"

class Texture final : public DirectXResourceObject {
public: // constructor
	Texture() noexcept;
	~Texture() noexcept;

private:
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

public: // public function
	void set_command() const;

	void release_srv_heap();

	[[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Resource> load_texture(const std::string& filePath);
	void create_resource_view();

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
	static std::optional<DirectX::ScratchImage> LoadTextureData(const std::string& filePath);
	
public: // getter
};

