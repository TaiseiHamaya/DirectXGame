#include "Material.h"

Material::Material() : ConstantBuffer<MaterialData>() {
}

Material::Material(const MaterialData& material) : ConstantBuffer<MaterialData>(material) {
}

Color* const Material::get_color() const {
	return &data->color;
}

void Material::set_uv_transform(Matrix4x4&& uvTransform) {
	data->uvTransform = uvTransform;
}
