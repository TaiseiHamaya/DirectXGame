#pragma once

#include "../BaseLightInstance.h"

#include "Library/Math/Color.h"

struct PointLightData {
	Color color; // 色
	Vector3 direction{ -CVector3::BASIS_Y }; // 向き
	float intensity{ 1 }; // 輝度
	float radius; // 範囲
	float decay; // 減衰率
};

class PointLightInstance : public BaseLightInstance<PointLightData> {
};
