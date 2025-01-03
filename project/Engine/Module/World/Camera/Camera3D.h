#pragma once

#include "Library/Math/Vector2.h"

#include "Engine/Module/World/WorldInstance/WorldInstance.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"

#ifdef _DEBUG
#include "Engine/Module/World/Mesh/MeshInstance.h"
#endif // _DEBUG

class Camera3D : public WorldInstance {
public:
	Camera3D() = default;
	virtual ~Camera3D() = default;

	Camera3D(const Camera3D&) = delete;
	Camera3D& operator=(const Camera3D&) = delete;

public:
	virtual void initialize();

	void update_matrix();

	void register_world(uint32_t index);

public:
	void set_transform(const Transform3D& transform) noexcept;

	void set_perspective_fov_info(float fovY, float aspectRatio, float nearClip, float farClip) noexcept;

	const Matrix4x4& vp_matrix() const;

private:
	void make_view_matrix();
	void make_perspectivefov_matrix();

public:
	static Matrix4x4 MakeViewportMatrix(const Vector2& origin, const Vector2& size, float minDepth = 0.0f, float maxDepth = 1.0f);

#ifdef _DEBUG
public:
	virtual void debug_gui();
	void debug_camera();
	void debug_draw() const;
	const Matrix4x4& vp_matrix_debug() const;
#endif // _DEBUG

private:
	Affine viewAffine;
	Matrix4x4 perspectiveFovMatrix;

	ConstantBuffer<Matrix4x4> vpMatrixBuffer;

	float fovY;
	float aspectRatio;
	float nearClip;
	float farClip;

#ifdef _DEBUG
	Matrix4x4 vpMatrix;
	Affine debugViewAffine;
	bool isVaildDebugCamera;
	std::unique_ptr<MeshInstance> debugCameraCenter;
	std::unique_ptr<WorldInstance> debugCamera;
	std::unique_ptr<MeshInstance> frustum;
	Vector3 offset;
	Vector2 preMousePos;
#endif // _DEBUG
};