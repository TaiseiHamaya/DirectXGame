#include "Rect3d.h"

#include <Library/Math/VectorConverter.h>

Rect3d::Rect3d() noexcept :
	IPrimitiveInstance() {
}

Rect3d::~Rect3d() noexcept = default;

void Rect3d::initialize(const Vector2& size_, const Vector2& pivot_) {
	data.size = size_;
	data.pivot = pivot_;
}

void Rect3d::initialize(const Vector3& size_, const Vector3& pivot_) {
	data.size = Converter::ToVector2(size_);
	data.pivot = Converter::ToVector2(pivot_);
}

//#ifdef DEBUG_FEATURES_ENABLE
//
//void Rect3d::debug_gui() {
//	IPrimitiveInstance<RectData>::debug_gui();
//
//	constexpr r32 FLOAT_MAX{ std::numeric_limits<r32>::max() };
//	ImGuiSliderFlags flag =
//		ImGuiSliderFlags_AlwaysClamp;
//	ImGui::Separator();
//	ImGui::DragFloat2("Size", &data.size.x, 0.1f, 0.0f, FLOAT_MAX, "%.3f", flag);
//	ImGui::DragFloat2("Pivot", &data.pivot.x, 0.01f);
//	ImGui::Checkbox("IsFlipY", &data.isFlipY);
//}
//
//#endif // DEBUG_FEATURES_ENABLE
