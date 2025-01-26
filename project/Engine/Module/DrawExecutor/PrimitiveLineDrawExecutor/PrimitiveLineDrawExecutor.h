#pragma once

#ifdef _DEBUG

#include <memory>
#include <string>

#include "Engine/Rendering/DirectX/DirectXResourceObject/StructuredBuffer/StructuredBuffer.h"
#include "Engine/Resources/PrimitiveGeometry/PrimitiveGeometryResource.h"
#include <Engine/Utility/Tools/ConstructorMacro.h>

#include <Library/Math/Matrix4x4.h>

class PrimitiveLineDrawExecutor {
public:
	PrimitiveLineDrawExecutor() = default;
	virtual ~PrimitiveLineDrawExecutor() = default;

	PrimitiveLineDrawExecutor(const std::string& primitiveGeometryName, uint32_t maxInstance);
	__NON_COPYABLE_CLASS(PrimitiveLineDrawExecutor)

public:
	void reinitialize(const std::string& primitiveGeometryName, uint32_t maxInstance);
	void draw_command(size_t InstanceCount) const;
	void write_to_buffer(uint32_t index, const Matrix4x4& worldMatrix);

private:
	std::shared_ptr<const PrimitiveGeometryResource> primitiveGeometry;
	StructuredBuffer<Matrix4x4> matrices;
};

#endif // _DEBUG
