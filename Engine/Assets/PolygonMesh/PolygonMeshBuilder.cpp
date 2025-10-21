#include "PolygonMeshBuilder.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#include "./PolygonMesh.h"
#include "./PolygonMeshLibrary.h"

#include <Library/Utility/Tools/SmartPointer.h>

#include "Engine/Application/Logger.h"
#include "Engine/Assets/Texture/TextureLibrary.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/IndexBuffer/IndexBuffer.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/VertexBuffer/VertexBuffer.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

/*
Tips
ObjファイルはMTLのUVTransformを受け取りたいので自作の別関数を使用
※AssimpはUVTransformに対して非対応
*/

// 前方宣言
static bool LoadObj(const std::filesystem::path& filePath, std::vector<PolygonMesh::MeshData>& meshData, std::unordered_map<std::string, PolygonMesh::MeshMaterialData>& materialData);
static bool LoadMtl(const std::filesystem::path& filePath, std::unordered_map<std::string, PolygonMesh::MeshMaterialData>& materialData);
static bool LoadMeshAssimp(const std::filesystem::path& filePath, std::vector<PolygonMesh::MeshData>& meshData, std::unordered_map<std::string, PolygonMesh::MeshMaterialData>& materialData);

PolygonMeshBuilder::PolygonMeshBuilder(const std::filesystem::path& filePath_) {
	filePath = BaseAssetBuilder::ResolveFilePath(filePath_, "Models");
}

bool PolygonMeshBuilder::run() {
	bool result = false;
	szgInformation(L"Start load file-\'{}\'", filePath.native());

	std::vector<PolygonMesh::MeshData> meshData;
	std::unordered_map<std::string, PolygonMesh::MeshMaterialData> materialData;
	std::string stem = filePath.extension().string();
	if (stem == ".obj") {
		result = LoadObj(filePath, meshData, materialData);
	}
	else {
		result = LoadMeshAssimp(filePath, meshData, materialData);
	}

	if (!result) {
		return result;
	}

	std::wstring meshFileName = filePath.filename().native();
	for (i32 index = 0; auto& mesh : meshData) {
		mesh.vertices->get_resource()->SetName(std::format(L"VertexBuffer-{}({})", index, meshFileName).c_str());
		mesh.indexes->get_resource()->SetName(std::format(L"IndexBuffer-{}({})", index, meshFileName).c_str());
		++index;
	}

	meshResult = eps::CreateShared<PolygonMesh>(meshData, materialData);

	szgInformation("Succeeded.");
	return true;
}

void PolygonMeshBuilder::postprocess() {
	// Do nothing
}

void PolygonMeshBuilder::transfer() {
	PolygonMeshLibrary::Transfer(filePath.filename().string(), meshResult);
}

bool LoadObj(const std::filesystem::path& filePath, std::vector<PolygonMesh::MeshData>& meshData, std::unordered_map<std::string, PolygonMesh::MeshMaterialData>& materialData) {
	std::vector<Vector3> vertex; // objファイルの頂点情報
	std::vector<Vector2> texcoord; // objファイルのtexcoord情報
	std::vector<Vector3> normal; // objファイルのnormal情報
	std::string line; // 1行を保存するよう

	std::vector<VertexDataBuffer> vertices; // vertexBuffer用
	std::vector<u32> indexes; // indexBuffer用

	std::string mtlFileName;

	std::unordered_map<std::string, u32> reverseMeshVertices; // 登録済み頂点情報とそのindex情報

	std::vector<PolygonMesh::MeshData>::iterator current = meshData.begin();

	// ファイルを開く
	std::ifstream file(filePath);
	if (!file.is_open()) {
		szgError("File \'{}\' is not found.", filePath.string());
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
	result = LoadMtl(directory / mtlFileName, materialData);
	return result;
}

bool LoadMtl(const std::filesystem::path& filePath, std::unordered_map<std::string, PolygonMesh::MeshMaterialData>& materialData) {
	std::string line;
	std::ifstream file;

	// mtlファイルを開く
	file.open(filePath);
	if (!file.is_open()) {
		szgWarning(L"File \'{}\' is not found.", filePath.native());
		return false;
	}

	PolygonMesh::MeshMaterialData* current = nullptr;

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
					std::filesystem::path directory{ filePath.parent_path() };
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

bool LoadMeshAssimp(const std::filesystem::path& filePath, std::vector<PolygonMesh::MeshData>& meshData, std::unordered_map<std::string, PolygonMesh::MeshMaterialData>& materialData) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath.string().c_str(),
		aiProcess_FlipUVs |
		aiProcess_FlipWindingOrder |
		aiProcess_LimitBoneWeights
	);
	if (importer.GetException() || !scene) {
		szgError("Failed to load mesh file. File-\'{}\' Message-\'{}\'", filePath.string(), importer.GetErrorString());
		return false;
	}
	if (!scene->HasMeshes()) {
		szgError("Can't find mesh. File-\'{}\'", filePath.string());
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
			PolygonMesh::MeshMaterialData newMaterial;
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
			szgError("This mesh don't have normal or texcoord. File-\'{}\'", filePath.string());
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
