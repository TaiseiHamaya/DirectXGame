#pragma once

#include <Library/Math/Vector2.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h"
#include "Engine/Module/World/WorldInstance/WorldInstance.h"

#ifdef DEBUG_FEATURES_ENABLE
#include "Engine/Runtime/Input/InputHandler.h"
#include "Engine/Module/World/Mesh/StaticMeshInstance.h"
#include "Engine/Module/DrawExecutor/PrimitiveGeometryDrawExecutor/PrimitiveGeometryDrawExecutor.h"
#endif // _DEBUG

namespace szg {

class Camera3D : public WorldInstance {
	friend class RemoteCamera3DInstance;

public:
	struct VpBuffers {
		Matrix4x4 viewProjection;
		Matrix4x4 view;
	};

	struct LightingPathBuffer {
		Vector3 position;
		u32 padding{ 0 };
		Matrix4x4 viewInv;
		Matrix4x4 projInv;
	};

public:
	Camera3D() = default;
	virtual ~Camera3D() = default;

	SZG_CLASS_MOVE_ONLY(Camera3D)

public:
	virtual void initialize();

	void update_affine() override;
	void transfer();

	void register_world_projection(u32 index) const;
	void register_world_lighting(u32 index) const;

public:
	void set_transform(const Transform3D& transform) noexcept;

	void set_perspective_fov_info(r32 fovY, r32 aspectRatio, r32 nearClip, r32 farClip) noexcept;

	const Matrix4x4& vp_matrix() const;

	const Affine& view_affine() const;
	const Matrix4x4& proj_matrix() const;

protected:
	void make_view_matrix();
	void make_perspectivefov_matrix();

public:
	static Matrix4x4 MakeViewportMatrix(const Vector2& origin, const Vector2& size, r32 minDepth = 0.0f, r32 maxDepth = 1.0f);

#ifdef DEBUG_FEATURES_ENABLE
public:
	virtual void debug_gui();
	void debug_camera();
	void debug_draw_axis();
	void debug_draw_frustum() const;
	const Matrix4x4& vp_matrix_debug() const;
	const Affine& debug_view_affine() const;
	const Matrix4x4& debug_proj_matrix() const;
	Reference<const StaticMeshInstance> camera_axis() const { return debugCameraCenter; }
#endif // _DEBUG

private:
	Affine viewAffine;
	Matrix4x4 projectionMatrix;

	ConstantBuffer<VpBuffers> vpBuffers;
	ConstantBuffer<LightingPathBuffer> lightingBuffer;

	r32 fovY;
	r32 aspectRatio;
	r32 nearClip;
	r32 farClip;

#ifdef DEBUG_FEATURES_ENABLE
	Matrix4x4 vpMatrix;
	Affine debugViewAffine;
	bool isValidDebugCamera{ false };
	bool useDebugCameraLighting{ true };
	Reference<StaticMeshInstance> debugCameraCenter;
	Reference<WorldInstance> debugCamera;
	std::unique_ptr<PrimitiveGeometryDrawExecutor> frustumExecutor;
	Vector3 offset;
	InputHandler<MouseID> debugMouseInputHandler;
#endif // _DEBUG
};

}; // szg
