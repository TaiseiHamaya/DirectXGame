#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Library/Math/Color3.h>
#include <Library/Math/Transform2D.h>

#include "./IMultiMeshInstance.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"

class Material;
class PolygonMesh;
class Texture;
class TransformMatrix;

class StaticMeshInstance : public IMultiMeshInstance {
public:
	StaticMeshInstance() noexcept;
	explicit StaticMeshInstance(const std::string& meshName_) noexcept(false);
	virtual ~StaticMeshInstance() noexcept;

	__CLASS_NON_COPYABLE(StaticMeshInstance)

public:
	void reset_mesh(const std::string& meshName_);

	/// <summary>
	/// Texture、Materialパラメータ、UVデータのリセットを行う
	/// </summary>
	void default_material();

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui() override;
#endif // _DEBUG
};
