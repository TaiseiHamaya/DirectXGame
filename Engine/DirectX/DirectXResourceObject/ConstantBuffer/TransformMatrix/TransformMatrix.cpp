#include "TransformMatrix.h"

TransformMatrix::TransformMatrix() : ConstantBuffer<TransformationMatrixData>() {
}

TransformMatrix::TransformMatrix(const TransformationMatrixData& transformMatrixData)
	: ConstantBuffer<TransformationMatrixData>(transformMatrixData) {
}

void TransformMatrix::set_transformation_matrix_data(Matrix4x4&& world, Matrix4x4&& wvp) {
	data->world = std::move(world);
	data->wvp = std::move(wvp);
}
