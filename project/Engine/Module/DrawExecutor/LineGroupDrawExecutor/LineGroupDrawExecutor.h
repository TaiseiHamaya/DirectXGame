#pragma once

#ifdef _DEBUG

#include <memory>
#include <string>

#include "Engine/Rendering/DirectX/DirectXResourceObject/StructuredBuffer/StructuredBuffer.h"
#include "Engine/Resources/Debug/LineGroup/LineGroupResource.h"
#include <Engine/Utility/Tools/ConstructorMacro.h>

#include <Library/Math/Matrix4x4.h>

class LineGroupDrawExecutor {
public:
	LineGroupDrawExecutor() = default;
	virtual ~LineGroupDrawExecutor() = default;

	LineGroupDrawExecutor(const std::string& lineGroupName, uint32_t maxInstance);
	__NON_COPYABLE_CLASS(LineGroupDrawExecutor)

public:
	void reinitialize(const std::string& lineGroupName, uint32_t maxInstance);
	void draw_command(size_t InstanceCount) const;
	void write_to_buffer(uint32_t index, const Matrix4x4& worldMatrix);

private:
	std::shared_ptr<const LineGroupResource> lineGroup;
	StructuredBuffer<Matrix4x4> matrices;
};

#endif // _DEBUG
