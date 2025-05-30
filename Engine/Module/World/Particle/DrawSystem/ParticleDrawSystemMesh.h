#pragma once

#include "./BaseParticleDrawSystem.h"

#include <string>

class TextureAsset;
class PolygonMesh;

class ParticleDrawSystemMesh final : public BaseParticleDrawSystem {
public:
	ParticleDrawSystemMesh(const std::string& meshName);
	~ParticleDrawSystemMesh() = default;

public:
	void draw_command(size_t InstanceCount) const override;

	void set_mesh(const std::string& meshName);

private:
	std::shared_ptr<const PolygonMesh> mesh;
	std::shared_ptr<const TextureAsset> texture;
};
