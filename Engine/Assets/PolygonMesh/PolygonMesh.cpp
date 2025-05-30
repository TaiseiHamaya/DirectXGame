#include "PolygonMesh.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#include <Library/Utility/Tools/SmartPointer.h>

#include "Engine/Application/Output.h"
#include "Engine/Assets/Texture/TextureLibrary.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/IndexBuffer/IndexBuffer.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/VertexBuffer/VertexBuffer.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

PolygonMesh::PolygonMesh() noexcept = default;

PolygonMesh::~PolygonMesh() noexcept = default;

bool PolygonMesh::load(const std::filesystem::path& filePath) {
	bool result = false;
	Information("Start load file-\'{}\'", filePath.string());
	//directory = filePath.parent_path();

	std::string stem = filePath.extension().string();
	if (stem == ".obj") {
		result = load_obj_file(filePath);
	}
	else {
		result = load_gltf_file(filePath);
	}

	if (!result) {
		return result;
	}

	std::wstring meshFileName = filePath.filename().wstring();
	for (i32 index = 0; auto & meshData : meshData) {
		meshData.vertices->get_resource()->SetName(std::format(L"VertexBuffer-{}({})", index, meshFileName).c_str());
		meshData.indexes->get_resource()->SetName(std::format(L"IndexBuffer-{}({})", index, meshFileName).c_str());
		++index;
	}

	Information("Succeeded.");
	return true;
}

const D3D12_VERTEX_BUFFER_VIEW& PolygonMesh::get_vbv(u32 index) const {
	return meshData[index].vertices->get_vbv();
}

const D3D12_INDEX_BUFFER_VIEW* const PolygonMesh::get_p_ibv(u32 index) const {
	return meshData[index].indexes->get_p_ibv();
}

const size_t PolygonMesh::material_count() const {
	return meshData.size();
}

const UINT PolygonMesh::index_size(u32 index) const {
	return static_cast<const UINT>(meshData[index].indexes->index_size());
}

const PolygonMesh::MeshData* PolygonMesh::mesh_data(u32 index) const {
	if (index < material_count()) {
		return &meshData[index];
	}
	return nullptr;
}

const PolygonMesh::MeshMaterialData* PolygonMesh::material_data(u32 index) const {
	if (index >= material_count()) {
		return nullptr;
	}
	if (has_material(index)) {
		return &materialData.at(meshData[index].materialName);
	}
	return nullptr;
}

bool PolygonMesh::has_material(u32 index) const {
	return materialData.contains(meshData[index].materialName);
}

