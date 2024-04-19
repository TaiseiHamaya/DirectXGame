#pragma once

#include <cstdint>
#include <array>

template<typename T, size_t ROW, size_t COLUMN>
class Array2D {
public:
	Array2D();

public:
	const size_t row() const { return data_.size(); }
	const size_t column() const { return data_.front().size(); }
	std::array<T, COLUMN>& operator[](size_t index) { return data_[index]; }
	const std::array<T, COLUMN>& operator[](size_t index) const { return data_[index]; }
	bool operator==(const Array2D<T, ROW, COLUMN>& rhs) const { return data_ == rhs.data_; }
	bool operator!=(const Array2D<T, ROW, COLUMN>& rhs) const { return !(*this == rhs); }

private:
	std::array<std::array<T, COLUMN>, ROW> data_;
};

template<typename T, size_t ROW, size_t COLUMN>
inline Array2D<T, ROW, COLUMN>::Array2D() {
	data_ = {0};
}
