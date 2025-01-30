#include "MultiRenderTargetNode.h"

//#include <d3d12.h>
//
//#include "Engine/Render/RenderTargetGroup/MultiRenderTarget.h"
//#include "Engine/DirectX/DxResource/OffscreenRender/OffscreenRender.h"
//
//MultiRenderTargetNode::MultiRenderTargetNode() = default;
//
//MultiRenderTargetNode::~MultiRenderTargetNode() noexcept = default;
//
//void MultiRenderTargetNode::set_render_target(const std::shared_ptr<MultiRenderTarget>& renderTarget_) {
//	renderTarget = renderTarget_;
//	auto&& list = renderTarget_->offscreen_render_list();
//	for (auto itr = list.end(); itr != list.end(); ++itr) {
//		resultSrvHandleList.emplace_back(itr->texture_gpu_handle());
//	}
//}
//
//const std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>& MultiRenderTargetNode::result_stv_handle_list() const {
//	return resultSrvHandleList;
//}