bool PolygonMesh::load_obj_file(const std::filesystem::path& filePath) {
	std::vector<Vector3> vertex; // objファイルの頂点情報
	std::vector<Vector2> texcoord; // objファイルのtexcoord情報
	std::vector<Vector3> normal; // objファイルのnormal情報
	std::string line; // 1行を保存するよう

	std::vector<VertexDataBuffer> vertices; // vertexBuffer用
	std::vector<u32> indexes; // indexBuffer用

	std::string mtlFileName;

	std::unordered_map<std::string, u32> reverseMeshVertices; // 登録済み頂点情報とそのindex情報

	std::vector<MeshData>::iterator current = meshData.begin();

	// ファイルを開く
	std::ifstream file(filePath);
	if (!file.is_open()) {
		Error("File \'{}\' is not found.", filePath.string());
		return false;
	}

	// 行取得
	while (std::getline(file, line, '\n')) {
		std::string identifier;
		std::stringstream sstream(line);
		// 識別子の取得
		std::getline(sstream, identifier, ' ');

		// vertex
		if (identifier == "v") {
			Vector3& write = vertex.emplace_back();
			sstream >> write.x >> write.y >> write.z;
			// 左手座標系から右手座標系へ
			write.x *= -1;
		}
		// texCoord
		else if (identifier == "vt") {
			auto& newTexcoord = texcoord.emplace_back();
			sstream >> newTexcoord.x >> newTexcoord.y;
			// blenderとDirectXだとy軸の方向が逆
			newTexcoord.y = 1.0f - newTexcoord.y;
		}
		// normal
		else if (identifier == "vn") {
			auto& newNormal = normal.emplace_back();
			sstream >> newNormal.x >> newNormal.y >> newNormal.z;
			// 左手座標系から右手座標系へ
			newNormal.x *= -1;
		}
		// 面情報
		else if (identifier == "f") {
			for (u32 faceIndex = 0; faceIndex < 3; ++faceIndex) {
				std::string element;

				std::getline(sstream, element, ' ');

				// すでに登録されているか
				if (reverseMeshVertices.contains(element)) {
					// あった場合
					// キーからvertexの要素番号(Index番号)を取り出す
					indexes.emplace_back(reverseMeshVertices.at(element));
				}
				else {
					std::array<u32, 3> elementIndexes;
					std::istringstream elementSstream(element);
					std::string index;
					// データを取り出す
					for (i32 elementIndex = 0; elementIndex < elementIndexes.size(); ++elementIndex) {
						std::getline(elementSstream, index, '/');
						if (index.empty()) {
							elementIndexes[elementIndex] = 0;
						}
						else {
							elementIndexes[elementIndex] = static_cast<u32>(std::stoi(index) - 1);
						}
					}
					if (vertex.size() <= elementIndexes[0]) {
						elementIndexes[0] = 0;
						if (vertex.empty()) {
							vertex.emplace_back(CVector3::ZERO);
						}
					}
					if (texcoord.size() <= elementIndexes[1]) {
						elementIndexes[1] = 0;
						if (texcoord.empty()) {
							texcoord.emplace_back(CVector2::ZERO);
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
					indexes.emplace_back(static_cast<u32>(vertices.size()) - 1);
					// インデックスの登録ログを取る
					reverseMeshVertices.emplace(element, static_cast<u32>(vertices.size() - 1));
				}
			}
			// indexをswapして左手座標系にする
			std::iter_swap(indexes.rbegin(), indexes.rbegin() + 2);
		}
		else if (identifier == "o") {
			if (!vertices.empty() && !indexes.empty()) {
				// リソースの作成とコピー
				current->vertices = eps::CreateUnique<Object3DVertexBuffer>(std::move(vertices));
				current->indexes = eps::CreateUnique<IndexBuffer>(indexes);
			}
			meshData.emplace_back();
			current = meshData.end() - 1;
			vertices.clear();
			indexes.clear();
			reverseMeshVertices.clear();
		}
		else if (identifier == "usemtl") {
			if (current == meshData.end()) {
				meshData.emplace_back();
				current = meshData.end() - 1;
			}
			std::getline(sstream, current->materialName, ' ');
		}
		// mtlファイル名の取得
		else if (identifier == "mtllib") {
			std::getline(sstream, mtlFileName, ' ');
		}
	}

	// 最後にもう1度リソースの作成とコピー
	current->vertices = std::make_unique<Object3DVertexBuffer>(vertices);
	current->indexes = std::make_unique<IndexBuffer>(indexes);

	bool result;
	std::filesystem::path directory{ filePath.parent_path() };
	result = load_mtl_file(directory / mtlFileName);
	return result;
}

bool PolygonMesh::load_mtl_file(const std::filesystem::path& mtlFilePath) {
	std::string line;
	std::ifstream file;

	// mtlファイルを開く
	file.open(mtlFilePath);
	if (!file.is_open()) {
		Warning("File \'{}\' is not found.", mtlFilePath.string());
		return false;
	}

	MeshMaterialData* current = nullptr;

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
					current->textureFileName = std::move(option);
					// テクスチャファイルのロード登録
					std::filesystem::path directory{ mtlFilePath.parent_path() };
					TextureLibrary::RegisterLoadQue(directory / current->textureFileName);
				}
				// -sオプション(スケール)
				else if (option[1] == 's') {
					Vector2 scale;
					std::string temp;
					sstream >> scale.x >> scale.y >> temp;
					current->defaultUV.set_scale(scale);
					std::getline(sstream, temp, ' ');
				}
				// -oオプション(原点)
				else if (option[1] == 'o') {
					Vector2 position;
					std::string temp;
					sstream >> position.x >> position.y >> temp;
					current->defaultUV.set_translate(position);
					std::getline(sstream, temp, ' ');
				}
			}
		}
		else if (identifier == "newmtl") {
			std::string option;
			std::getline(sstream, option, '\n');
			current = &materialData[option];
		}
	}
	return true;
}

