#pragma once

#include "Engine/Module/ParticleSystem/Emitter/BaseEmitter.h"

class EmitterSample final : public BaseEmitter {
public: // Constructor/Destructor
	EmitterSample() = default;
	~EmitterSample() = default;

public:
	void initialize() override;
	void update() override;
	void restart() override;
	void on_emit(BaseParticle* const particle) override;
};
