#pragma once

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

class Affine;
class WorldInstance;

class Hierarchy final {
public:
	Hierarchy(Reference<const Affine> selfAffine_);
	~Hierarchy() = default;

	__CLASS_NON_COPYABLE(Hierarchy)

public:
	void set_parent(const WorldInstance& instance) noexcept;
	void reset_parent() noexcept;
	const Reference<const WorldInstance>& get_parent() const noexcept;
	bool has_parent() const noexcept;
	const Affine& parent_affine() const;
	const Affine& parent_matrix_safe() const noexcept;

private:
	Reference<const WorldInstance> parent;
	Reference<const Affine> selfAffine;
};
