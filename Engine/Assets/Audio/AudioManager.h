#pragma once

#include <wrl/client.h>
#include <xaudio2.h>

#include <Library/Utility/Template/SingletonInterface.h>

/// <summary>
/// XAudio2管理クラス
/// </summary>
namespace szg {

class AudioManager final : public SingletonInterface<AudioManager> {
	SZG_CLASS_SINGLETON(AudioManager)

public:
	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	static void Finalize();

	static const Microsoft::WRL::ComPtr<IXAudio2>& GetXAudio2() { return GetInstance().xAudio2; };

#ifdef DEBUG_FEATURES_ENABLE
public:
	static void DebugGui();
#endif // _DEBUG

private:
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masteringVoice{ nullptr };
	r32 masterVolume{ 1.0f };

public:
	static r32 GetMasterVolume();
	static void SetMasterVolume(r32 volume);
};

}; // szg
