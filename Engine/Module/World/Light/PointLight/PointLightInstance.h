#pragma once

#include "../BaseLightInstance.h"

#include <Library/Math/Color3.h>

struct PointLightData {
	Color3 color; // 色
	r32 intensity{ 1 }; // 輝度
	
	Vector3 position; // 位置
	r32 radius{ 1 }; // 範囲

	r32 decay{ 0 }; // 減衰率
};

class PointLightInstance : public BaseLightInstance<PointLightData> {
public:
	PointLightInstance() = default;
	virtual ~PointLightInstance() = default;

	PointLightInstance(const PointLightInstance&) = delete;
	PointLightInstance& operator=(const PointLightInstance&&) = delete;
	PointLightInstance(PointLightInstance&&) = default;
	PointLightInstance& operator=(PointLightInstance&&) = default;

public:
	void update_affine() override;
	void transfer() override;

public:
	Matrix4x4 transform_matrix() const;
};
