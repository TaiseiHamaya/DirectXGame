#include "PolygonMesh.h"

#include <sstream>
#include <fstream>
#include <unordered_map>
#include <algorithm>

#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXResourceObject/VertexBuffer/VertexBuffer.h"
#include "Engine/DirectX/DirectXResourceObject/IndexBuffer/IndexBuffer.h"


PolygonMesh::PolygonMesh() = default;

PolygonMesh::~PolygonMesh() = default;

MeshLoadResult PolygonMesh::load(const std::string& directoryPath, const std::string& fileName) {
	MeshLoadResult result;
	std::string mtlFileName;

	result = load_object_file(directoryPath, fileName);
	if (result != kMeshLoadResultSucecced) {
		return result;
	}
	result = load_mtl_file(directoryPath, mtlFileName);
	if (result != kMeshLoadResultSucecced) {
		return result;
	}

	return kMeshLoadResultSucecced;
}

const D3D12_VERTEX_BUFFER_VIEW* const PolygonMesh::get_p_vbv() const {
	return meshData.vertices->get_p_vbv();
}

const D3D12_INDEX_BUFFER_VIEW* const PolygonMesh::get_p_ibv() const {
	return meshData.indexes->get_p_ibv();
}

const UINT PolygonMesh::get_index_size() const {
	return static_cast<const UINT>(meshData.indexes->get_index_size());
}

//const std::weak_ptr<Texture>& PolygonMesh::get_texture() const {
//	return texture;
//}

MeshLoadResult PolygonMesh::load_object_file(const std::string& directoryPath, const std::string& objFileName) {
	std::vector<VertexData::Vector4> vertex;
	std::vector<Vector2> texcoord;
	std::vector<Vector3> normal;
	std::string line;
	std::unordered_map<std::string, uint32_t> reverseMeshVertices;

	std::vector<VertexData> vertices;
	std::vector<std::uint32_t> indexes;

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
					indexes.push_back(reverseMeshVertices.at(element));
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
					vertices.emplace_back(vertex[elementIndexes[0]], texcoord[elementIndexes[1]], normal[elementIndexes[2]]);
					// インデックスデータを追加
					indexes.emplace_back(static_cast<uint32_t>(vertices.size()) - 1);
					// インデックスの登録ログを取る
					reverseMeshVertices.emplace(element, static_cast<uint32_t>(vertices.size() - 1));
				}
			}
			std::iter_swap(indexes.rbegin(), indexes.rbegin() + 2); // indexをswapして左手座標系にする
		}
		else if (identifier == "mtllib") {
			sstream >> materialData.textureFileName;
		}
	}

	// リソースの作成とコピー
	meshData.vertices = std::make_unique<VertexBuffer>(vertices);
	meshData.indexes = std::make_unique<IndexBuffer>(indexes);

	return kMeshLoadResultSucecced;
}

MeshLoadResult PolygonMesh::load_mtl_file(const std::string& directory, const std::string& mtlFileName) {
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