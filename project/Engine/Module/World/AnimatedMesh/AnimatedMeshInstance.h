#pragma once

#include "Engine/Utility/Tools/ConstructorMacro.h"

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

class AnimatedMeshInstance : public WorldInstance {
public:
	AnimatedMeshInstance() noexcept(false) = default;
	//explicit AnimatedMeshInstance(const std::string& meshName_) noexcept(false);
	virtual ~AnimatedMeshInstance() noexcept = default;

	__NON_COPYABLE_CLASS(AnimatedMeshInstance)

public:
};

