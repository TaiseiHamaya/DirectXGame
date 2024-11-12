#include "ScoreManager.h"

#include <format>

#include <Engine/Runtime/WorldClock/WorldClock.h>
#include <Engine/Utility/Tools/SmartPointer.h>
#include <Library/Math/Definition.h>

#include "Game/GameScene/Enemy/BaseEnemy.h"

#include <Engine/Application/EngineSettings.h>

void ScoreManager::initialize(const WorldInstance* camera_) {
	scoreDisplay.clear();
	displayTimer = 0;
	camera = camera_;
	score = 0;
	create_score_sprite();
}

void ScoreManager::begin() {
	displayTimer -= WorldClock::DeltaSeconds();
}

void ScoreManager::update() {
}

void ScoreManager::begin_rendering() noexcept {
}

void ScoreManager::late_update() {
	if (!scoreDisplay.empty() && displayTimer <= 0) {
		scoreDisplay.pop_front();
		displayTimer = DefaultDisplayTime;
	}
	if (!scoreDisplay.empty()) {
		auto& drawScore = scoreDisplay.front();
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
	for (auto& sprite : scoreSprite) {
		sprite->begin_rendering();
	}
}

void ScoreManager::draw() const {
	if (!scoreDisplay.empty()) {
		scoreDisplay.front().model->draw();
	}
}

void ScoreManager::draw_sprite() const {
	if (!scoreDisplay.empty()) {
		for (const std::unique_ptr<SpriteInstance>& sprite : scoreDisplay.front().scoreSprite) {
			sprite->draw();
		}
	}
	for (const auto& sprite : scoreSprite) {
		sprite->draw();
	}
}

void ScoreManager::register_enemy(BaseEnemy* enemy) {
	if (!enemy) {
		return;
	}
	if (scoreDisplay.empty()) {
		displayTimer = DefaultDisplayTime;
	}
	// 要素追加
	auto& newScore = scoreDisplay.emplace_back();
	// モデル設定
	newScore.model = eps::CreateUnique<MeshInstance>(enemy->use_model_name());
	newScore.model->initialize();
	if (camera) {
		newScore.model->set_parent(*camera);
	}
	newScore.model->get_transform().set_translate({ 1.2f,-0.3f, 4.0f });
	//スコア加算
	int scoreValue = enemy->get_score();
	score += scoreValue;
	// スコア描画設定
	std::string scoreStr = std::to_string(scoreValue);
	newScore.scoreSprite.resize(scoreStr.size());

	constexpr float SpriteWidth = 128 / 2;
	Vector2 center{ EngineSettings::CLIENT_SIZE.x - 512 / 3, 128 };
	Vector2 left = center - Vector2{ SpriteWidth * scoreStr.size() / 2,0.0f };
	for (int i = 0; std::unique_ptr<SpriteInstance>&sprite : newScore.scoreSprite) {
		sprite = eps::CreateUnique<SpriteInstance>(
			std::format("score_sprite_{}.png", scoreStr[i]),
			Vector2{ 0.0f,0.0f }
		);
		sprite->get_transform().set_scale({ 0.5f, 0.5f });
		sprite->get_transform().set_translate(left + Vector2{ i * SpriteWidth, 0.0f });
		++i;
	}

	create_score_sprite();
}

void ScoreManager::create_score_sprite() {
	constexpr float SpriteWidth = 128 / 2;
	// 合計スコア描画設定
	std::string accumulateScoreStr = std::to_string(score);
	Vector2 accumulateCenter{ EngineSettings::CLIENT_SIZE.x - 512 / 3, 0};
	Vector2 accumulateLeft = accumulateCenter - Vector2{ SpriteWidth * accumulateScoreStr.size() / 2,0.0f };;
	if (scoreSprite.size() < accumulateScoreStr.size()) {
		scoreSprite.resize(accumulateScoreStr.size());
	}
	for (int i = 0; auto & sprite : scoreSprite) {
		sprite = eps::CreateUnique<SpriteInstance>(
			std::format("score_sprite_{}.png", accumulateScoreStr[i]),
			Vector2{ 0.0f,0.0f }
		);
		sprite->get_transform().set_scale({ 0.5f, 0.5f });
		sprite->get_transform().set_translate(accumulateLeft + Vector2{ i * SpriteWidth, 0.0f });
		++i;
	}
}
