#pragma once

#include "../BaseLightInstance.h"

#include <Library/Math/Color3.h>

struct DirectionalLightData {
	Color3 color; // 色
	float intensity{ 1 }; // 輝度
	Vector3 direction{ -CVector3::BASIS_Y }; // 向き
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
	void transfer() override;

#ifdef _DEBUG
public:
	void debug_gui() override;
#endif // _DEBUG
};
