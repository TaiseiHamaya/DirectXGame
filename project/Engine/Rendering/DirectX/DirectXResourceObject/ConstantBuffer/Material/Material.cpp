#include "Material.h"

#include <utility>

Material::Material() noexcept(false) : ConstantBuffer<MaterialBufferData>() {
}

Material::Material(const MaterialBufferData& material) noexcept(false) : ConstantBuffer<MaterialBufferData>(material) {
}

Color3& Material::get_color_reference() const noexcept {
	return data->color;
}

void Material::set_uv_transform(Matrix4x4&& uvTransform) noexcept {
	data->uvTransformMatrix = std::move(uvTransform);
}

void Material::set_lighting(LighingType lighting_) {
	data->lighting = static_cast<std::uint32_t>(lighting_);
}
