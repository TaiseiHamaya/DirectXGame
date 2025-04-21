#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

class Texture;

class TextureLibrary final {
private:
	TextureLibrary();

public:
	~TextureLibrary() noexcept;

private:
	TextureLibrary(const TextureLibrary&) = delete;
	TextureLibrary& operator=(const TextureLibrary&) = delete;

public:
	static TextureLibrary& GetInstance() noexcept;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize() noexcept;

	/// <summary>
	/// 終了処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// ロードキューにファイルを追加
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	static void RegisterLoadQue(const std::filesystem::path& filePath);

	/// <summary>
	/// テクスチャデータの取得
	/// </summary>
	/// <param name="textureName">拡張子付きファイル名</param>
	/// <returns>Textureクラスのweak_ptr</returns>
	static std::shared_ptr<const Texture> GetTexture(const std::string& textureName) noexcept(false);
	
	/// <summary>
	/// テクスチャが登録済みか取得
	/// </summary>
	/// <param name="textureName">拡張子付きファイル名</param>
	/// <returns>bool値</returns>
	static bool IsRegistered(const std::string& textureName) noexcept(false);

	/// <summary>
	/// 読み込み済みテクスチャのアンロード
	/// </summary>
	/// <param name="textureName">テクスチャ名</param>
	static void UnloadTexture(const std::string& textureName);

	/// <summary>
	/// 転送[ユーザー使用は基本しないこと]
	/// </summary>
	/// <param name="name">転送時の名前</param>
	/// <param name="data">転送データ</param>
	static void Transfer(const std::string& name, std::shared_ptr<Texture>& data);

#ifdef DEBUG_FEATURES_ENABLE
	/// <summary>
	/// 登録済みファイルをImGuiComboで取得
	/// </summary>
	/// <param name="current">現在選択中のテクスチャ名</param>
	/// <returns>current変更フラグ</returns>
	static bool TextureListGui(std::shared_ptr<const Texture>& current);
#endif // _DEBUG

private:
	/// <summary>
	/// テクスチャが登録済みか取得(mutexなし)
	/// </summary>
	/// <param name="textureName">テクスチャ名</param>
	/// <returns>bool値</returns>
	static bool IsRegisteredNonlocking(const std::string& textureName) noexcept(false);

private: // メンバ変数
	std::unordered_map<std::string, std::shared_ptr<Texture>> textureInstanceList; // テクスチャリスト
};

