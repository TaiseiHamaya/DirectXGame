#pragma once

#include <ImNodeFlow/include/ImNodeFlow.h>

#include <Library/Math/Vector2.h>

#include "Engine/Debug/Editor/Adapter/EditorValueField.h"

class WorldLayerRenderImNode : public ImFlow::BaseNode {
public:
	WorldLayerRenderImNode(u64 outPinId_);
	~WorldLayerRenderImNode() override = default;

public:
	void draw() override;

private:
	u64 outPinId;

	struct Viewport {
		EditorValueField<r32> left{ "Left" };
		EditorValueField<r32> top{ "Top" };
		EditorValueField<r32> width{ "Width" };
		EditorValueField<r32> height{ "Height" };
		EditorValueField<r32> minDepth{ "MinDepth" };
		EditorValueField<r32> maxDepth{ "MaxDepth" };
	};
	struct Scissor {
		EditorValueField<Vector2> topLeft{ "TopLeft" };
		EditorValueField<Vector2> bottomRight{ "BottomRight" };
	};

	Viewport gBufferViewport;
	EditorValueField<Vector2> gBufferSize{ "GBufferSize" };
	EditorValueField<u32> worldIndex{ "WorldIndex" };
	EditorValueField<u32> layerIndex{ "LayerIndex" };
	EditorValueField<u32> cameraId{ "CameraId" };
	EditorValueField<Vector2> outputSize{ "OutputSize" };
	Scissor layerScissor;
};
