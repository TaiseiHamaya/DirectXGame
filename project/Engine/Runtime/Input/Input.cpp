#include "Input.h"

#include <cassert>

#include <Engine/Application/WinApp.h>

Input& Input::GetInstance() {
	static Input instance;
	return instance;
}

void Input::Initialize() {
	Input& instance = GetInstance();

	//各種初期化
	instance.create_direct_input();
	instance.create_keybord_device();
	instance.create_mouse_device();
	instance.initialize_joystate();
}

void Input::Update() {
	HRESULT result;
	Input& instance = GetInstance();
	// preと今をswapすることで前状態を記録
	std::swap(instance.joystate, instance.preJoystate);
	std::swap(instance.mouseState, instance.preMouseState);
	std::swap(instance.keyboardState, instance.preKeyboardState);

	// ゲームパッド更新
	XInputGetState(instance.dwUserIndex, instance.joystate.get());

	// キーボード更新
	instance.keyboardDevice->Acquire();
	result = instance.keyboardDevice->GetDeviceState(
		static_cast<DWORD>(instance.keyboardState.size() * sizeof(BYTE)),
		instance.keyboardState.data()
	);
	if (FAILED(result)) {
		// 失敗したら0埋め
		std::fill(instance.keyboardState.begin(), instance.keyboardState.end(), 0);
	}

	// マウス更新
	instance.mouseDevice->Acquire();
	result = instance.mouseDevice->GetDeviceState(
		sizeof(DIMOUSESTATE2),
		instance.mouseState.get()
	);
	if (FAILED(result)) {
		// 失敗したら0埋め
		*instance.mouseState = { 0 };
	}

	// マウス位置更新
	POINT point;
	DWORD error;
	if (!GetCursorPos(&point)) {
		error = GetLastError();
	}
	if (!ScreenToClient(WinApp::GetWndHandle(), &point)) {
		error = GetLastError();
	}
	instance.mousePosition = { static_cast<float>(point.x), static_cast<float>(point.y) };
}

bool Input::IsPressKey(KeyID id) {
	int idInt = static_cast<int>(id);
	auto& keyboardState = GetInstance().keyboardState;
	return keyboardState[idInt];
}

bool Input::IsTriggerKey(KeyID id) {
	int idInt = static_cast<int>(id);
	Input& instance = GetInstance();
	auto& keyboardState = instance.keyboardState;
	auto& preKeyboardState = instance.preKeyboardState;
	return keyboardState[idInt] && !preKeyboardState[idInt];
}

bool Input::IsReleaseKey(KeyID id) {
	int idInt = static_cast<int>(id);
	Input& instance = GetInstance();
	auto& keyboardState = instance.keyboardState;
	auto& preKeyboardState = instance.preKeyboardState;
	return !keyboardState[idInt] && preKeyboardState[idInt];
}

bool Input::IsPressMouse(MouseID id) {
	int idInt = static_cast<int>(id);
	auto& rgbButtons = GetInstance().mouseState->rgbButtons;
	return rgbButtons[idInt];
}

bool Input::IsTriggerMouse(MouseID id) {
	int idInt = static_cast<int>(id);
	Input& instance = GetInstance();
	auto& rgbButtons = instance.mouseState->rgbButtons;
	auto& preRgbButtons = instance.preMouseState->rgbButtons;
	return rgbButtons[idInt] && !preRgbButtons[idInt];
}

bool Input::IsReleaseMouse(MouseID id) {
	int idInt = static_cast<int>(id);
	Input& instance = GetInstance();
	auto& rgbButtons = instance.mouseState->rgbButtons;
	auto& preRgbButtons = instance.preMouseState->rgbButtons;
	return !rgbButtons[idInt] && preRgbButtons[idInt];
}

const Vector2& Input::MousePosition() {
	return GetInstance().mousePosition;
}

Vector2 Input::MouseDelta() {
	Input& instance = GetInstance();
	return Vector2{
		static_cast<float>(instance.mouseState->lX),
		static_cast<float>(instance.mouseState->lY),
	};
}

float Input::WheelDelta() {
	Input& instance = GetInstance();
	// mouseLZは120区切りっぽい？
	return (instance.mouseState->lZ) / 120.0f;
}

bool Input::IsPressPad(PadID id) {
	int idInt = static_cast<int>(id);
	auto& buttons = GetInstance().joystate->Gamepad.wButtons;
	return buttons & idInt;
}

bool Input::IsTriggerPad(PadID id) {
	int idInt = static_cast<int>(id);
	Input& instance = GetInstance();
	auto& buttons = instance.joystate->Gamepad.wButtons;
	auto& preButtons = instance.preJoystate->Gamepad.wButtons;
	return (buttons & idInt) && !(preButtons & idInt);
}

bool Input::IsReleasePad(PadID id) {
	int idInt = static_cast<int>(id);
	Input& instance = GetInstance();
	auto& buttons = instance.joystate->Gamepad.wButtons;
	auto& preButtons = instance.preJoystate->Gamepad.wButtons;
	return !(buttons & idInt) && (preButtons & idInt);
}

