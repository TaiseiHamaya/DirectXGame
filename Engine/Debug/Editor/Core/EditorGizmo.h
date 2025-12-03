#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <ImGuizmo.h>

#include <bitset>

#include <Library/Math/Matrix4x4.h>
#include <Library/Math/Vector2.h>
#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Runtime/Input/InputHandler.h"

class Camera3D;
class EditorObjectMoveCommand;
class EditorSelectObject;
class EditorWorldView;

class EditorGizmo {
public:
	EditorGizmo();
	~EditorGizmo();

	__CLASS_NON_COPYABLE(EditorGizmo)

public:
	void begin_frame(const Vector2& origin, const Vector2& size);
	void draw_gizmo(Reference<EditorSelectObject> object, Reference<const EditorWorldView> world);
	void scene_header();

private:
	std::bitset<2> gizmoState;

	Matrix4x4 view;
	Matrix4x4 proj;

	ImGuizmo::MODE mode{ ImGuizmo::MODE::WORLD };
	ImGuizmo::OPERATION operation{ ImGuizmo::OPERATION::TRANSLATE };

	InputHandler<KeyID> gizmoKeyHandler;
};

#endif // DEBUG_FEATURES_ENABLE
