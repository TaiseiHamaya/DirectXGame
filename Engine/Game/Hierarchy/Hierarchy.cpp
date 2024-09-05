#include "Hierarchy.h"

#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/TransformMatrix/TransformMatrix.h"
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Math/Matrix4x4.h"

void Hierarchy::initialize(const TransformMatrix& transformMatrix) {
	currentMatrix = &transformMatrix.get_data()->world;
}

void Hierarchy::set_parent(const GameObject& gameObject) noexcept {
	parent = &gameObject.get_hierarchy();
}

const Hierarchy& Hierarchy::get_parent() const noexcept {
	return *parent;
}

bool Hierarchy::has_parent() const noexcept {
	return !parent;
}

const Matrix4x4& Hierarchy::parent_matrix() const {
	return *parent->currentMatrix;
}
