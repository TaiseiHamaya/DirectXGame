#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

class AudioAsset;

class AudioLibrary final {
private:
	AudioLibrary() noexcept;

public:
	~AudioLibrary() noexcept;

private:
	AudioLibrary(const AudioLibrary&) = delete;
	AudioLibrary& operator=(const AudioLibrary&) = delete;

public:
	static AudioLibrary& GetInstance() noexcept;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

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
	/// オーディオデータの取得
	/// </summary>
	/// <param name="audioName">拡張子付きファイル名</param>
	/// <returns>AudioResourceクラスのweak_ptr</returns>
	static const std::unique_ptr<AudioAsset>& GetAudio(const std::string& audioName);

	/// <summary>
	/// オーディオが登録済みか取得
	/// </summary>
	/// <param name="audioName">拡張子付きファイル名</param>
	/// <returns>bool値</returns>
	static bool IsRegistered(const std::string& audioName) noexcept(false);

	/// <summary>
	/// 読み込み済みAudioのアンロード
	/// </summary>
	/// <param name="audioName">オーディオ名</param>
	static void UnloadAudio(const std::string& audioName);

	/// <summary>
	/// 転送[ユーザー使用は基本しないこと]
	/// </summary>
	/// <param name="name">転送時の名前</param>
	/// <param name="data">転送データ</param>
	static void Transfer(const std::string& name, std::unique_ptr<AudioAsset>&& data);

private:
	/// <summary>
	/// オーディオが登録済みか取得(mutexなし)
	/// </summary>
	/// <param name="textureName">オーディオ名</param>
	/// <returns>bool値</returns>
	static bool IsRegisteredNonlocking(const std::string& textureName) noexcept(false);

private:
	std::unordered_map<std::string, std::unique_ptr<AudioAsset>> audioResources;
};
