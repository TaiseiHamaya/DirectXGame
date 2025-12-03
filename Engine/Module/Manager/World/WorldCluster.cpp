#include "WorldCluster.h"

#include "Engine/Assets/Json/JsonAsset.h"
#include "Engine/Loader/WorldInstanceLoader.h"

void WorldCluster::initialize() {
	worldRoot.initialize();
	worldRenderCollection.initialize();
	collisionManager.set_callback_manager(std::make_unique<CollisionCallbackManager>());
}

void WorldCluster::setup([[maybe_unused]] const std::filesystem::path& setupFile) {
	worldRoot.setup(instanceBucket);
	JsonAsset json{ setupFile };
	WorldInstanceLoader loader;
	loader.setup(worldRoot);
	if (json.cget().contains("Instances") && json.cget()["Instances"].is_array()) {
		for (const nlohmann::json& instanceJson : json.cget()["Instances"]) {
			loader.entry_point(instanceJson, nullptr);
		}
	}

	u8 numLayer= json.cget().value<u8>("NumLayer", 0);
	worldRenderCollection.setup(numLayer);
}

void WorldCluster::begin_frame() {
	// ---------- Instantiate後の処理 ----------
	// 描画が側に伝達
	worldRenderCollection.collect_instantiated(instanceBucket);
	instanceBucket.reset();
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
}

WorldRoot& WorldCluster::world_root_mut() {
	return worldRoot;
}

Reference<WorldRenderCollection> WorldCluster::render_collection() {
	return worldRenderCollection;
}
