#pragma once

#include <functional>
#include <memory>
#include <type_traits>
#include <vector>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h"
#include "Engine/Module/DrawExecutor/Mesh/SkinningMeshDrawManager.h"
#include "Engine/Module/DrawExecutor/Mesh/StaticMeshDrawManager.h"
#include "Engine/Module/Render/RenderPath/RenderPath.h"
#include "Engine/Module/Render/RenderTargetGroup/SingleRenderTarget.h"
#include "Engine/Module/World/WorldInstance/WorldInstance.h"

class StaticMeshInstance;
class SkinningMeshInstance;

class WorldRoot {
private:
	struct InstanceData {
		Reference<WorldInstance> instance;
		std::function<void()> destroyCallback;
	};

public:
	WorldRoot();
	~WorldRoot();

	__CLASS_NON_COPYABLE(WorldRoot)

public:
	void initialize();
	void update();
	void transfer();
	void draw();

	void post_update();

	template<std::derived_from<WorldInstance> T, typename ...Args>
	Reference<T> create(Reference<const WorldInstance> parent = nullptr, Args&&... args);

	void destroy(Reference<WorldInstance> instance);

private:
	u64 nextInstanceId = 0;
	std::unordered_map<u64, std::unique_ptr<InstanceData>> worldInstances;

	std::vector<Reference<WorldInstance>> destroyInstances;

	RenderTexture renderTexture;
	SingleRenderTarget renderTarget;
	RenderPath renderPath;
	StaticMeshDrawManager staticMeshDrawManager;
	SkinningMeshDrawManager skinningMeshDrawManager;
};

template<std::derived_from<WorldInstance> T, typename ...Args>
inline Reference<T> WorldRoot::create(Reference<const WorldInstance> parent, Args&&... args) {
	std::unique_ptr<T> instance = T::Insantiate(nextInstanceId, parent, std::forward<Args>(args)...);
	++nextInstanceId;



	//std::unique_ptr<T> instance = std::make_unique<T>(args...);
	//u64 instanceId = nextInstanceId;
	//++nextInstanceId;

	//if (parent) {
	//	instance->reparent(parent, false);
	//}
	//else {
	//	instance->reparent(nullptr, false);
	//}

	//if constexpr (std::is_same_v<T, StaticMeshInstance>) {
	//	staticMeshDrawManager.register_instance(instance);
	//}
	//else if constexpr (std::is_same_v<T, SkinningMeshInstance>) {
	//	skinningMeshDrawManager.register_instance(instance);
	//}



	//Reference<WorldInstance> result = instance;
	//worldInstances.emplace(
	//	result,
	//	std::move(instance),
	//	[&, instanceId, result]() {
	//	if constexpr (std::is_same_v<T, StaticMeshInstance>) {
	//		staticMeshDrawManager.unregister_instance(instance);
	//	}
	//	else if constexpr (std::is_same_v<T, SkinningMeshInstance>) {
	//		skinningMeshDrawManager.unregister_instance(instance);
	//	}
	//	worldInstances.erase(instanceId);
	//}
	//);

	//return result;
}
