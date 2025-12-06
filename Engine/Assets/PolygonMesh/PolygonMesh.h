#pragma once

#include <d3d12.h>

#include <memory>
#include <string>
#include <unordered_map>

#include <Library/Math/Transform2D.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/VertexBuffer/VertexBuffer.h"

namespace szg {

class IndexBuffer;

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
	PolygonMesh(std::vector<MeshData>& meshData_, std::unordered_map<std::string, MeshMaterialData>& materialData_) noexcept;
	~PolygonMesh() noexcept;

	SZG_CLASS_MOVE_ONLY(PolygonMesh)

public:
	/// <summary>
	/// VertexBufferViewを取得
	/// </summary>
	/// <returns>VertexBufferView Pointer</returns>
	const D3D12_VERTEX_BUFFER_VIEW& get_vbv(u32 index) const;

	/// <summary>
	/// IndexBufferViewを取得
	/// </summary>
	/// <returns>IndexBufferView Pointer</returns>
	const D3D12_INDEX_BUFFER_VIEW* const get_p_ibv(u32 index) const;

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
	const UINT index_size(u32 index) const;

	const MeshData* mesh_data(u32 index) const;

	const MeshMaterialData* material_data(u32 index) const;

private:
	bool has_material(u32 index) const;

private:
	std::vector<MeshData> meshData;
	std::unordered_map<std::string, MeshMaterialData> materialData;
};

}; // szg
