#pragma once

#include "Engine/DirectX/DirectXResourceObject/BufferObjects.h"
#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"

class Material final : public ConstantBuffer<MaterialData> {
public:
	Material();
	~Material() = default;

	Material(const MaterialData& material);

public:
	Color& get_color_reference() const;
	void set_uv_transform(Matrix4x4&& uvTransform);
};
