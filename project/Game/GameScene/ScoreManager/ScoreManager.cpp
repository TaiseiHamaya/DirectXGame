#include "ScoreManager.h"

#include <format>

#include <Engine/Runtime/WorldClock/WorldClock.h>
#include <Engine/Utility/Tools/SmartPointer.h>
#include <Library/Math/Definition.h>

#include "Game/GameScene/Enemy/BaseEnemy.h"

#include <Engine/Application/EngineSettings.h>

void ScoreManager::initialize(const WorldInstance* camera_) {
	score.clear();
	displayTimer = 0;
	camera = camera_;
}

void ScoreManager::begin() {
	displayTimer -= WorldClock::DeltaSeconds();
}

void ScoreManager::update() {
}

void ScoreManager::begin_rendering() noexcept {
}

void ScoreManager::late_update() {
	if (!score.empty() && displayTimer <= 0) {
		score.pop_front();
		displayTimer = DefaultDisplayTime;
	}
	if (!score.empty()) {
		auto& drawScore = score.front();
		// 回転させる
		Quaternion rotation = drawScore.model->get_transform().get_quaternion();
		Quaternion displacement = Quaternion::AngleAxis(CVector3::BASIS_Y, 1 * ToRadian);
		drawScore.model->get_transform().set_quaternion(rotation * displacement);
		// 描画準備
		drawScore.model->begin_rendering();
		for (const std::unique_ptr<SpriteInstance>& sprite : drawScore.scoreSprite) {
			sprite->begin_rendering();
		}
	}
}

void ScoreManager::draw() const {
	if (!score.empty()) {
		score.front().model->draw();
	}
}

void ScoreManager::draw_sprite() const {
	if (!score.empty()) {
		for (const std::unique_ptr<SpriteInstance>& sprite : score.front().scoreSprite) {
			sprite->draw();
		}
	}
}

void ScoreManager::register_enemy(BaseEnemy* enemy) {
	if (!enemy) {
		return;
	}
	if (score.empty()) {
		displayTimer = DefaultDisplayTime;
	}
	// 要素追加
	auto& newScore = score.emplace_back();
	// モデル設定
	newScore.model = eps::CreateUnique<MeshInstance>(enemy->use_model_name());
	newScore.model->initialize();
	if (camera) {
		newScore.model->set_parent(*camera);
	}
	newScore.model->get_transform().set_translate({ 1.2f,-0.5f, 4.0f});
	// スコア描画設定
	int scoreValue = enemy->get_score();
	std::string scoreStr = std::to_string(scoreValue);
	newScore.scoreSprite.resize(scoreStr.size());

	constexpr float SpriteWidth = 128 / 2;
	Vector2 center{ EngineSettings::CLIENT_SIZE.x - 512 / 3, 0 };
	Vector2 left = center - Vector2{ SpriteWidth * scoreStr.size() / 2,0.0f };
	for (int i = 0; std::unique_ptr<SpriteInstance>&sprite : newScore.scoreSprite) {
		sprite = eps::CreateUnique<SpriteInstance>(
			std::format("score_sprite_{}.png", scoreStr[i]),
			Vector2{ 0.0f,0.0f }
		);
		sprite->get_transform().set_scale({ 0.5f, 0.5f });
		sprite->get_transform().set_translate(left + Vector2{i* SpriteWidth, 0.0f});
		++i;
	}
}
