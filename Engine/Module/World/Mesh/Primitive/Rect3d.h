#pragma once

#include "./IPrimitiveInstance.h"

struct RectData {
	Vector2 size{ CVector2::BASIS };
	Vector2 pivot{ CVector2::ZERO };
	bool isFlipY{ false };
};

class Rect3d : public IPrimitiveInstance<RectData> {
#ifdef DEBUG_FEATURES_ENABLE
	friend class RemoteRect3dInstance;
#endif // DEBUG_FEATURES_ENABLE

public:

public:
	Rect3d() noexcept;
	virtual ~Rect3d() noexcept;

	__CLASS_NON_COPYABLE(Rect3d)

public:
	void initialize(const Vector2& size_, const Vector2& pivot_ = CVector2::ZERO);
	void initialize(const Vector3& size_, const Vector3& pivot_ = CVector3::ZERO);

public:
	void set_flip_y(bool flag);

protected:
};
