#include "Material.h"

#include <utility>

Material::Material() noexcept(false) : ConstantBuffer<MaterialData>() {
}

Material::Material(const MaterialData& material) noexcept(false) : ConstantBuffer<MaterialData>(material) {
}

Color3& Material::get_color_reference() const noexcept {
	return data->color;
}

void Material::set_uv_transform(Matrix4x4&& uvTransform) noexcept {
	data->uvTransform = std::move(uvTransform);
}

void Material::set_lighting(LighingType lighting_) {
	data->lighting = static_cast<std::uint32_t>(lighting_);
}
