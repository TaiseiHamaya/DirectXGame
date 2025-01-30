#pragma once

#include <memory>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/StructuredBuffer/StructuredBuffer.h"
#include "Engine/Module/World/Mesh/StaticMeshInstance.h"

class NodeAnimationPlayer;
class SkeletonAsset;

class SkinningMeshInstance : public StaticMeshInstance {
private:
	struct SkeletonMatrixPaletteWell {
		Matrix4x4 skeletonSpaceMatrix;
		Matrix4x4 skeletonSpaceInv;
	};

	struct SkeletonSpaceInstance {
		Transform3D transform;
		Affine affine;
	};

	struct SkeletonData {
		std::vector<SkeletonSpaceInstance> jointInstance; // JointのTransformとMatrix
		std::vector<StructuredBuffer<SkeletonMatrixPaletteWell>> matrixPalettes; // GPU用Matrix
	};

private:
	using StaticMeshInstance::reset_mesh;

public:
	SkinningMeshInstance() noexcept(false);

	/// <summary>
	/// 引数付きコンストラクタ
	/// </summary>
	/// <param name="meshName">メッシュ名</param>
	/// <param name="animationName">アニメーション名</param>
	/// <param name="isLoop">ループするかどうか</param>
	explicit SkinningMeshInstance(const std::string& meshName, const std::string& animationName = "", bool isLoop = false);
	virtual ~SkinningMeshInstance() noexcept;

	__NON_COPYABLE_CLASS(SkinningMeshInstance)

public:
	virtual void begin() override;
	virtual void transfer() noexcept override;
	virtual void draw() const override;

public:
	void reset_animated_mesh(const std::string& meshName, const std::string& animationName = "", bool isLoop = false);
	NodeAnimationPlayer* const get_animation();

private:
	void create_skeleton();

#ifdef _DEBUG
public:
	void debug_gui() override;
	void draw_skeleton();
#endif // _DEBUG

protected:
	std::unique_ptr<NodeAnimationPlayer> nodeAnimation; // NodeAnimation

private:
	std::shared_ptr<const SkeletonAsset> skeletonResrouce; // Skeleton関連

	SkeletonData skeletonData; // SkeletonのTransform関連

	std::vector<StaticMeshInstance> boneMeshTest; // デバッグ用
};
