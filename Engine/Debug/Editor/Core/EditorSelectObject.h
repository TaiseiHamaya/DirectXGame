#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <Library/Math/Affine.h>
#include <Library/Math/Transform3D.h>
#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

namespace szg {

class IRemoteObject;

struct EditorSelectObjectBody {
	Reference<IRemoteObject> object;
	struct TransformData {
		Reference<Transform3D> transform{ nullptr };
		Reference<Affine> affine{ nullptr };
	} transformData;
};

class EditorSelectObject {
public:

public:
	EditorSelectObject() = default;
	~EditorSelectObject() = default;

	SZG_CLASS_MOVE_ONLY(EditorSelectObject)

public:
	void set_item(Reference<IRemoteObject> object_, EditorSelectObjectBody::TransformData transform_ = {});
	EditorSelectObjectBody& get_item_mut();

	bool is_selected(Reference<const IRemoteObject> rhs) const;

private:
	EditorSelectObjectBody select;
};

}; // szg

#endif // DEBUG_FEATURES_ENABLE
