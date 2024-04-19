#pragma once

#include "Engine/Math/Vector2d.h"
#include "Engine/Math/Vector3d.h"
#include "Engine/Math/Matrix.h"
#include "Engine/Math/Color.h"
#include <d3d12.h>

#include <vector>
#include <string>

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

public:
	struct VertexData {
		struct Vector4 {
			Vector3 position;
			float w;
		} vertex;
		Vector2 texcoord;
		Vector3 normal;
	};

public:
	MeshLoadResult load(const std::string& directoryPath, const std::string& fileName);
	void set_buffer_command(ID3D12GraphicsCommandList* const commandList);
	UINT get_index_size();

private:
	struct MeshData {
		std::vector<VertexData> vertices;
		std::vector<uint32_t> indexes;
		UINT verticesMemorySize;
		UINT indexesMemorySize;
	} meshData;
	struct MaterialData {
		std::string textureFileName;
	} materialData;
	ID3D12Resource* meshVertexResource;
	ID3D12Resource* meshIndexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

private:
	static MeshLoadResult LoadObjectFile(MeshData& meshData, const std::string& directoryPath, const std::string& objFileName, std::string& mtlFileName);
	static MeshLoadResult LoadMtlFile(MaterialData& materialData, const std::string& directory, const std::string& mtlFileName);
};

