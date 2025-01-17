#pragma once

#include <string>

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

class BaseCollider : public WorldInstance {
public:
	BaseCollider();
	~BaseCollider() = default;

public:
	virtual void update();

public:
	const std::string& group() const noexcept;

public:
	void set_group_name(const std::string& name);

private:
	const std::string* groupName;
};

/*
Colliderに関する仕様メモ

CallBack関数を設定すると、衝突時にbindしたが呼び出される
Enter, Exitが設定されていなく衝突した場合、OnCollisionが呼び出される

この判定はColliderインスタンスごとに判定が行われる

*/