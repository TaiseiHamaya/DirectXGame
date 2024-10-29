#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class PolygonMesh;

class PolygonMeshManager final {
private:
	PolygonMeshManager() noexcept;

public:
	~PolygonMeshManager() noexcept;

private:
	PolygonMeshManager(const PolygonMeshManager&) = delete;
	PolygonMeshManager& operator=(const PolygonMeshManager&) = delete;

public:
	static PolygonMeshManager& GetInstance() noexcept;
	
	/// <summary>
	/// ロードキューに.objファイルを追加
	/// </summary>
	/// <param name="directoryPath">ディレクトリ</param>
	/// <param name="fileName">ファイル名</param>
	static void RegisterLoadQue(const std::string& directoryPath, const std::string& fileName);

	/// <summary>
	/// メッシュの取得
	/// </summary>
	/// <param name="meshName">メッシュ名</param>
	/// <returns>PolygonMeshクラスのweak_ptr</returns>
	static std::weak_ptr<PolygonMesh> GetPolygonMesh(const std::string& meshName);

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
	static bool IsRegisteredNolocking(const std::string& meshName);

private:
	std::unordered_map<std::string, std::shared_ptr<PolygonMesh>> meshInstanceList;
};
