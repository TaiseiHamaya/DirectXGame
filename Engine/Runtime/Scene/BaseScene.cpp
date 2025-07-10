#include "BaseScene.h"

#include "Engine/Application/Output.h"

/////////////////////
/// シーンの使い方 ///
////////////////////
/// 追記予定
/*

*/
///
///
/// Update関数内で更新処理、Draw関数内で描画処理を行うこと！！！

void BaseScene::load() {
}

void BaseScene::initialize() {
}

void BaseScene::finalize() {
}

void BaseScene::popped() {
}

void BaseScene::begin() {
}

void BaseScene::begin_rendering() {
}

void BaseScene::late_update() {
}

#ifdef DEBUG_FEATURES_ENABLE
void BaseScene::debug_update() {
}
#endif // _DEBUG

u64 BaseScene::world_size() const {
	return worlds.size();
}

Reference<WorldManager> BaseScene::get_world(u64 index) {
#ifdef DEBUG_FEATURES_ENABLE
	ErrorIf(index >= worlds.size(), "Out of range.");
#endif // DEBUG_FEATURES_ENABLE

	return worlds[index];
}
