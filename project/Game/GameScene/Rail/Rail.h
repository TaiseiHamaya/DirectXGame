#pragma once

#include <optional>
#include <string>
#include <vector>
#include <memory>

#include "Library/Math/Vector3.h"

#include <Engine/Module/World/Mesh/MeshInstance.h>

class Rail {
public:
	struct RailPoint {
		Vector3 position;
		std::optional<float> upwardAngle;
		float minSpeed;
		float maxSpeed;
#ifdef _DEBUG
		std::unique_ptr<MeshInstance> debugDrawObj; // デバッグ表示用モデル
#endif // _DEBUG
	};

	struct RailMesh {
		std::unique_ptr<MeshInstance> mesh;
		float minSpeed;
		float maxSpeed;
	};

public:
	Rail() = default;
	~Rail() = default;

	Rail(const Rail&) = delete;
	Rail& operator=(const Rail&) = delete;

public:
	void initialize();
	void load_rail(const std::string& filename);

	void begin_rendering();
	void draw() const;

public:
	float rail_length() const;
	void transform_from_mileage(WorldInstance& worldInstance, float mileage) const;
	void update_speed_from_mileage(float& speed, float mileage) const;

private:
	void create_rail_point(const Vector3& position, float min, float max, const std::optional<float>& upwardAngle = std::nullopt);
	void create_rail();

	float parametric_from_mileage(float mileage) const;

#ifdef _DEBUG
public:
	void debug_draw();
#endif // _DEBUG

private:
	std::vector<RailPoint> railPoints;

	std::vector<RailMesh> railDrawMesh;
	float railLength;

public:
	static void LoadMesh();
};
