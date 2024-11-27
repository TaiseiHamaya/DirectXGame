#include "TransformMatrix.h"

TransformMatrix::TransformMatrix() : ConstantBuffer<TransformMatrixData>() {
}

TransformMatrix::~TransformMatrix() noexcept = default;

//TransformMatrix::TransformMatrix(const Matrix4x4& transformMatrixData)
//	: ConstantBuffer<Matrix4x4>(transformMatrixData) {
//}

void TransformMatrix::set_transformation_matrix_data(const Matrix4x4& world) noexcept {
	*data = { world, world.inverse() };
}
