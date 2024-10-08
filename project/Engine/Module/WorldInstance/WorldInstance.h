#pragma once

#include "Engine/Module/Transform3D/Transform3D.h"
#include "Engine/Module/Hierarchy/Hierarchy.h"

class WorldInstance {
public:
	WorldInstance() = default;
	virtual ~WorldInstance() = default;

	WorldInstance(const WorldInstance&) = delete;
	WorldInstance& operator=(const WorldInstance&&) = delete;
	WorldInstance(WorldInstance&&) = default;
	WorldInstance& operator=(WorldInstance&&) = default;

public:
	virtual void initialize();
	void update_matrix();

private:
	Matrix4x4 create_world_matrix() const;

public:
	void set_active(bool isActive);
	bool is_active() const { return isActive; };
	const Hierarchy& get_hierarchy() const;
	Hierarchy& get_hierarchy();
	const Transform3D& get_transform() const;
	Transform3D& get_transform();
	const Matrix4x4& world_matrix() const;
	Vector3 world_position() const;
	void set_parent(const Hierarchy& hierarchy_);

#ifdef _DEBUG
public:
	virtual void debug_gui();
#endif // _DEBUG

protected:
	Transform3D transform{};
	Hierarchy hierarchy{};
	Matrix4x4 worldMatrix = CMatrix4x4::IDENTITY;
	bool isActive = true;
};

