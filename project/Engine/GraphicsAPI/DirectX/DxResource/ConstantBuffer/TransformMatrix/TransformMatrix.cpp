#include "TransformMatrix.h"

#include <Library/Math/Affine.h>

TransformMatrix::TransformMatrix() : ConstantBuffer<TransformMatrixData>() {}

TransformMatrix::~TransformMatrix() noexcept = default;

void TransformMatrix::set_transformation_matrix_data(const Affine& world) noexcept {
	Basis wIT = world.inverse().get_basis().transposed();
	*data = { world.to_matrix(), wIT.to_matrix4x4() };
}
