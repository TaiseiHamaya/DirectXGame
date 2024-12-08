#pragma once

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

#include "Library/Math/Color4.h"
#include "Library/Math/Transform2D.h"

class Particle final : public WorldInstance {
public: // Constructor/Destructor
	Particle(
		const Vector3& translate,
		float lifetime_, 
		const Vector3& velocity_, const Vector3& acceleration_,
		const Color4& startColor_, const Color4& endColor_,
		const Vector3& startSize_, const Vector3& endSize_
	);
	~Particle() = default;

	Particle(const Particle&) = delete;
	Particle& operator=(const Particle&) = delete;
	Particle(Particle&&) = delete;
	Particle& operator=(Particle&&) = delete;

public: // Member function
	void update();

public: // Getter/Setter
	const Color4& get_color() const { return color; };
	bool is_destroy() const { return isDestroy; };

	const Matrix4x4 create_uv_matrix() const;

protected: // Member variable
	bool isDestroy = false;

	float timer;
	float lifetime;

	Vector3 velocity;
	Vector3 acceleration;

	Color4 color;
	Color4 startColor;
	Color4 endColor;

	Vector3 startSize;
	Vector3 endSize;
	Transform2D uvTransform;
};
