#pragma once

#include <vector>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

class Camera3D;
class StaticMeshInstance;
class SkinningMeshInstance;
class DirectionalLightInstance;
class PointLightInstance;
class Rect3d;

class InstanceBucket final {
	friend class WorldRenderCollection;

public:
	InstanceBucket() = default;
	~InstanceBucket() = default;

	__CLASS_NON_COPYABLE(InstanceBucket)

public:
	void reset();

	template<typename T>
	void register_instance(Reference<T> instance);

private:
	std::vector<Reference<Camera3D>> camera;
	std::vector<Reference<StaticMeshInstance>> staticMesh;
	std::vector<Reference<SkinningMeshInstance>> skinMesh;
	std::vector<Reference<DirectionalLightInstance>> directionalLightInstance;
	std::vector<Reference<PointLightInstance>> pointLightInstance;
	std::vector<Reference<Rect3d>> rect;
};

template<typename T>
void InstanceBucket::register_instance(Reference<T> instance) {
	if constexpr (std::is_same_v<T, Camera3D>) {
		camera.emplace_back(instance);
	}
	else if constexpr (std::is_same_v<T, StaticMeshInstance>) {
		staticMesh.emplace_back(instance);
	}
	else if constexpr (std::is_same_v<T, SkinningMeshInstance>) {
		skinMesh.emplace_back(instance);
	}
	else if constexpr (std::is_same_v<T, DirectionalLightInstance>) {
		directionalLightInstance.emplace_back(instance);
	}
	else if constexpr (std::is_same_v<T, PointLightInstance>) {
		pointLightInstance.emplace_back(instance);
	}
	else if constexpr (std::is_same_v<T, Rect3d>) {
		rect.emplace_back(instance);
	}
}