Vector2 Input::StickL() {
	Input& instance = GetInstance();
	auto& gamepad = instance.joystate->Gamepad;
	Vector2 result{
		static_cast<float>(gamepad.sThumbLX),
		static_cast<float>(gamepad.sThumbLY)
	};
	// ShortMaxで割ることで[-1,1]にする
	result /= std::numeric_limits<SHORT>::max();

	// lengthとデッドゾーンを比較する
	if (result.length() < instance.deadZone) {
		result = CVector2::ZERO;
	}

	return result;
}

Vector2 Input::StickR() {
	Input& instance = GetInstance();
	auto& gamepad = instance.joystate->Gamepad;
	Vector2 result{
		static_cast<float>(gamepad.sThumbRX),
		static_cast<float>(gamepad.sThumbRY)
	};
	// ShortMaxで割ることで[-1,1]にする
	result /= std::numeric_limits<SHORT>::max();

	// lengthとデッドゾーンを比較する
	if (result.length() < instance.deadZone) {
		result = CVector2::ZERO;
	}

	return result;
}

float Input::TriggerL() {
	Input& instance = GetInstance();
	auto& gamepad = instance.joystate->Gamepad;
	// ByteMaxで割ることで[0,1]にする
	return (float)gamepad.bLeftTrigger / std::numeric_limits<BYTE>::max();
}

float Input::TriggerR() {
	Input& instance = GetInstance();
	auto& gamepad = instance.joystate->Gamepad;
	return (float)gamepad.bRightTrigger / std::numeric_limits<BYTE>::max();
}

uint32_t Input::NumPad() {
	XINPUT_STATE tester{ 0 };
	HRESULT result{};
	uint32_t numPad = 0;
	// 接続可能なPad数は4つまで
	for (uint32_t i = 0; i < 4; ++i) {
		result = XInputGetState(i, &tester);
		// S_OKだったら使用可能
		if (result == S_OK) {
			++numPad;
		}
	}
	return numPad;
}

uint8_t Input::ActivePad() {
	XINPUT_STATE tester{ 0 };
	HRESULT result{};
	uint8_t active = 0;
	// 接続可能なPad数は4つまで
	for (int i = 0; i < 4; ++i) {
		result = XInputGetState(i, &tester);
		// S_OKだったら使用可能
		if (result == S_OK) {
			active |= 1 << i;
		}
	}
	return active;
}

void Input::create_direct_input() {
	HRESULT result;

	// 作成
	result = DirectInput8Create(
		WinApp::GetWindowHandle(),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		reinterpret_cast<void**>(directInput.GetAddressOf()),
		nullptr
	);
	assert(SUCCEEDED(result));
}

void Input::create_keybord_device() {
	HRESULT result;
	// 作成
	result = directInput->CreateDevice(
		GUID_SysKeyboard,
		keyboardDevice.GetAddressOf(),
		nullptr
	);
	assert(SUCCEEDED(result));
	// フォーマットの設定
	result = keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	// その他コンフィグ
	result = keyboardDevice->SetCooperativeLevel(
		WinApp::GetWndHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
	);
	assert(SUCCEEDED(result));

	// stateの初期化
	keyboardState.resize(256);
	preKeyboardState.resize(256);
}

void Input::create_mouse_device() {
	HRESULT result;
	// 作成
	result = directInput->CreateDevice(
		GUID_SysMouse,
		mouseDevice.GetAddressOf(),
		nullptr
	);
	assert(SUCCEEDED(result));
	// フォーマットの設定
	result = mouseDevice->SetDataFormat(&c_dfDIMouse2);
	assert(SUCCEEDED(result));
	// その他コンフィグ
	result = mouseDevice->SetCooperativeLevel(
		WinApp::GetWndHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
	);
	assert(SUCCEEDED(result));

	// stateの生成
	mouseState = std::make_unique<DIMOUSESTATE2>();
	preMouseState = std::make_unique<DIMOUSESTATE2>();
}

void Input::initialize_joystate() {
	// stateの生成
	joystate = std::make_unique<XINPUT_STATE>();
	preJoystate = std::make_unique<XINPUT_STATE>();
}

Vector2 InputAdvanced::PressWASD() {
	return PressCustum(KeyID::W, KeyID::S, KeyID::A, KeyID::D);
}

Vector2 InputAdvanced::PressArrow() {
	return PressCustum(KeyID::Up, KeyID::Down, KeyID::Left, KeyID::Right);
}

Vector2 InputAdvanced::PressCustum(KeyID up, KeyID down, KeyID left, KeyID right) {
	Vector2 result = CVector2::ZERO;
	if (Input::IsPressKey(up)) {
		result.y += 1.0f;
	}
	if (Input::IsPressKey(down)) {
		result.y -= 1.0f;
	}
	if (Input::IsPressKey(right)) {
		result.x += 1.0f;
	}
	if (Input::IsPressKey(left)) {
		result.x -= 1.0f;
	}
	NormalizeOneOrOver(result);
	return result;
}

void InputAdvanced::NormalizeOneOrOver(Vector2& vector) {
	if (vector.length() >= 1) {
		vector = vector.normalize();
	}
}
