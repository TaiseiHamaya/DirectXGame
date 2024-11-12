#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <Engine/Module/World/Mesh/MeshInstance.h>
#include <Library/Math/Vector3.h>

#ifdef _DEBUG
#include "./RailEditor/RailEditor.h"
#endif // _DEBUG

struct RailPoint {
	Vector3 position;
	std::optional<float> upwardAngle;
	float minSpeed;
	float maxSpeed;
#ifdef _DEBUG
	std::unique_ptr<MeshInstance> debugDrawObj; // デバッグ表示用モデル
#endif // _DEBUG
};

class Rail {
	friend class RailEditor;
public:

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

	void begin_rendering();
	void draw() const;

public:
	float rail_length() const;
	void transform_from_mileage(WorldInstance& worldInstance, float mileage) const;
	void update_speed_from_mileage(float& speed, float mileage) const;

private:
	void load_rail();
	void create_rail_point(const Vector3& position, float min, float max, const std::optional<float>& upwardAngle = std::nullopt);
	void apply_rail_point();
	void create_rail();

#ifdef _DEBUG
public:
	void debug_draw();
	void editor_gui();
#endif // _DEBUG

private:
	std::vector<RailPoint> railPoints;

	std::vector<RailMesh> railDrawMesh;
	float railLength;

#ifdef _DEBUG
	std::unique_ptr<RailEditor> editor;
#endif // _DEBUG

private:
	static const inline std::filesystem::path LoadPath
	{ "./Resources/GameScene/Stage/" };

public:
	static void LoadMesh();
};
