#pragma once

#include "Engine/DirectX/DirectXResourceObject/BufferObjects.h"
#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"

class Material final : public ConstantBuffer<MaterialData> {
public:
	Material() noexcept(false);
	~Material() noexcept = default;

	Material(const MaterialData& material) noexcept(false);

public:
	Color& get_color_reference() const noexcept;
	void set_uv_transform(Matrix4x4&& uvTransform) noexcept;
};
