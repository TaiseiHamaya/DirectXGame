#pragma once

#include "Library/Math/Vector2.h"

#include "Engine/Module/World/WorldInstance/WorldInstance.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"

#ifdef _DEBUG
#include "Engine/Module/World/Mesh/MeshInstance.h"
#include "Engine/Module/DrawExecutor/PrimitiveLineDrawExecutor/PrimitiveLineDrawExecutor.h"
#endif // _DEBUG

class Camera3D : public WorldInstance {
public:
	struct CameraVPBuffers {
		Matrix4x4 viewProjection;
		Matrix4x4 view;
	};

	struct LightingPathBuffer {
		Vector3 position;
		uint32_t padding{ 0 };
		Matrix4x4 viewInv;
	};

public:
	Camera3D() = default;
	virtual ~Camera3D() = default;

	Camera3D(const Camera3D&) = delete;
	Camera3D& operator=(const Camera3D&) = delete;

public:
	virtual void initialize();

	void update_matrix();

	void register_world_projection(uint32_t index);
	void register_world_lighting(uint32_t index);

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
	void debug_draw_axis() const;
	void debug_draw_frustum() const;
	const Matrix4x4& vp_matrix_debug() const;
#endif // _DEBUG

private:
	Affine viewAffine;
	Matrix4x4 perspectiveFovMatrix;

	ConstantBuffer<CameraVPBuffers> vpBuffers;
	ConstantBuffer<LightingPathBuffer> worldPosition;

	float fovY;
	float aspectRatio;
	float nearClip;
	float farClip;

#ifdef _DEBUG
	Matrix4x4 vpMatrix;
	Affine debugViewAffine;
	bool isValidDebugCamera;
	bool useDebugCameraLighting;
	std::unique_ptr<MeshInstance> debugCameraCenter;
	std::unique_ptr<WorldInstance> debugCamera;
	std::unique_ptr<PrimitiveLineDrawExecutor> frustumExecutor;
	Vector3 offset;
	Vector2 preMousePos;
#endif // _DEBUG
};