#pragma once

#include <memory>
#include <string>

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

#ifdef _DEBUG
#include <Engine/Module/World/Mesh/MeshInstance.h>
#endif // _DEBUG

class BaseCollider : public WorldInstance {
public:
	BaseCollider();
	~BaseCollider() = default;

public:
	virtual void initialize() = 0;
	virtual void update();

public:
	virtual constexpr std::string type() const = 0;
	const std::string& group() const noexcept;

public:
	void set_group_name(const std::string& name);

private:
	const std::string* groupName;

#ifdef _DEBUG
public:
	MeshInstance& get_collider_drawer() const;

protected:
	std::unique_ptr<MeshInstance> colliderDrawer;
#endif // _DEBUG
};

/*
Colliderに関する仕様メモ

CallBack関数を設定すると、衝突時にbindしたが呼び出される
Enter, Exitが設定されていなく衝突した場合、OnCollisionが呼び出される

この判定はColliderインスタンスごとに判定が行われる

*/