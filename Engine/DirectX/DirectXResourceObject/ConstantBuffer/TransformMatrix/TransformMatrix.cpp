#include "TransformMatrix.h"

TransformMatrix::TransformMatrix() : ConstantBuffer<TransformationMatrixData>() {
}

TransformMatrix::~TransformMatrix() noexcept = default;

TransformMatrix::TransformMatrix(const TransformationMatrixData& transformMatrixData)
	: ConstantBuffer<TransformationMatrixData>(transformMatrixData) {
}

void TransformMatrix::set_transformation_matrix_data(Matrix4x4&& world, const Matrix4x4& cameraVP) noexcept {
	data->world = std::move(world);
	data->wvp = data->world * cameraVP;
}
