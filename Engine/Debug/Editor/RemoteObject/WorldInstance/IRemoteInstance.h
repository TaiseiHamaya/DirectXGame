#pragma once

#include "../IRemoteObject.h"

#include <memory>
#include <vector>

#include <Library/Math/Transform3D.h>
#include <Library/Utility/Template/Reference.h>

template<typename RuntimeType>
class IRemoteInstance : public IRemoteObject {
public:
	IRemoteInstance() = default;
	virtual ~IRemoteInstance() = default;

	__CLASS_DEFAULT_ALL(IRemoteInstance)

public:
	std::unique_ptr<IRemoteObject> move_force(Reference<const IRemoteObject> child) override;

	void reparent(Reference<IRemoteObject> remoteObject) override;

	void add_child(std::unique_ptr<IRemoteObject> child) override;

protected:
	Reference<RuntimeType> self;

	std::vector<std::unique_ptr<IRemoteObject>> children;
	Transform3D transform;
};

template<typename RuntimeType>
std::unique_ptr<IRemoteObject> IRemoteInstance<RuntimeType>::move_force(Reference<const IRemoteObject> child) {
	auto itr = std::find_if(children.begin(), children.end(),
[&](const std::unique_ptr<IRemoteObject>& lhs) {
		return lhs.get() == child.ptr();
	});
	if (itr != children.end()) {
		std::unique_ptr<IRemoteObject> childObject = std::move(*itr);
		children.erase(itr);
		return childObject;
	}
	return nullptr;
}

template<typename RuntimeType>
void IRemoteInstance<RuntimeType>::reparent(Reference<IRemoteObject> remoteObject) {
	parent = remoteObject;
}

template<typename RuntimeType>
void IRemoteInstance<RuntimeType>::add_child(std::unique_ptr<IRemoteObject> child) {
	if (!child) {
		return;
	}
	child->reparent(this);
	children.emplace_back(std::move(child));
}
