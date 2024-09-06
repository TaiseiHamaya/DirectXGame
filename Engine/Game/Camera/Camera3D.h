#pragma once

#include "Engine/Game/GameObject/GameObject.h"

class Camera3D : public GameObject {
public:
	Camera3D() = default;
	virtual ~Camera3D() = default;

	Camera3D(const Camera3D&) = delete;
	Camera3D& operator=(const Camera3D&) = delete;

public:
	virtual void initialize();

	void update_matrix();

public:
	void set_transform(const Transform3D& transform) noexcept;

	void set_perspective_fov_info(float fovY, float aspectRatio, float nearClip, float farClip) noexcept;

	const Matrix4x4& vp_matrix() const;

#ifdef _DEBUG
	virtual void debug_gui();
	//void debug_camera();
#endif // _DEBUG

private:
	void make_view_matrix();
	void make_perspectivefov_matrix();

private:
	Matrix4x4 viewMatrix;
	Matrix4x4 perspectiveFovMatrix;

	Matrix4x4 vpMatrix;

	float fovY;
	float aspectRatio;
	float nearClip;
	float farClip;
};