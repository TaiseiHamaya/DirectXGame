#pragma once

#include <d3d12.h>

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include "Engine/GraphicsAPI/DirectX/DxResource/VertexBuffer/VertexBuffer.h"

#include <Library/Math/Transform2D.h>

class IndexBuffer;
class Texture;

class PolygonMesh final {
public:
	struct MeshData {
		std::unique_ptr<Object3DVertexBuffer> vertices;
		std::unique_ptr<IndexBuffer> indexes;
		std::string meshName;
		std::string materialName;
	};

	struct MeshMaterialData {
		std::string textureFileName;
		Transform2D defaultUV;
	};

public:
	PolygonMesh() noexcept;
	~PolygonMesh() noexcept;

private:
	PolygonMesh(const PolygonMesh&) = delete;
	PolygonMesh& operator=(const PolygonMesh&) = delete;

public:
	/// <summary>
	/// ロード関数
	/// </summary>
	/// <param name="filePath">ファイルディレクトリ</param>
	/// <returns>成功値</returns>
	bool load(const std::filesystem::path& filePath);

	/// <summary>
	/// VertexBufferViewを取得
	/// </summary>
	/// <returns>VertexBufferView Pointer</returns>
	const D3D12_VERTEX_BUFFER_VIEW& get_vbv(std::uint32_t index) const;

	/// <summary>
	/// IndexBufferViewを取得
	/// </summary>
	/// <returns>IndexBufferView Pointer</returns>
	const D3D12_INDEX_BUFFER_VIEW* const get_p_ibv(std::uint32_t index) const;

	/// <summary>
	/// 使用するマテリアル数の取得
	/// </summary>
	/// <returns></returns>
	const size_t material_count() const;

	/// <summary>
	/// IndexBufferのサイズ
	/// </summary>
	/// <param name="index">指定するインデックス</param>
	/// <returns>UINT型で返す</returns>
	const UINT index_size(std::uint32_t index) const;

	const MeshData* mesh_data(std::uint32_t index) const;

	const MeshMaterialData* material_data(std::uint32_t index) const;

private:
	bool has_material(std::uint32_t index) const;

private:
	bool load_obj_file(const std::filesystem::path& filePath);
	bool load_mtl_file(const std::filesystem::path& mtlFileName);

	bool load_gltf_file(const std::filesystem::path& mtlFilePath);

private:
	std::vector<MeshData> meshData;
	std::unordered_map<std::string, MeshMaterialData> materialData;
};
