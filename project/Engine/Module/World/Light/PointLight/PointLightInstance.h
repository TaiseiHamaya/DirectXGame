#pragma once

#include "../BaseLightInstance.h"

#include "Library/Math/Color.h"

struct PointLightData {
	Color color; // 色
	Vector3 position; // 位置
	float intensity{ 1 }; // 輝度
	float radius{ 1 }; // 範囲
	float decay{ 0 }; // 減衰率
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
	void begin_rendering() override;

#ifdef _DEBUG
public:
	void debug_gui() override;
#endif // _DEBUG
};
