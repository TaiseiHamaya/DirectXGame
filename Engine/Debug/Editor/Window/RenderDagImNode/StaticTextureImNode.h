#pragma once

#include <ImNodeFlow/include/ImNodeFlow.h>

#include "Engine/Debug/Editor/Adapter/EditorValueField.h"

#include <string>

class StaticTextureImNode : public ImFlow::BaseNode {
public:
	StaticTextureImNode(u64 outPinId_);
	~StaticTextureImNode() override = default;

public:
	void draw() override;

private:
	u64 outPinId;
	EditorValueField<std::string> textureFilename{ "TextureFilename" };

private:
	static std::vector<std::string> peName;
};
