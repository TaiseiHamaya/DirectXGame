#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <Library/Utility/Tools/ConstructorMacro.h>
#include <Library/Utility/Template/Reference.h>

#include <string>

class IRemoteObject {
public:
	IRemoteObject() = default;
	virtual ~IRemoteObject() = default;

	__CLASS_DEFAULT_ALL(IRemoteObject)

public:
	virtual void draw_inspector() = 0;

	virtual void draw_hierarchy(Reference<IRemoteObject>& select) = 0;

	virtual void delete_hierarchy() {};

protected:
	bool isOpen{ false };
	std::string hierarchyName;
};

#endif // DEBUG_FEATURES_ENABLE
