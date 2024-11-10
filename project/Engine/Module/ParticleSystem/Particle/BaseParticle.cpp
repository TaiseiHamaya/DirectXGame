#include "BaseParticle.h"

const Matrix4x4 BaseParticle::create_uv_matrix() const {
	return uvTransform.get_matrix4x4_padding();
}
