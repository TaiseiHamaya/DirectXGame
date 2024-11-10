#pragma once

#include "../BaseLightInstance.h"

#include "Library/Math/Color.h"

struct DirectionalLightData {
	Color color; // 色
	Vector3 direction{ -CVector3::BASIS_Y }; // 向き
	float intensity{ 1 }; // 輝度
};

class DirectionalLightInstance : public BaseLightInstance<DirectionalLightData> {
public:
	DirectionalLightInstance() = default;
	virtual ~DirectionalLightInstance() = default;

	DirectionalLightInstance(const DirectionalLightInstance&) = delete;
	DirectionalLightInstance& operator=(const DirectionalLightInstance&&) = delete;
	DirectionalLightInstance(DirectionalLightInstance&&) = default;
	DirectionalLightInstance& operator=(DirectionalLightInstance&&) = default;

public:
	void begin_rendering() override;

#ifdef _DEBUG
public:
	void debug_gui() override;
#endif // _DEBUG
};
