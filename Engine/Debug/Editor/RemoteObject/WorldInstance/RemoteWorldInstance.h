#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "./IRemoteInstance.h"

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

namespace szg {

class RemoteWorldInstance final : public IRemoteInstance<WorldInstance> {
public:
	friend class EditorSceneSerializer;

public:
	RemoteWorldInstance();
	~RemoteWorldInstance();

	SZG_CLASS_DEFAULT(RemoteWorldInstance)

public:
	void draw_inspector() override;

	nlohmann::json serialize() const override;

	constexpr InstanceType instance_type() const { return InstanceType::WorldInstance; }
};

}; // szg

#endif // DEBUG_FEATURES_ENABLE
