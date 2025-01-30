#pragma once

#include <memory>
#include <vector>

#define DIRECTINPUT_VERSION 0x0800

#include <wrl/client.h>
#include <dinput.h>
#include <Xinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment (lib, "xinput.lib")

#include <Library/Math/Vector2.h>

#include "./InputEnum.h"

class Input {
private:
	Input() = default;
	~Input() = default;

	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

public:
	static Input& GetInstance();

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	static void Update();

	// ----------キーボード----------

	/// <summary>
	/// キーが押されている
	/// </summary>
	/// <param name="id">ID</param>
	/// <returns></returns>
	static bool IsPressKey(KeyID id);

	/// <summary>
	/// キーが押された瞬間
	/// </summary>
	/// <param name="id">ID</param>
	/// <returns></returns>
	static bool IsTriggerKey(KeyID id);

	/// <summary>
	/// キーが離された
	/// </summary>
	/// <param name="id">ID</param>
	/// <returns></returns>
	static bool IsReleaseKey(KeyID id);

	// ----------マウス----------
	/// <summary>
	/// マウスボタンが押されている
	/// </summary>
	/// <param name="id">ID</param>
	/// <returns></returns>
	static bool IsPressMouse(MouseID id);
	/// <summary>
	/// マウスボタンが押された瞬間
	/// </summary>
	/// <param name="id">ID</param>
	/// <returns></returns>
	static bool IsTriggerMouse(MouseID id);

	/// <summary>
	/// マウスボタンが離された
	/// </summary>
	/// <param name="id">ID</param>
	/// <returns></returns>
	static bool IsReleaseMouse(MouseID id);

	/// <summary>
	/// ウィンドウ基準でマウス位置を取得
	/// </summary>
	/// <returns></returns>
	static const Vector2& MousePosition();

	/// <summary>
	/// マウスの移動量を取得
	/// </summary>
	/// <returns></returns>
	static Vector2 MouseDelta();

	/// <summary>
	/// マウスホイールの移動量を取得
	/// </summary>
	/// <returns>[-1,1]</returns>
	static float WheelDelta();

	// ----------ゲームパッド----------
	/// <summary>
	/// Padボタンが押されているか
	/// </summary>
	/// <param name="id">ID</param>
	/// <returns></returns>
	static bool IsPressPad(PadID id);

	/// <summary>
	/// Padボタンがトリガーしたか
	/// </summary>
	/// <param name="id">ID</param>
	/// <returns></returns>
	static bool IsTriggerPad(PadID id);

	/// <summary>
	/// Padボタンが離されたか
	/// </summary>
	/// <param name="id">ID</param>
	/// <returns></returns>
	static bool IsReleasePad(PadID id);

	/// <summary>
	/// 左スティックの入力
	/// </summary>
	/// <returns>[-1,1]の値</returns>
	static Vector2 StickL();

	/// <summary>
	/// 右スティックの入力
	/// </summary>
	/// <returns>[-1,1]の値</returns>
	static Vector2 StickR();

	/// <summary>
	/// 左ショルダーの入力
	/// </summary>
	/// <returns>[0,1]の値</returns>
	static float TriggerL();

	/// <summary>
	/// 右ショルダーの入力
	/// </summary>
	/// <returns>[0,1]の値</returns>
	static float TriggerR();

	/// <summary>
	/// スティックのデッドゾーン設定
	/// </summary>
	/// <param name="deadZone">[0,1]で設定</param>
	static void SetDeadZone(float deadZone) { GetInstance().deadZone = deadZone; };

	/// <summary>
	/// 接続されたゲームパッド数を取得
	/// </summary>
	/// <returns>最大4(XInputの制約)</returns>
	static uint32_t NumPad();

	/// <summary>
	/// 使用可能Padをビットフラグで取得(未デバッグ)
	/// </summary>
	/// <returns></returns>
	static uint8_t ActivePad();

private:
	void create_direct_input();
	void create_keybord_device();
	void create_mouse_device();
	void initialize_joystate();

private:
	Microsoft::WRL::ComPtr<IDirectInput8> directInput = nullptr;

	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboardDevice;
	std::vector<BYTE> keyboardState;
	std::vector<BYTE> preKeyboardState;

	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouseDevice;
	Vector2 mousePosition;
	std::unique_ptr<DIMOUSESTATE2> mouseState;
	std::unique_ptr<DIMOUSESTATE2> preMouseState;

	uint32_t dwUserIndex = 0;
	std::unique_ptr<XINPUT_STATE> joystate;
	std::unique_ptr<XINPUT_STATE> preJoystate;
	float deadZone = 0.2f;
};

class InputAdvanced {
private:
	InputAdvanced() = default;
	~InputAdvanced() = default;

	InputAdvanced(const InputAdvanced&) = delete;
	InputAdvanced& operator=(const InputAdvanced&) = delete;

public:
	/// <summary>
	/// WASDの入力を取得
	/// ※ワールド空間ベース
	/// </summary>
	/// <returns></returns>
	static Vector2 PressWASD();

	/// <summary>
	/// 矢印の入力を取得
	/// ※ワールド空間ベース
	/// </summary>
	/// <returns></returns>
	static Vector2 PressArrow();

	/// <summary>
	/// 4キーの入力を取得
	/// </summary>
	/// <param name="up">Y+</param>
	/// <param name="down">Y-</param>
	/// <param name="left">X-</param>
	/// <param name="right">X+</param>
	/// <returns></returns>
	static Vector2 PressCustum(KeyID up, KeyID down, KeyID left, KeyID right);

private:
	static void NormalizeOneOrOver(Vector2& vector);
};
