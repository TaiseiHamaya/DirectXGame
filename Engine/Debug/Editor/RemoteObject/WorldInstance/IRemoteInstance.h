#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "../IRemoteObject.h"

#include <memory>
#include <vector>

#include <imgui.h>

#include <Library/Math/Transform3D.h>
#include <Library/Utility/Template/Reference.h>

#include "../../Core/EditorHierarchyDandD.h"
#include "Engine/Debug/Editor/Command/EditorCommandInvoker.h"
#include "Engine/Debug/Editor/Command/EditorSelectCommand.h"

template<typename RuntimeType, typename DebugVisualType>
class IRemoteInstance : public IRemoteObject {
public:
	IRemoteInstance() = default;
	virtual ~IRemoteInstance() = default;

	__CLASS_DEFAULT_ALL(IRemoteInstance)

public:
	void draw_hierarchy(Reference<const EditorSelectObject> select) override;

	std::unique_ptr<IRemoteObject> move_force(Reference<const IRemoteObject> child) override;

	void reparent(Reference<IRemoteObject> remoteObject) override;

	void add_child(std::unique_ptr<IRemoteObject> child) override;

	virtual void set_editor_world_view(Reference<EditorWorldView> worldView, Reference<const Affine> parentAffine) override;

protected:
	Reference<RuntimeType> self;

	std::vector<std::unique_ptr<IRemoteObject>> children;

	std::unique_ptr<DebugVisualType> debugVisual;

	EditorValueField<Transform3D> transform{ "Local transform" };
};

template<typename RuntimeType, typename DebugVisualType>
inline void IRemoteInstance<RuntimeType, DebugVisualType>::draw_hierarchy(Reference<const EditorSelectObject> select) {
	bool isSelected = select->is_selected(this);
	int flags =
		ImGuiTreeNodeFlags_DrawLinesToNodes |
		ImGuiTreeNodeFlags_SpanAllColumns |
		ImGuiTreeNodeFlags_OpenOnArrow | // 矢印で開く
		ImGuiTreeNodeFlags_OpenOnDoubleClick; // ダブルクリックで開く
	if (isSelected) {
		flags |= ImGuiTreeNodeFlags_Selected; // 選択時は選択状態にする
	}
	if (isOpen) {
		flags |= ImGuiTreeNodeFlags_DefaultOpen;
	}
	if (children.empty()) {
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	isOpen = ImGui::TreeNodeEx(std::format("{}##{}", hierarchyName.get(), (void*)this).c_str(), flags);
	EditorHierarchyDandD::CheckDandD(this, parent);

	// こうすると選択できるらしい
	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen() && !isSelected) {
		EditorCommandInvoker::Execute(
			std::make_unique<EditorSelectCommand>(this, transform.get())
		);
	}

	if (isOpen) {
		for (auto& child : children) {
			child->draw_hierarchy(select);
		}
		ImGui::TreePop();
	}
}

template<typename RuntimeType, typename DebugVisualType>
std::unique_ptr<IRemoteObject> IRemoteInstance<RuntimeType, DebugVisualType>::move_force(Reference<const IRemoteObject> child) {
	auto itr = std::find_if(children.begin(), children.end(), [&](const std::unique_ptr<IRemoteObject>& lhs) {
		return lhs.get() == child.ptr();
	});
	if (itr != children.end()) {
		std::unique_ptr<IRemoteObject> childObject = std::move(*itr);
		children.erase(itr);
		return childObject;
	}
	return nullptr;
}

template<typename RuntimeType, typename DebugVisualType>
void IRemoteInstance<RuntimeType, DebugVisualType>::reparent(Reference<IRemoteObject> remoteObject) {
	parent = remoteObject;
}

template<typename RuntimeType, typename DebugVisualType>
void IRemoteInstance<RuntimeType, DebugVisualType>::add_child(std::unique_ptr<IRemoteObject> child) {
	if (!child) {
		return;
	}
	child->reparent(this);
	children.emplace_back(std::move(child));
}

template<typename RuntimeType, typename DebugVisualType>
inline void IRemoteInstance<RuntimeType, DebugVisualType>::set_editor_world_view(Reference<EditorWorldView> worldView, Reference<const Affine> parentAffine) {
	Affine affine = Affine::FromTransform3D(transform.cget());
	if (parentAffine) {
		affine *= *parentAffine;
	}
	for (const auto& child : children) {
		if (child) {
			child->set_editor_world_view(worldView, affine);
		}
	}
}

#endif // DEBUG_FEATURES_ENABLE
