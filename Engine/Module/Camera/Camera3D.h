#pragma once

#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"
#include "Engine/Module/GameObject/GameObject.h"

class Camera3D : public GameObject {
public:
	Camera3D() = default;
	virtual ~Camera3D() = default;

	Camera3D(const Camera3D&) = delete;
	Camera3D& operator=(const Camera3D&) = delete;

public:
	virtual void initialize();

	void update_matrix();
	
	void set_command(uint32_t index);

public:
	void set_transform(const Transform3D& transform) noexcept;

	void set_perspective_fov_info(float fovY, float aspectRatio, float nearClip, float farClip) noexcept;

	const Matrix4x4& vp_matrix() const;

private:
	void make_view_matrix();
	void make_perspectivefov_matrix();

#ifdef _DEBUG
public:
	virtual void debug_gui();
	void debug_camera();
	void debug_draw() const;
#endif // _DEBUG

private:
	Matrix4x4 viewMatrix;
	Matrix4x4 perspectiveFovMatrix;

	ConstantBuffer<Matrix4x4> vpMatrix;

	float fovY;
	float aspectRatio;
	float nearClip;
	float farClip;

#ifdef _DEBUG
	Matrix4x4 vpMatrixCamera;
	Matrix4x4 debugViewMatrix;
	bool isVaildDebugCamera;
	std::unique_ptr<GameObject> debugCameraCenter;
	std::unique_ptr<GameObject> debugCamera;
	Vector3 offset;
	Vector2 preMousePos;
#endif // _DEBUG
};