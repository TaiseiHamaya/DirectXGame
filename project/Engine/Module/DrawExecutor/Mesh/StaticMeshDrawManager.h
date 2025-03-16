#pragma once

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "./StaticMeshDrawExecutor.h"
#include "Engine/Module/World/Mesh/StaticMeshInstance.h"

#ifdef DEBUG_FEATURES_ENABLE
class Camera3D;
#endif // _DEBUG

class StaticMeshDrawManager final {
private:
	struct Data {
		std::unordered_set<Reference<const StaticMeshInstance>> instances;
		// key : meshID
		std::unordered_map<std::string, StaticMeshDrawExecutor> executors;

		Data() = default;
		Data(Data&&) noexcept = default;
	};

public:
	StaticMeshDrawManager() = default;
	virtual ~StaticMeshDrawManager() = default;

	__CLASS_NON_COPYABLE(StaticMeshDrawManager)

public:
	void initialize(uint32_t numLayer);
	void make_instancing(uint32_t layer, const std::string& meshName, uint32_t maxInstance);
	void register_instance(Reference<const StaticMeshInstance> instance);
	void transfer();
	void draw_layer(uint32_t layer);

#ifdef DEBUG_FEATURES_ENABLE
public:
	void register_debug_instance(uint32_t layer, Reference<const Camera3D> camera, bool isShowGrid);
	void debug_gui();
#endif // _DEBUG

private:
	std::vector<Data> drawData;

#ifdef DEBUG_FEATURES_ENABLE
	uint32_t layer;
	std::string selectMesh;
	uint32_t maxInstance;
#endif // _DEBUG
};
