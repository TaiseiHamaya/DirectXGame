#pragma once

#include "BaseParticleSystem.h"
#include "Engine/Module/PolygonMesh/PolygonMesh.h"

class ParticleSystemModel final : public BaseParticleSystem {
public:
	ParticleSystemModel() = default;
	virtual ~ParticleSystemModel() = default;

	ParticleSystemModel(const ParticleSystemModel&) = delete;
	ParticleSystemModel& operator=(const ParticleSystemModel&) = delete;
	ParticleSystemModel(ParticleSystemModel&&) = default;
	ParticleSystemModel& operator=(ParticleSystemModel&&) = default;

public:
	void draw() const;

public:
	void set_mesh(const std::string& meshName);

private:
	std::weak_ptr<PolygonMesh> mesh;
	std::weak_ptr<Texture> texture;
};

