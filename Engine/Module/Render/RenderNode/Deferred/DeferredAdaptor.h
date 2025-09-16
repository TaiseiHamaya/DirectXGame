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
constexpr const u32 NUM_GBUFFER = 2;

/// <summary>
/// MRTの型
/// </summary>
using GBuffersType = MultiRenderTarget<NUM_GBUFFER>;

/// <summary>
/// DXGI_FORMATの一覧
/// </summary>
constexpr const std::array<DXGI_FORMAT, NUM_GBUFFER> DXGI_FORMAT_LIST = {
	DXGI_FORMAT_R10G10B10A2_UNORM,
	DXGI_FORMAT_R32_UINT,
};
};
