#pragma once

#include <d3d12.h>
#include <memory>
#include <string>

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

class PolygonMesh {
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
		std::string textureFileName;
	} materialData;
	std::weak_ptr<Texture> texture;
};
