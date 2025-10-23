#pragma once

#include <string>
#include <xaudio2.h>

#include <Library/Utility/Tools/ConstructorMacro.h>

/// <summary>
/// オーディオ再生
/// </summary>
class AudioPlayer {
public:
	AudioPlayer() = default;
	~AudioPlayer();

	__CLASS_NON_COPYABLE(AudioPlayer)

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="name">音声ファイル名</param>
	/// <param name="volume">音量</param>
	/// <param name="isLoop">ループ再生フラグ</param>
	void initialize(const std::string& name, r32 volume = 1.0f, bool isLoop = false);

	/// <summary>
	/// 設定済みオーディオの破棄
	/// </summary>
	void destroy();

	/// <summary>
	/// 再生
	/// </summary>
	void play();

	/// <summary>
	/// 停止
	/// </summary>
	void stop();

	/// <summary>
	/// 一時停止
	/// </summary>
	void pause();

	/// <summary>
	/// 最初から再生(まだ再生されてなかった場合は通常通り再生)
	/// </summary>
	void restart();

	void set_volume(r32 volume);
	void set_loop(bool isLoop);

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // _DEBUG

private:
	XAUDIO2_BUFFER buffer{};
	IXAudio2SourceVoice* sourceVoice{ nullptr };
};
