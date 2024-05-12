#pragma once

#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"
#include "Engine/DirectX/DirectXResourceObject/BufferObjects.h"

class Material final : public ConstantBuffer<MaterialData> {
public:
	Material();
	~Material() = default;

	Material(const MaterialData& material);

public:
	Color* const get_color() const;
	void set_uv_transform(Matrix4x4&& uvTransform);
};
