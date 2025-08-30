#include "WorldCluster.h"

void WorldCluster::initialize() {
	worldRoot.initialize();
	worldRenderCollection.initialize();
}

void WorldCluster::setup([[maybe_unused]] const std::filesystem::path& setupFile) {
	worldRoot.setup(instanceBucket);
}

void WorldCluster::update() {
	worldRoot.update();
	
	worldRoot.update_affine();
}

void WorldCluster::pre_draw() {
	worldRenderCollection.transfer();

	worldRoot.post_update();
}

void WorldCluster::end_frame() {
	// ---------- 削除予定インスタンス処理 ----------
	// 描画に関して
	worldRenderCollection.remove_marked_destroy();
	// コリジョン
	collisionManager.remove_marked_destroy();
	// 実際の削除
	worldRoot.delete_marked_destroy();
	
	// ---------- Instantiate後の処理 ----------
	// 描画が側に伝達
	worldRenderCollection.collect_instantiated(instanceBucket);
	instanceBucket.reset();
}

Reference<WorldRenderCollection> WorldCluster::render_collection() {
	return worldRenderCollection;
}
