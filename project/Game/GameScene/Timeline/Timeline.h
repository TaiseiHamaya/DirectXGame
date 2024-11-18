#pragma once

#include <filesystem>
#include <list>
#include <string>

#include <Library/Math/Vector3.h>

class EnemyManager;
class EnemyTypeEditor;
class EnemyMovementsEditor;
class RailCamera;

struct PopData {
	float mileage;
	Vector3 translate;
	std::string type;
	std::string movement;
};

class Timeline {
public:
	void initialize(EnemyManager* enemyManager_, RailCamera* railCamera_);
	void update();

public:
	void load_pop_data();

#ifdef _DEBUG
public:
	void initialize_editor(
		const EnemyTypeEditor* typeDatabase,
		const EnemyMovementsEditor* moveDatabase
	);
	void debug_gui();

private:
	void detail_window();
	void export_pop_data();
#endif // _DEBUG

private:
	static const inline std::filesystem::path LoadPath
	{ "./Resources/GameScene/Timeline/" };

	RailCamera* railCamera;

	std::list<PopData> data;
	PopData* detail;

	EnemyManager* enemyManager;

#ifdef _DEBUG
	std::list<PopData> editData;
	const EnemyTypeEditor* typeDatabase;
	const EnemyMovementsEditor* moveDatabase;
#endif // _DEBUG
};
