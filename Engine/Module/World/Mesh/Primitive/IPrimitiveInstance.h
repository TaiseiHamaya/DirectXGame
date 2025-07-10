#pragma once

#include <memory>

#include <Library/Math/Color4.h>
#include <Library/Math/Transform2D.h>

#include "../IDrawInstance.h"
#include "Engine/Assets/Texture/TextureAsset.h"
#include "Engine/Assets/Texture/TextureLibrary.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"

enum class PrimitiveType {
	Rect3D,
	Circle,
	Cylinder,
};

struct PrimitiveMaterial {
	std::shared_ptr<const TextureAsset> texture;
	Color4 color;
	Transform2D uvTransform;
	LighingType lightingType{ LighingType::HalfLambert };
	r32 shininess{ 50 };
};

template<typename PrimitiveData>
class IPrimitiveInstance : public IDrawInstance<PrimitiveType> {
public:
	IPrimitiveInstance() noexcept;
	virtual ~IPrimitiveInstance() noexcept = default;

	__CLASS_NON_COPYABLE(IPrimitiveInstance)

public:
	PrimitiveMaterial& get_material() { return material; }
	const PrimitiveMaterial& get_material() const { return material; }
	PrimitiveData& get_data() { return data; }
	const PrimitiveData& get_data() const { return data; }

protected:
	PrimitiveData data;
	PrimitiveMaterial material;
};

template<typename PrimitiveData>
inline IPrimitiveInstance<PrimitiveData>::IPrimitiveInstance() noexcept :
	IDrawInstance() {
}

//#ifdef DEBUG_FEATURES_ENABLE
//template<typename PrimitiveData>
//inline void IPrimitiveInstance<PrimitiveData>::debug_gui() {
//	IDrawInstance<PrimitiveType>::debug_gui();
//	ImGui::Separator();
//	TextureLibrary::TextureListGui(material.texture);
//
//	material.uvTransform.debug_gui();
//
//	material.color.debug_gui();
//
//	if (ImGui::RadioButton("None", material.lightingType == LighingType::None)) {
//		material.lightingType = LighingType::None;
//	}
//	ImGui::SameLine();
//	if (ImGui::RadioButton("Lambert", material.lightingType == LighingType::Lambert)) {
//		material.lightingType = LighingType::Lambert;
//	}
//	ImGui::SameLine();
//	if (ImGui::RadioButton("Half lambert", material.lightingType == LighingType::HalfLambert)) {
//		material.lightingType = LighingType::HalfLambert;
//	}
//
//	ImGui::DragFloat("Shininess", &material.shininess, 0.1f, 0.0f, std::numeric_limits<r32>::max());
//}
//#endif // DEBUG_FEATURES_ENABLE
