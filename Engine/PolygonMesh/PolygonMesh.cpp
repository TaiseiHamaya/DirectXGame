#include "PolygonMesh.h"

#include <sstream>
#include <fstream>
#include <unordered_map>
#include <algorithm>

#include "Engine/D3D12Kernel.h"

PolygonMesh::PolygonMesh() {
	meshVertexResource = nullptr;
	meshIndexResource = nullptr;
	meshData.vertices.clear();
	meshData.verticesMemorySize = 0;
	meshData.indexes.clear();
	meshData.indexesMemorySize = 0;
}

PolygonMesh::~PolygonMesh() {
	if (meshVertexResource) {
		meshVertexResource->Unmap(0, nullptr);
		meshVertexResource->Release();
	}
	if (meshIndexResource) {
		meshIndexResource->Unmap(0, nullptr);
		meshIndexResource->Release();
	}
}

MeshLoadResult PolygonMesh::load(const std::string& directoryPath, const std::string& fileName) {
	MeshLoadResult result;
	std::string mtlFileName;

	result = LoadObjectFile(meshData, directoryPath, fileName, mtlFileName);
	if (result != kMeshLoadResultSucecced) {
		return result;
	}
	result = LoadMtlFile(materialData, directoryPath, mtlFileName);
	if (result != kMeshLoadResultSucecced) {
		return result;
	}

	meshData.verticesMemorySize = static_cast<UINT>(sizeof(VertexData) * meshData.vertices.size());
	meshData.indexesMemorySize = static_cast<UINT>(sizeof(uint32_t) * meshData.indexes.size());
	meshVertexResource = D3D12Kernel::GetInstance()->create_buffer_resource(meshData.verticesMemorySize);
	if (!meshVertexResource) {
		return kMeshLoadResultFailedCreatingVertexBuffer;
	}
	meshIndexResource = D3D12Kernel::GetInstance()->create_buffer_resource(meshData.indexesMemorySize);
	if (!meshIndexResource) {
		return kMeshLoadResultFailedCreatingIndexBuffer;
	}

	vertexBufferView.BufferLocation = meshVertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = meshData.verticesMemorySize;
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	indexBufferView.BufferLocation = meshIndexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = meshData.indexesMemorySize;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	VertexData* vertexData = nullptr;
	meshVertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, meshData.vertices.data(), meshData.verticesMemorySize);

	uint32_t* indexData = nullptr;
	meshIndexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	std::memcpy(indexData, meshData.indexes.data(), meshData.indexesMemorySize);

	return kMeshLoadResultSucecced;
}

void PolygonMesh::set_buffer_command(ID3D12GraphicsCommandList* const commandList) {
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView); // VBV
	commandList->IASetIndexBuffer(&indexBufferView);
}

UINT PolygonMesh::get_index_size() {
	return static_cast<UINT>(meshData.indexes.size());
}

MeshLoadResult PolygonMesh::LoadObjectFile(MeshData& meshData, const std::string& directoryPath, const std::string& objFileName, std::string& mtlFileName) {
	std::vector<VertexData::Vector4> vertex;
	std::vector<Vector2> texcoord;
	std::vector<Vector3> normal;
	std::string line;
	std::unordered_map<std::string, uint32_t> reverseMeshVertices;

	std::ifstream file(directoryPath + "/" + objFileName);
	if (!file.is_open()) {
		return kMeshLoadResultFailedObjectFileOpen;
	}

	while (std::getline(file, line, '\n')) {
		std::string identifier;
		std::stringstream sstream(line);
		sstream >> identifier;

		if (identifier == "v") {
			vertex.push_back(VertexData::Vector4{});
			sstream >> vertex.back().position.x >> vertex.back().position.y >> vertex.back().position.z;
			vertex.back().position.x *= -1;
			vertex.back().w = 1.0f;
		}
		else if (identifier == "vt") {
			texcoord.push_back(Vector2{});
			sstream >> texcoord.back().x >> texcoord.back().y;
			texcoord.back().y = 1.0f - texcoord.back().y;
		}
		else if (identifier == "vn") {
			normal.push_back(Vector3{});
			sstream >> normal.back().x >> normal.back().y >> normal.back().z;
			normal.back().x *= -1;
		}
		else if (identifier == "f") {
			for (uint32_t faceIndex = 0; faceIndex < 3; ++faceIndex) {
				std::string element; // v/vt/vn
				sstream >> element;
				if (reverseMeshVertices.count(element)) {
					// あった場合
					// キーからvertexの要素番号を取り出す
					meshData.indexes.push_back(reverseMeshVertices.at(element));
				}
				else {
					// なかった場合
					std::array<uint32_t, 3> elementIndexes;
					std::istringstream elementSstream(element);
					std::string index;
					// データを取り出す
					for (int elementIndex = 0; elementIndex < elementIndexes.size(); ++elementIndex) {
						std::getline(elementSstream, index, '/');
						elementIndexes[elementIndex] = static_cast<uint32_t>(std::stoi(index) - 1);
					}
					// 頂点データを代入
					meshData.vertices.emplace_back(vertex[elementIndexes[0]], texcoord[elementIndexes[1]], normal[elementIndexes[2]]);
					// インデックスデータを追加
					meshData.indexes.emplace_back(static_cast<uint32_t>(meshData.vertices.size()) - 1);
					// インデックスの登録ログを取る
					reverseMeshVertices.emplace(element, static_cast<uint32_t>(meshData.vertices.size() - 1));
				}
			}
			std::iter_swap(meshData.indexes.rbegin(), meshData.indexes.rbegin() + 2); // indexをswapして左手座標系にする
		}
		else if (identifier == "mtllib") {
			sstream >> mtlFileName;
		}
	}
	return kMeshLoadResultSucecced;
}

MeshLoadResult PolygonMesh::LoadMtlFile(MaterialData& materialData, const std::string& directory, const std::string& mtlFileName) {
	std::string line;
	std::ifstream file(directory + "/" + mtlFileName);
	if (!file.is_open()) {
		return kMeshLoadResultFailedMtlFileOpen;
	}

	while (std::getline(file, line, '\n')) {
		std::string identifier;
		std::stringstream sstream(line);
		sstream >> identifier;

		if (identifier == "map_Kd") {
			std::string textureFileName;
			sstream >> textureFileName;
			materialData.textureFileName = directory + "/" + textureFileName;
		}
	}
	return kMeshLoadResultSucecced;
}
