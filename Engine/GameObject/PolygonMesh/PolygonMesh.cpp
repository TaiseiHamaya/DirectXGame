#include "PolygonMesh.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "Engine/DirectX/DirectXResourceObject/IndexBuffer/IndexBuffer.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/TextureManager/TextureManager.h"
#include "Engine/DirectX/DirectXResourceObject/VertexBuffer/VertexBuffer.h"
#include "Engine/Utility/Utility.h"


PolygonMesh::PolygonMesh() noexcept = default;

PolygonMesh::~PolygonMesh() noexcept = default;

MeshLoadResult PolygonMesh::load(const std::string& directoryPath, const std::string& fileName) {
	MeshLoadResult result;
	Log("[PolygonMesh] Start load .obj file. file-\'"+ directoryPath + "/" + fileName +"\'\n");
	directory = directoryPath;
	objectName = fileName;

	result = load_obj_file(directoryPath, fileName);
	if (result != kMeshLoadResultSucecced) {
		return result;
	}
	result = load_mtl_file();
	if (result != kMeshLoadResultSucecced) {
		return result;
	}

	Log("[PolygonMesh] Success\n");
	return kMeshLoadResultSucecced;
}

void PolygonMesh::reset_data() noexcept(false) {
	texture = TextureManager::GetTexture(materialData.textureFileName);
}

void PolygonMesh::set_texture(const std::string& textureNamae) noexcept(false) {
	texture = TextureManager::GetTexture(textureNamae);
}

const D3D12_VERTEX_BUFFER_VIEW* const PolygonMesh::get_p_vbv() const noexcept {
	return meshData.vertices->get_p_vbv();
}

const D3D12_INDEX_BUFFER_VIEW* const PolygonMesh::get_p_ibv() const noexcept {
	return meshData.indexes->get_p_ibv();
}

const UINT PolygonMesh::index_size() const noexcept {
	return static_cast<const UINT>(meshData.indexes->index_size());
}

const std::weak_ptr<Texture>& PolygonMesh::get_texture() const noexcept {
	return texture;
}

const Transform2D& PolygonMesh::get_default_uv() const noexcept {
	return materialData.defaultUV;
}

const std::string& PolygonMesh::get_texture_name() const noexcept {
	return materialData.textureFileName;
}

