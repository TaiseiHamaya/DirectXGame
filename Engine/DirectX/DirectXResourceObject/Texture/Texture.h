#pragma once
#include <cstdint>
#include <optional>
#include <string>

#include <d3d12.h>
#include <DirectXTex.h>

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"

class Texture : public DirectXResourceObject {
public: // constructor
	Texture();
	~Texture();

private:
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

public: // public function
	[[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Resource> load_texture(const std::string& filePath);
	void create_resource_view();
	void set_command() const;
	const std::uint32_t& get_texture_width();
	const std::uint32_t& get_texture_height();

private: // private function
	void create_texture_resource(const DirectX::TexMetadata& metadata);
	[[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Resource> upload_texture_data(const DirectX::ScratchImage& mipImages);
	
private: // private value
	std::optional<std::uint32_t> heapIndex;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	std::uint32_t width;
	std::uint32_t height;

private: // static function
	static DirectX::ScratchImage LoadTextureData(const std::string& filePath);
	
public: // getter
};

