#pragma once

#include <d3d12.h>

#include <memory>
#include <string>

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
	PolygonMesh();
	~PolygonMesh();

private:
	PolygonMesh(const PolygonMesh&) = delete;
	PolygonMesh& operator=(const PolygonMesh&) = delete;

public:
	MeshLoadResult load(const std::string& directoryPath, const std::string& fileName);
	void reset_data();
	void set_texture(std::weak_ptr<Texture>& texture_);
	const D3D12_VERTEX_BUFFER_VIEW* const get_p_vbv() const;
	const D3D12_INDEX_BUFFER_VIEW* const get_p_ibv() const;
	const UINT get_index_size() const;
	const std::weak_ptr<Texture>& get_texture() const;
	const Transform2D& get_default_uv() const;
	const std::string& get_texture_name() const;

private:
	MeshLoadResult load_object_file(const std::string& directoryPath, const std::string& objFileName);
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
