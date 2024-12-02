#pragma once

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

#include "Library/Math/Color4.h"
#include "Library/Math/Transform2D.h"

class BaseParticle : public WorldInstance {
public: // Constructor/Destructor
	BaseParticle() = default;
	virtual ~BaseParticle() = default;

	BaseParticle(const BaseParticle&) = delete;
	BaseParticle& operator=(const BaseParticle&) = delete;
	BaseParticle(BaseParticle&&) = delete;
	BaseParticle& operator=(BaseParticle&&) = delete;

public: // Member function
	virtual void update() {};

public: // Getter/Setter
	const Color4& get_color() const { return color; };
	const Transform2D& uv_transform() const { return uvTransform; };
	bool is_destroy() const { return isDestroy; };

	const Matrix4x4 create_uv_matrix() const;

protected: // Member variable
	bool isDestroy = false;

	Color4 color;
	Transform2D uvTransform;
};
