#pragma once

#include <unordered_map>

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
	void set_parent(Reference<WorldInstance> instance) noexcept;

	bool has_parent() const noexcept;
	
	void reset_parent() noexcept;
	
	void add_child(Reference<WorldInstance> instance);

	void remove_child(Reference<WorldInstance> instance) noexcept;
	
	Reference<const WorldInstance> parent_imm() const noexcept;
	Reference<WorldInstance> parent_mut() noexcept;

	const std::unordered_map<u64, Reference<WorldInstance>>& children_imm() const noexcept;
	std::unordered_map<u64, Reference<WorldInstance>>& children_mut() noexcept;

	const Affine& parent_affine() const;
	const Affine& parent_affine_safe() const noexcept;
	

private:
	Reference<WorldInstance> parent;
	std::unordered_map<u64, Reference<WorldInstance>> children;

	Reference<const Affine> selfAffine;
};
