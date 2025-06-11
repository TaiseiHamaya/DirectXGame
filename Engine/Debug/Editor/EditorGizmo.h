#pragma once

#include <ImGuizmo.h>

#include <Library/Math/Matrix4x4.h>
#include <Library/Math/Vector2.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "./RemoteObject/IRemoteObject.h"

class Camera3D;

class EditorGizmo {
public:
	EditorGizmo() = default;
	~EditorGizmo() = default;

	__CLASS_NON_COPYABLE(EditorGizmo)

public:
	void begin_frame(Reference<Camera3D> camera, const Vector2& origin, const Vector2& size);
	void draw_gizmo(Reference<IRemoteObject> object);

private:
	Matrix4x4 view;
	Matrix4x4 proj;

	ImGuizmo::OPERATION operation{ ImGuizmo::OPERATION::TRANSLATE };
};
