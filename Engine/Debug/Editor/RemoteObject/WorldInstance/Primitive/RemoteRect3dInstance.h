#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "../IRemoteInstance.h"

#include <string>

#include <Library/Math/Color4.h>
#include <Library/Math/Transform2D.h>

#include "Engine/Module/World/Mesh/Primitive/Rect3d.h"

class RemoteRect3dInstance final : public IRemoteInstance<Rect3d, Rect3d> {
public:
	friend class EditorSceneSerializer;

	struct Material {
		std::string texture{ "Error.png" };
		EditorValueField<Color4> color{ "Color", CColor4::WHITE };
		EditorValueField<Transform2D> uvTransform{ "UV Transform" };
		LighingType lightingType{ LighingType::None };
		EditorValueField<r32> shininess{ "Shininess", 50 };
	};

public:
	RemoteRect3dInstance() = default;
	~RemoteRect3dInstance() = default;

	__CLASS_DEFAULT_ALL(RemoteRect3dInstance)

public:
	void setup() override;

	void update_preview(Reference<RemoteWorldObject> world, Reference<Affine> parentAffine) override;

	void draw_inspector() override;

	nlohmann::json serialize() const override;

	void on_spawn() override;

	void on_destroy() override;

	constexpr InstanceType instance_type() const { return InstanceType::Rect3dInstance; }

private:
	void reset_material();

private:
	EditorValueField<bool> isDraw{ "IsDraw", true };
	EditorValueField<u32> layer{ "Layer", 0 };

	EditorValueField<Vector2> size{ "Size", CVector2::ONE };
	EditorValueField<Vector2> pivot{ "Pivot", CVector2::ZERO };
	EditorValueField<bool> isFlipY{ "IsFlipY", false };

	Material material;
};

#endif // DEBUG_FEATURES_ENABLE
