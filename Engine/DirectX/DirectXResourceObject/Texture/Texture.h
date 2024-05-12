#pragma once

#include <d3d12.h>
#include <DirectXTex.h>
#include <numeric>
#include <string>

class Texture {
public: // constructor
	Texture();
	~Texture();

public: // operator

public: // public function
	void set_command(ID3D12GraphicsCommandList* const commandList);
	void load_texture(const std::string& FilePath, uint32_t heapIndex);
	const D3D12_GPU_DESCRIPTOR_HANDLE& get_gpu_handle() const;

private: // private function
	void create_texture_resource(const DirectX::TexMetadata& metadata);
	[[nodiscard]] ID3D12Resource* upload_texture_data(const DirectX::ScratchImage& mipImages);

private: // private value
	uint32_t heapIndex;
	ID3D12Resource* textureResource;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;

private: // static function
	static DirectX::ScratchImage LoadTextureData(const std::string& filePath);

public: // getter
};