MeshLoadResult PolygonMesh::load_obj_file(const std::string& directoryPath, const std::string& objFileName) {
	std::vector<VertexData::Vector4> vertex; // objファイルの頂点情報
	std::vector<Vector2> texcoord; // objファイルのtexcoord情報
	std::vector<Vector3> normal; // objファイルのnormal情報
	std::string line; // 1行を保存するよう

	std::vector<VertexData> vertices; // vertexBuffer用
	std::vector<std::uint32_t> indexes; // indexBuffer用

	std::unordered_map<std::string, uint32_t> reverseMeshVertices; // 登録済み頂点情報とそのindex情報

	// ファイルを開く
	std::ifstream file(directoryPath + "/" + objFileName);
	if (!file.is_open()) {
		Log("[PolygonMesh] File \'" + directoryPath + "/" + objFileName + "\' is not found.\n");
		return kMeshLoadResultFailedObjectFileOpen;
	}

	// 行取得
	while (std::getline(file, line, '\n')) {
		std::string identifier;
		std::stringstream sstream(line);
		// 識別子の取得
		sstream >> identifier;

		// vertex
		if (identifier == "v") {
			vertex.push_back(VertexData::Vector4{});
			sstream >> vertex.back().position.x >> vertex.back().position.y >> vertex.back().position.z;
			// 左手座標系から右手座標系へ
			vertex.back().position.x *= -1;
			vertex.back().w = 1.0f;
		}
		// texCoord
		else if (identifier == "vt") {
			texcoord.push_back(Vector2{});
			sstream >> texcoord.back().x >> texcoord.back().y;
			// blenderとDirectXだとy軸の方向が逆
			texcoord.back().y = 1.0f - texcoord.back().y;
		}
		// normal
		else if (identifier == "vn") {
			normal.push_back(Vector3{});
			sstream >> normal.back().x >> normal.back().y >> normal.back().z;
			// 左手座標系から右手座標系へ
			normal.back().x *= -1;
		}
		// 面情報
		else if (identifier == "f") {
			for (uint32_t faceIndex = 0; faceIndex < 3; ++faceIndex) {
				std::string element; 

				sstream >> element; // v/vt/vnの取得

				// すでに登録されているか
				if (reverseMeshVertices.contains(element)) {
					// あった場合
					// キーからvertexの要素番号(Index番号)を取り出す
					indexes.push_back(reverseMeshVertices.at(element));
				}
				else {
					std::array<uint32_t, 3> elementIndexes;
					std::istringstream elementSstream(element);
					std::string index;
					// データを取り出す
					for (int elementIndex = 0; elementIndex < elementIndexes.size(); ++elementIndex) {
						std::getline(elementSstream, index, '/');
						if (index.empty()) {
							elementIndexes[elementIndex] = 0;
						}
						else {
							elementIndexes[elementIndex] = static_cast<uint32_t>(std::stoi(index) - 1);
						}
					}
					if (vertex.size() <= elementIndexes[0]) {
						elementIndexes[0] = 0;
						if (vertex.empty()) {
							vertex.push_back(VertexData::Vector4{ CVector3::ZERO,0 });
						}
					}
					if (texcoord.size() <= elementIndexes[1]) {
						elementIndexes[1] = 0;
						if (texcoord.empty()) {
							texcoord.push_back(CVector2::ZERO);
						}
					}
					if (normal.size() <= elementIndexes[2]) {
						elementIndexes[2] = 0;
						if (normal.empty()) {
							normal.push_back(CVector3::BASIS_X);
						}
					}
					// 頂点データを追加
					vertices.emplace_back(vertex[elementIndexes[0]], texcoord[elementIndexes[1]], normal[elementIndexes[2]]);
					// インデックスデータを追加
					indexes.emplace_back(static_cast<uint32_t>(vertices.size()) - 1);
					// インデックスの登録ログを取る
					reverseMeshVertices.emplace(element, static_cast<uint32_t>(vertices.size() - 1));
				}
			}
			// indexをswapして左手座標系にする
			std::iter_swap(indexes.rbegin(), indexes.rbegin() + 2);
		}
		// mtlファイル名の取得
		else if (identifier == "mtllib") {
			sstream >> materialData.mtlFileName;
		}
	}

	// リソースの作成とコピー
	meshData.vertices = std::make_unique<VertexBuffer>(vertices);
	meshData.indexes = std::make_unique<IndexBuffer>(indexes);

	return kMeshLoadResultSucecced;
}

MeshLoadResult PolygonMesh::load_mtl_file() {
	std::string line;
	std::ifstream file;

	// mtlファイルを開く
	file.open(directory + "/" + materialData.mtlFileName);
	if (!file.is_open()) {
		Log("[PolygonMesh] File \'" + directory + "/" + materialData.mtlFileName + "\' is not found.\n");
		return kMeshLoadResultFailedMtlFileOpen;
	}

	// 1行ずつ取得
	while (std::getline(file, line, '\n')) {
		std::string identifier;
		std::stringstream sstream(line);

		// 識別子取得
		std::getline(sstream, identifier, ' ');

		if (identifier == "map_Kd") {
			std::string option;
			while (std::getline(sstream, option, ' ')) {
				// オプションではない場合
				if (option[0] != '-') {
					// optionデータがそのままテクスチャファイル名になるので転送
					materialData.textureFileName = std::move(option);
					// テクスチャファイルのロード登録
					TextureManager::RegisterLoadQue(directory, materialData.textureFileName);
				}
				// -sオプション(スケール)
				else if (option[1] == 's') {
					Vector2 scale;
					std::string temp;
					sstream >> scale.x >> scale.y >> temp;
					materialData.defaultUV.set_scale(scale);
					std::getline(sstream, temp, ' ');
				}
				// -oオプション(原点)
				else if (option[1] == 'o') {
					Vector2 position;
					std::string temp;
					sstream >> position.x >> position.y >> temp;
					materialData.defaultUV.set_translate(position);
					std::getline(sstream, temp, ' ');
				}
			}
		}
	}
	return kMeshLoadResultSucecced;
}
