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

protected:
	Reference<RuntimeType> self;

	std::vector<std::unique_ptr<IRemoteObject>> children;
	Transform3D transform;
};
