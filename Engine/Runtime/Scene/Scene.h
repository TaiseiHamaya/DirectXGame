#pragma once

#include <string>
#include <vector>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Module/Manager/World/WorldCluster.h"
#include "Engine/Module/Render/RenderDAG.h"

class Scene {
public:
	Scene() = default;
	~Scene() = default;

	__CLASS_NON_COPYABLE(Scene)

public:
	void load_asset();

	void initialize();

	void setup();

	virtual void custom_setup() {};

	void update();

	void pre_draw();

	void draw() const;

	void end_frame();

public:
	Reference<WorldCluster> get_world(u32 index);

	void set_name(const std::string& name);
	std::string_view name() const noexcept;

protected:
	std::string sceneName;

private:
	std::vector<WorldCluster> worlds;
	RenderDAG renderDAG;
};
