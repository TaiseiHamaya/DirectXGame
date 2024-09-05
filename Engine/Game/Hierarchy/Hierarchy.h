#pragma once

class GameObject;
class Matrix4x4;
class TransformMatrix;

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
	void initialize(const TransformMatrix& transformMatrix);

public:
	void set_parent(const GameObject& gameObject) noexcept;
	const Hierarchy& get_parent() const noexcept;
	bool has_parent() const noexcept;
	const Matrix4x4& parent_matrix() const;

private:
	const Hierarchy* parent;
	const Matrix4x4* currentMatrix;
};
