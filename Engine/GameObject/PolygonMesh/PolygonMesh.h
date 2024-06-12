#pragma once

#include <d3d12.h>

#include <memory>
#include <string>
#include <unordered_map>

#include "Engine/GameObject/Transform2D/Transform2D.h"

class VertexBuffer;
class IndexBuffer;
class Texture;

enum MeshLoadResult {
	kMeshLoadResultSucecced,
	kMeshLoadResultFailedObjectFileOpen,
	kMeshLoadResultFailedMtlFileOpen,
	kMeshLoadResultObjectFileLogicError,
	kMeshLoadResultMtlFileLogicError,
	kMeshLoadResultFailedCreatingVertexBuffer,
	kMeshLoadResultFailedCreatingIndexBuffer,
};

class PolygonMesh final {
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
	/// <param name="directoryPath">ファイルディレクトリ</param>
	/// <param name="objectName">ファイル名</param>
	/// <returns>成功値</returns>
	MeshLoadResult load(const std::string& directoryPath, const std::string& objectName);
	
	/// <summary>
	/// テクスチャやUVデータをデフォルトに設定
	/// </summary>
	void reset_data() noexcept(false);

	/// <summary>
	/// 使用するテクスチャを引数のテクスチャ名のものにする(見つからなかった場合はエラーテクスチャを使用)
	/// </summary>
	/// <param name="textureNamae">テクスチャ名</param>
	void set_texture(const std::string& textureNamae) noexcept(false);

	/// <summary>
	/// VertexBufferViewを取得
	/// </summary>
	/// <returns>VertexBufferView Pointer</returns>
	const D3D12_VERTEX_BUFFER_VIEW* const get_p_vbv() const noexcept;

	/// <summary>
	/// IndexBufferViewを取得
	/// </summary>
	/// <returns>IndexBufferView Pointer</returns>
	const D3D12_INDEX_BUFFER_VIEW* const get_p_ibv() const noexcept;

	/// <summary>
	/// IndexBufferのサイズ
	/// </summary>
	/// <returns>UINT型で返す</returns>
	const UINT index_size() const noexcept;

	/// <summary>
	/// デフォルトのテクスチャのweak_ptr
	/// </summary>
	/// <returns></returns>
	const std::weak_ptr<Texture>& get_texture() const noexcept;

	/// <summary>
	/// デフォルトのUV Transformを取得
	/// </summary>
	/// <returns>Transform2D</returns>
	const Transform2D& get_default_uv() const noexcept;

	/// <summary>
	/// デフォルトのテクスチャ名を取得
	/// </summary>
	/// <returns>テクスチャ名[std::string]</returns>
	const std::string& get_texture_name() const noexcept;

private:
	MeshLoadResult load_obj_file(const std::string& directoryPath, const std::string& objFileName);
	MeshLoadResult load_mtl_file();

private:
	std::string directory;
	std::string objectName;
	struct MeshData {
		std::unique_ptr<VertexBuffer> vertices;
		std::unique_ptr<IndexBuffer> indexes;
	} meshData;
	struct MaterialData {
		std::string mtlFileName;
		std::string textureFileName;
		Transform2D defaultUV;
	} materialData;
	std::weak_ptr<Texture> texture;
};
