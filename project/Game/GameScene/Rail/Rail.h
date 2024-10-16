#pragma once

#include <string>
#include <vector>
#include <optional>

#include "Engine/Math/Vector3.h"

#include <Engine/Module/GameObject/GameObject.h>

class Rail {
public:
	struct RailPoint {
		Vector3 position;
		std::optional<float> upwardAngle;
#ifdef _DEBUG
		std::unique_ptr<GameObject> debugDrawObj; // デバッグ表示用モデル
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

#ifdef _DEBUG
public:
	void debug_draw();
#endif // _DEBUG

private:
	void create_rail_point(const Vector3& position, const std::optional<float>& upwardAngle = std::nullopt);
	void create_rail();

private:
	std::vector<RailPoint> railPoints;

	std::vector<GameObject> railDrawMesh;

public:
	static void LoadMesh();
};
