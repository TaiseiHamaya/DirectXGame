#pragma once

enum class RenderNodeType {
	WorldLayer = 0,
	PostEffect,
	StaticTexture,
	Result,
	
	Max,

	Unknown = -1,
};

enum class PostEffectType {
	None,
	ChromaticAberration,
	Grayscale,
	Outline,
	RadialBlur,
	DownSampling,
	TextureBlend2,
	TextureBlend4,
	LuminanceExtraction,
	GaussianBlur,
	Bloom,
};
