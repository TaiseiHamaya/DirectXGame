#pragma once

#include "Engine/GameObject/Transform3D/Transform3D.h"
#include "Vector2.h"

#include <memory>

class Camera3D final {
	friend class Debug;
private:// シングルトンなのでprivate
	Camera3D() = default;

public:	// コピー禁止
	Camera3D(const Camera3D&) = delete;
	Camera3D& operator=(const Camera3D&) = delete;

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
	static void SetCameraPos(const Vector3& pos) noexcept;

	static void SetCameraTransform(const Transform3D& transform) noexcept;

	static void SetPerspectiveFovInfomation(float fovY, float aspectRatio, float nearClip, float farClip) noexcept;

	static void CameraUpdate();

	static const Matrix4x4& GetVPMatrix() noexcept;

#ifdef _DEBUG
	static void DebugGUI();
#endif // _DEBUG

public:
	static const Transform3D& GetCameraTransform() noexcept;

private:
	void camera_update();
	void make_view_matrix();
	void make_perspectivefov_matrix();
#ifdef _DEBUG
	void _debug_gui();
#endif // _DEBUG
private:// 一つに絞る
	static std::unique_ptr<Camera3D> instance;

private:
	Matrix4x4 viewMatrix;
	Matrix4x4 persectiveFovMatrix;

	Matrix4x4 vpMatrix;

	Transform3D camera;

	float fovY;
	float aspectRatio;
	float nearClip;
	float farClip;
};