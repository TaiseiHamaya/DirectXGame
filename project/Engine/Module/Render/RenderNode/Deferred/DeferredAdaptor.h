#pragma once

#include <memory>

#include <Library/Utility/Tools/SmartPointer.h>

#include "Engine/Application/EngineSettings.h"
#include "Engine/Module/Render/RenderTargetGroup/MultiRenderTarget.h"
#include "Engine/Module/Render/RenderTargetGroup/SingleRenderTarget.h"

namespace DeferredAdaptor {

/// <summary>
/// G-Bufferの数
/// </summary>
constexpr const uint32_t NUM_GBUFFER = 2;

/// <summary>
/// MRTの型
/// </summary>
using GBuffersType = MultiRenderTarget<NUM_GBUFFER>;

/// <summary>
/// DXGI_FORMATの一覧
/// </summary>
constexpr const std::array<DXGI_FORMAT, NUM_GBUFFER> DXGI_FORMAT_LIST = {
	DXGI_FORMAT_R10G10B10A2_UNORM,
	DXGI_FORMAT_R11G11B10_FLOAT
};

/// <summary>
/// Deferred用RTの生成関数
/// </summary>
/// <param name="width">幅</param>
/// <param name="height">高さ</param>
/// <returns>initialize済みGBuffer</returns>
inline std::shared_ptr<GBuffersType> CreateGBufferRenderTarget(
	std::uint32_t width = EngineSettings::CLIENT_WIDTH, std::uint32_t height = EngineSettings::CLIENT_HEIGHT
) {
	auto result = eps::CreateShared<GBuffersType>();
	result->initialize(
		width, height, DXGI_FORMAT_LIST
	);

	return result;
}

inline std::shared_ptr<SingleRenderTarget> CreateLightingTargetTexture(
	std::uint32_t width = EngineSettings::CLIENT_WIDTH, std::uint32_t height = EngineSettings::CLIENT_HEIGHT
) {
	auto result = eps::CreateShared<SingleRenderTarget>();
	result->initialize(
		width, height
	);

	return result;
}

};
