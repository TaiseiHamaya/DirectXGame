#pragma once

#include "Engine/GameObject/Transform2D/Transform2D.h"
#include "Matrix.h"
#include "Vector3.h"

#include <memory>

class Camera2D final {
private:// シングルトンなのでprivate
	Camera2D() noexcept = default;

public:	// コピー禁止
	Camera2D(const Camera2D&) = delete;
	Camera2D& operator=(const Camera2D&) = delete;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

public:
	/// <summary>
	/// カメラ位置の設定
	/// </summary>
	/// <param name="cameraMatrix_"></param>
	static void SetCameraPos(const Vector2& pos) noexcept;

	static void SetCameraTransform(const Transform2D& transform) noexcept;

	static void SetNDCInfomation(float left, float right, float bottom, float top, float near, float far) noexcept;

	static void CameraUpdate();

	static const Matrix4x4& GetVPMatrix() noexcept;

private:
	void camera_update();
	void make_view_matrix();
	void make_ortho_matrix();

#ifdef _DEBUG
public:
	static void DebugGUI();
private:
	void debug_gui();
#endif // _DEBUG

private:// 一つに絞る
	static std::unique_ptr<Camera2D> instance;

private:// メンバ変数
	Matrix4x4 viewMatrix;
	Matrix4x4 orthoMatrix;

	Matrix4x4 vpMatrix;

	Transform2D camera;
	Vector3 ndcLeftBottomNear;
	Vector3 ndcRightTopFar;
};