#pragma once

#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h"

enum class LighingType {
	None,
	Lambert,
	HalfLambert,
};

class Material final : public ConstantBuffer<MaterialBufferData> {
public:
	Material() noexcept(false);
	~Material() noexcept = default;

	Material(const MaterialBufferData& material) noexcept(false);

	Material(Material&&) = default;
	Material& operator=(Material&&) = default;

public:
	Color3& get_color_reference() const noexcept;
	void set_uv_transform(Matrix4x4&& uvTransform) noexcept;
	void set_lighting(LighingType lighting_);
};
