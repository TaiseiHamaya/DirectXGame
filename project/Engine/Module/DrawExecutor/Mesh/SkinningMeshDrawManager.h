#pragma once

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "./SkinningMeshDrawExecutor.h"
#include "Engine/Module/World/Mesh/SkinningMeshInstance.h"

#ifdef _DEBUG
class Camera3D;
#endif // _DEBUG

class SkinningMeshDrawManager final {
private:
	struct Data {
		std::unordered_set<Reference<const SkinningMeshInstance>> instances;
		// key : meshID
		std::unordered_map<std::string, SkinningMeshDrawExecutor> executors;

		Data() = default;
		Data(Data&&) noexcept = default;
	};

public:
	SkinningMeshDrawManager() = default;
	virtual ~SkinningMeshDrawManager() = default;

	__NON_COPYABLE_CLASS(SkinningMeshDrawManager)

public:
	void initialize(uint32_t numLayer);
	void make_instancing(uint32_t layer, const std::string& meshName, uint32_t maxInstance);
	void register_instance(Reference<const SkinningMeshInstance> instance);
	void transfer();
	void draw_layer(uint32_t layer);

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

private:
	std::vector<Data> drawData;

#ifdef _DEBUG
	uint32_t layer;
	std::string selectMesh;
	uint32_t maxInstance;
#endif // _DEBUG
};

