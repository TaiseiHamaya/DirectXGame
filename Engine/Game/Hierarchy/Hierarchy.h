#pragma once

class Matrix4x4;

class Hierarchy final {
public:
	Hierarchy() = default;
	~Hierarchy() = default;

public:// コピー禁止、ムーブ許可
	Hierarchy(const Hierarchy&) = delete;
	Hierarchy& operator=(const Hierarchy&) = delete;
	Hierarchy(Hierarchy&&) = default;
	Hierarchy& operator=(Hierarchy&&) = default;

public:
	void initialize(const Matrix4x4& worldMatrix) noexcept;

public:
	void set_parent(const Hierarchy& hierarchy) noexcept;
	const Hierarchy& get_parent() const noexcept;
	bool has_parent() const noexcept;
	const Matrix4x4& parent_matrix() const noexcept;

private:
	const Hierarchy* parent;
	const Matrix4x4* currentMatrix;
};
