#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IRenderDagImNode.h"

#include <Library/Math/Vector2.h>

#include "Engine/Application/ProjectSettings/ProjectSettings.h"
#include "Engine/Debug/Editor/Adapter/EditorValueField.h"

#include <string>
#include <vector>

class PostEffectImNode : public IRenderDagImNode {
public:
	PostEffectImNode();
	~PostEffectImNode() override = default;

public:
	struct Data {
		EditorValueField<u32> peType{ "Type" };
		EditorValueField<Vector2> outputSize{ "OutputSize", ProjectSettings::ClientSize() };
		EditorValueField<bool> isUseRuntime{ "UseRuntime", false };
		EditorValueField<std::string> effectTagName{ "EffectTag", "" };
		std::vector<u64> extraInputs;
	};

public:
	void draw() override;

	void remove_extra_input_pins();
	void update_extra_input_pins();

public:
	const Data& get_data() const;
	void set_data(const Data& data_);

private:
	Data data;

private:
	static std::vector<std::string> peName;
};

#endif // DEBUG_FEATURES_ENABLE
