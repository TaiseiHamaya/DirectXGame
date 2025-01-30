#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

class PolygonMesh;

class PolygonMeshLibrary final {
private:
	PolygonMeshLibrary() noexcept;

public:
	~PolygonMeshLibrary() noexcept;

private:
	PolygonMeshLibrary(const PolygonMeshLibrary&) = delete;
	PolygonMeshLibrary& operator=(const PolygonMeshLibrary&) = delete;

public:
	static PolygonMeshLibrary& GetInstance() noexcept;
	
	/// <summary>
	/// ロードキューに.objファイルを追加
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	static void RegisterLoadQue(const std::filesystem::path& filePath);

	/// <summary>
	/// メッシュの取得
	/// </summary>
	/// <param name="meshName">メッシュ名</param>
	/// <returns>PolygonMeshクラスのweak_ptr</returns>
	static std::shared_ptr<const PolygonMesh> GetPolygonMesh(const std::string& meshName);

	/// <summary>
	/// メッシュが登録されているか取得
	/// </summary>
	/// <param name="meshName">確認するメッシュ名</param>
	/// <returns>登録されていればtrue[bool]</returns>
	static bool IsRegistered(const std::string& meshName);

	/// <summary>
	/// nameでPolygonMeshデータを登録済みデータに保存
	/// </summary>
	/// <param name="name">登録名</param>
	/// <param name="data">ロード済みデータ</param>
	static void Transfer(const std::string& name, std::shared_ptr<PolygonMesh>& data);

#ifdef _DEBUG
	/// <summary>
	/// メッシュ一覧をComboBoxで表示するImGui(Debugビルドのみ)
	/// </summary>
	/// <param name="current">現在選択中のメッシュ名</param>
	/// <returns>currentが変更されたかどうか</returns>
	static bool MeshListGui(std::string& current);
#endif // _DEBUG

private:
	/// <summary>
	/// メッシュが登録されているか取得(mutexなし)
	/// </summary>
	/// <param name="meshName">確認するメッシュ名</param>
	/// <returns>登録されていればtrue[bool]</returns>
	static bool IsRegisteredNonlocking(const std::string& meshName);

private:
	std::unordered_map<std::string, std::shared_ptr<PolygonMesh>> meshInstanceList;
};
