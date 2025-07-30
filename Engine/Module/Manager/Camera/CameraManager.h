#pragma once

#include <vector>

#include <Library/Utility/Tools/ConstructorMacro.h>
#include <Library/Utility/Template/Reference.h>

#include "Engine/Module/World/Camera/Camera3D.h"

class CameraManager final {
public:
	CameraManager() = default;
	~CameraManager() noexcept = default;

	__CLASS_NON_COPYABLE(CameraManager)

public:
	void register_camera(Reference<Camera3D> cameraInstance);

private:
	std::vector<Reference<Camera3D>> cameras;
};
