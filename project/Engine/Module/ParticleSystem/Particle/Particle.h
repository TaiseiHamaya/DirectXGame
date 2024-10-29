#pragma once

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

#include "Library/Math/Color.h"

#include <memory>

class BaseParticleMovements;

class Particle final : public WorldInstance {
public: // Constructor/Destructor
	Particle(std::unique_ptr<BaseParticleMovements>&& movements_);
	~Particle() = default;

	Particle(const Particle&) = delete;
	Particle& operator=(const Particle&) = delete;
	Particle(Particle&&) = delete;
	Particle& operator=(Particle&&) = delete;

public: // Member function
	void initialize() override;
	void update();

public: // Getter/Setter
	void set_color(const Color& color_) { color = color_; };
	const Color& get_color() const { return color; };
	void set_destroy() { isDestroy = true; };

	bool is_destroy() const { return isDestroy; };

private: // Member variable
	bool isDestroy = false;

	Color color;
	std::unique_ptr<BaseParticleMovements> movements;
};
