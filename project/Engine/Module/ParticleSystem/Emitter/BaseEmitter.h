#pragma once

#include "Engine/Module/WorldInstance/WorldInstance.h"

class Particle;

class BaseEmitter : public WorldInstance {
public: // Constructor/Destructor
	BaseEmitter() = default;
	virtual ~BaseEmitter() = default;

public: // Member function
	virtual void initialize() {};
	virtual void update();

	virtual void on_emit(Particle* const particle) {};
	virtual void restart() = 0;

public: // Getter/Setter
	uint32_t num_emits() const { return numEmits; };
	bool is_emit() const { return isEmit; };
	bool is_end() const;

protected: // Member variable
	bool isLoop;
	bool isEmit;
	float timer;
	float duration;
	uint32_t numEmits;
};
