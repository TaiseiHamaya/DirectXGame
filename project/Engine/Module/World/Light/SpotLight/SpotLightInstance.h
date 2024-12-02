#pragma once

#include "../BaseLightInstance.h"

#include "Library/Math/Color.h"

struct SpotLightData {
	Color color; // 色
	Vector3 position; // 位置
	float intensity{ 1 }; // 輝度
	Vector3 direction{ CVector3::DOWN }; // 向き
	float distance; // ライト距離
	float decay; // 距離減衰率
	float angle; // ライト範囲
	float falloffStart; // ライト
};

class SpotLightInstance : public BaseLightInstance<SpotLightData> {
public:
	SpotLightInstance() = default;
	virtual ~SpotLightInstance() = default;

	SpotLightInstance(const SpotLightInstance&) = delete;
	SpotLightInstance& operator=(const SpotLightInstance&&) = delete;
	SpotLightInstance(SpotLightInstance&&) = default;
	SpotLightInstance& operator=(SpotLightInstance&&) = default;

public:
	void begin_rendering() override;

#ifdef _DEBUG
public:
	void debug_gui() override;
#endif // _DEBUG
};

