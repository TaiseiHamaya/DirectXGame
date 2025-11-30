#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Module/Manager/SceneScript/SceneScriptManager.h"
#include "Engine/Module/Manager/World/WorldCluster.h"
#include "Engine/Module/Render/RenderDAG.h"

class Scene {
public:
	Scene() = default;
	virtual ~Scene() = default;

	__CLASS_NON_COPYABLE(Scene)

public:
	virtual void load_asset();

	void initialize();

	void setup();

	virtual void custom_setup() {};

	void begin_frame();

	void update();

	void pre_draw();

	void draw() const;

	void end_frame();

	void finalize();

public:
	Reference<WorldCluster> get_world(u32 index);

	void set_name(const std::string& name);
	std::string_view name() const noexcept;

protected:
	std::string sceneName;
	SceneScriptManager sceneScriptManager;

private:
	std::vector<std::unique_ptr<WorldCluster>> worlds;
	RenderDAG renderDAG;
};
