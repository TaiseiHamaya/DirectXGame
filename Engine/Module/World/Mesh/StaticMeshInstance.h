#pragma once

#include "./IMultiMeshInstance.h"

#include <string>

namespace szg {

class Material;
class PolygonMesh;
class Texture;
class TransformMatrix;

class StaticMeshInstance : public IMultiMeshInstance {
public:
	friend class RemoteStaticMeshInstance;
	friend class RemoteSkinningMeshInstance;

public:
	StaticMeshInstance() noexcept;
	explicit StaticMeshInstance(const std::string& meshName_) noexcept(false);
	virtual ~StaticMeshInstance() noexcept;

	SZG_CLASS_MOVE_ONLY(StaticMeshInstance)

public:
	void reset_mesh(const std::string& meshName_);

	/// <summary>
	/// Texture、Materialパラメータ、UVデータのリセットを行う
	/// </summary>
	void default_material();
};

}; // szg
