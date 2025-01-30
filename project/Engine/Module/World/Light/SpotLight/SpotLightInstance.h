#pragma once

#include "../BaseLightInstance.h"

#include <Library/Math/Color3.h>

struct SpotLightData {
	Color3 color; // 色
	float intensity{ 1 }; // 輝度

	Vector3 position; // 位置
	float distance; // ライト距離
	
	Vector3 direction{ CVector3::DOWN }; // 向き
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
	void transfer() override;

#ifdef _DEBUG
public:
	void debug_gui() override;
#endif // _DEBUG
};

