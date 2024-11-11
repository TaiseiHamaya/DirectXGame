#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Library/Math/Vector3.h"

#include <Engine/Module/World/Mesh/MeshInstance.h>

class Rail {
public:
	struct RailPoint {
		Vector3 position;
		std::optional<float> upwardAngle;
#ifdef _DEBUG
		std::unique_ptr<MeshInstance> debugDrawObj; // デバッグ表示用モデル
#endif // _DEBUG
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

private:
	void create_rail_point(const Vector3& position, const std::optional<float>& upwardAngle = std::nullopt);
	void create_rail();

#ifdef _DEBUG
public:
	void debug_draw();
#endif // _DEBUG

private:
	std::vector<RailPoint> railPoints;

	std::vector<MeshInstance> railDrawMesh;
	float railLength;

public:
	static void LoadMesh();
};
