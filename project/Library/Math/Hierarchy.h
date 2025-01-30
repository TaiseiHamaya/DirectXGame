#pragma once

class Affine;
class WorldInstance;

#include <Library/Utility/Template/Reference.h>

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
	void initialize(const Affine& worldMatrix) noexcept;

public:
	void set_parent(const WorldInstance& hierarchy) noexcept;
	void reset_parent() noexcept;
	const Reference<const WorldInstance>& get_parent() const noexcept;
	//const WorldInstance* get_parent_address() const noexcept;
	bool has_parent() const noexcept;
	const Affine& parent_affine() const;
	const Affine& parent_matrix_safe() const noexcept;

private:
	Reference<const WorldInstance> parent;
	Reference<const Affine> currentMatrix;
};
