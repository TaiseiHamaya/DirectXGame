#pragma once

#include <Library/Math/Transform3D.h>
#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

class IRemoteObject;

class EditorSelectObject {
public:
	struct SelectObject {
		Reference<IRemoteObject> object;
		Reference<Transform3D> transform;
	};

public:
	EditorSelectObject() = default;
	~EditorSelectObject() = default;

	__CLASS_NON_COPYABLE(EditorSelectObject)

public:
	void set_item(Reference<IRemoteObject> object_, Reference<Transform3D> transform_ = nullptr);
	const SelectObject& get_item();

	bool is_selected(Reference<const IRemoteObject> rhs) const;

private:
	SelectObject select;
};
