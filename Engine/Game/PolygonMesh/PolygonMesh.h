#pragma once

#include <d3d12.h>

#include <memory>
#include <string>
#include <unordered_map>

#include "Engine/Game/Transform2D/Transform2D.h"

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
	/// VertexBufferViewを取得
	/// </summary>
	/// <returns>VertexBufferView Pointer</returns>
	const D3D12_VERTEX_BUFFER_VIEW* const get_p_vbv(std::uint32_t index) const;

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
	/// mtlファイルが存在しているか
	/// </summary>
	/// <param name="index">指定するインデックス</param>
	/// <returns></returns>
	bool has_mtl(std::uint32_t index) const;

	/// <summary>
	/// IndexBufferのサイズ
	/// </summary>
	/// <param name="index">指定するインデックス</param>
	/// <returns>UINT型で返す</returns>
	const UINT index_size(std::uint32_t index) const;

	/// <summary>
	/// デフォルトのテクスチャのweak_ptr
	/// </summary>
	/// <param name="index">指定するインデックス</param>
	/// <returns></returns>
	const std::string& texture_name(std::uint32_t index) const;

	/// <summary>
	/// デフォルトのUV Transformを取得
	/// </summary>
	/// <param name="index">指定するインデックス</param>
	/// <returns>Transform2D</returns>
	const Transform2D& default_uv(std::uint32_t index) const;

	/// <summary>
	/// デフォルトのテクスチャ名を取得
	/// </summary>
	/// <param name="index">指定するインデックス</param>
	/// <returns>テクスチャ名[std::string]</returns>
	const std::string& model_name(std::uint32_t index) const;

private:
	MeshLoadResult load_obj_file(const std::string& directoryPath, const std::string& objFileName);
	MeshLoadResult load_mtl_file();

private:
	std::string directory;
	std::string objectName;

	std::string mtlFileName;

	struct MeshData {
		std::string objectName;
		std::unique_ptr<VertexBuffer> vertices;
		std::unique_ptr<IndexBuffer> indexes;
		std::string usemtl;
	};
	std::vector<MeshData> meshDatas;
	
	struct MaterialData {
		std::string textureFileName;
		Transform2D defaultUV;
	};
	std::unordered_map<std::string, MaterialData> materialDatas;
};
