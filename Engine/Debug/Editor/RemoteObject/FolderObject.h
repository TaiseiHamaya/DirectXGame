#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IRemoteObject.h"

#include <memory>
#include <vector>

class FolderObject final : public IRemoteObject {
public:
	friend class EditorSceneSerializer;

public:
	void draw_inspector() override;

	void draw_hierarchy(Reference<const EditorSelectObject> select) override;

private:
	std::vector<std::unique_ptr<IRemoteObject>> children;
};

#endif // DEBUG_FEATURES_ENABLE
