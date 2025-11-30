#pragma once

#include <memory>
#include <vector>

#include <Library/Utility/Tools/ConstructorMacro.h>

class ISceneScript;

class SceneScriptManager final {
public:
	SceneScriptManager();
	~SceneScriptManager();

	__CLASS_NON_COPYABLE(SceneScriptManager)

public:
	void register_script(std::unique_ptr<ISceneScript> script);

	void prev_update();
	void post_update();

	void finalize();

private:
	std::vector<std::unique_ptr<ISceneScript>> scripts;
};
