#pragma once

#include <wrl/client.h>
#include <xaudio2.h>

#include <Library/Utility/Template/SingletonInterface.h>

class AudioManager final : public SingletonInterface<AudioManager> {
	__CLASS_SINGLETON_INTERFACE(AudioManager)

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

#ifdef _DEBUG
public:
	static void DebugGui();
#endif // _DEBUG

private:
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masteringVoice{ nullptr };
	float masterVolume{ 1.0f };

public:
	static float GetMasterVolume();
	static void SetMasterVolume(float volume);
};