bool PolygonMesh::load_gltf_file(const std::filesystem::path& filePath) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath.string().c_str(),
		aiProcess_FlipUVs |
		aiProcess_FlipWindingOrder |
		aiProcess_LimitBoneWeights
	);
	if (importer.GetException() || !scene) {
		Error("Failed to load mesh file. File-\'{}\' Message-\'{}\'", filePath.string(), importer.GetErrorString());
		return false;
	}
	if (!scene->HasMeshes()) {
		Error("Can't find mesh. File-\'{}\'", filePath.string());
		return false;
	}

	// あとで逆参照する用
	std::unordered_map<u32, std::string> materialNameFromIndex;

	// Material解析
	for (u32 materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			MeshMaterialData newMaterial;
			// テクスチャ名の書き込み
			newMaterial.textureFileName = textureFilePath.C_Str();
			std::filesystem::path directory{ filePath.parent_path() };
			TextureLibrary::RegisterLoadQue(directory / newMaterial.textureFileName);

			std::string materialName;
			aiString aiMaterialName;
			// マテリアル名の取得
			if (material->Get(AI_MATKEY_NAME, aiMaterialName) == AI_SUCCESS) {
				materialName = aiMaterialName.C_Str();
			}
			materialData.emplace(materialName, std::move(newMaterial));
			materialNameFromIndex.emplace(materialIndex, materialName);
		}
	}

	// メッシュ解析
	for (u32 meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {

		std::vector<VertexDataBuffer> vertices; // vertexBuffer用
		std::vector<u32> indexes; // indexBuffer用
		aiMesh* mesh = scene->mMeshes[meshIndex];
		// normalが存在しない、texcoordが存在しない場合はメッシュとして使用しない
		if (!mesh->HasNormals() || !mesh->HasTextureCoords(0)) {
			Error("This mesh don't have normal or texcoord. File-\'{}\'", filePath.string());
			continue;
		}
		vertices.reserve(mesh->mNumVertices);
		// 頂点取得
		for (u32 vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
			VertexDataBuffer& vertex = vertices.emplace_back();
			vertex.position = { -position.x,position.y, position.z };
			vertex.normal = { -normal.x, normal.y, normal.z };
			vertex.texcoord = { texcoord.x, texcoord.y };
		}
		indexes.reserve(static_cast<size_t>(mesh->mNumFaces) * 3);
		// Index取得
		for (u32 faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			if (face.mNumIndices != 3) {
				continue;
			}

			for (u32 element = 0; element < face.mNumIndices; ++element) {
				u32 vertexIndex = face.mIndices[element];
				indexes.emplace_back(vertexIndex);
			}
		}
		// 作成
		PolygonMesh::MeshData& newMesh = meshData.emplace_back();
		// 転送
		newMesh.vertices = std::make_unique<Object3DVertexBuffer>(vertices);
		newMesh.indexes = std::make_unique<IndexBuffer>(indexes);
		// メッシュ名の取得
		newMesh.meshName = mesh->mName.C_Str();
		// Material名の取得
		if (materialNameFromIndex.contains(mesh->mMaterialIndex)) {
			newMesh.materialName = materialNameFromIndex.at(mesh->mMaterialIndex);
		}
	}

	return true;
}
