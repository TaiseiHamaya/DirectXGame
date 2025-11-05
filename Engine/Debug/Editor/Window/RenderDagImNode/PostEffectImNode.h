#pragma once

#include <ImNodeFlow/include/ImNodeFlow.h>

#include <Library/Math/Vector2.h>

#include "Engine/Debug/Editor/Adapter/EditorValueField.h"

#include <string>
#include <vector>

class PostEffectImNode : public ImFlow::BaseNode {
public:
	PostEffectImNode(u64 outPinId_);
	~PostEffectImNode() override = default;

public:
	void draw() override;

private:
	u64 outPinId;

	EditorValueField<u32> peType{ "Type" };
	EditorValueField<Vector2> outputSize{ "OutputSize" };

private:
	static std::vector<std::string> peName;
};
