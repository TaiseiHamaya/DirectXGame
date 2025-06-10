#pragma once

#include "../IRemoteObject.h"

#include <Library/Math/Transform3D.h>

template<typename RuntimeType>
class IRemoteInstance : public IRemoteObject {
public:
	IRemoteInstance() = default;
	virtual ~IRemoteInstance() = default;

	__CLASS_DEFAULT_ALL(IRemoteInstance)

public:


protected:
	Reference<RuntimeType> self;

	std::vector<std::unique_ptr<IRemoteObject>> children;
	Transform3D transform;
};
