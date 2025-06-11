#pragma once

#include <memory>

#include <ImGuizmo.h>

#include <Library/Math/Matrix4x4.h>
#include <Library/Math/Vector2.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "./RemoteObject/IRemoteObject.h"

class Camera3D;
class EditorObjectMoveCommand;
class EditorSelectObject;

class EditorGizmo {
public:
	EditorGizmo();
	~EditorGizmo();

	__CLASS_NON_COPYABLE(EditorGizmo)

public:
	void begin_frame(Reference<Camera3D> camera, const Vector2& origin, const Vector2& size);
	void draw_gizmo(Reference<EditorSelectObject> object);
	void scene_header();

private:
	std::unique_ptr<EditorObjectMoveCommand> moveCommand;

	Matrix4x4 view;
	Matrix4x4 proj;

	ImGuizmo::MODE mode{ ImGuizmo::MODE::WORLD };
	ImGuizmo::OPERATION operation{ ImGuizmo::OPERATION::TRANSLATE };
};
