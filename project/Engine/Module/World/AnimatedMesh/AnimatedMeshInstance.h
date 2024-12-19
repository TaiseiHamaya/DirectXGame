#pragma once

#include <memory>

#include "Engine/Utility/Tools/ConstructorMacro.h"

#include "Engine/Module/World/Mesh/MeshInstance.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/StructuredBuffer/StructuredBuffer.h"

class NodeAnimationPlayer;
class SkeletonResource;

class AnimatedMeshInstance : public MeshInstance {
private:
	struct SkeletonMatrixPaletteWell {
		Matrix4x4 skeletonSpaceMatrix;
		Matrix4x4 skeletonSpaceInv;
	};

	struct SkeletonSpaceInstance {
		Transform3D transform;
		Matrix4x4 matrix;
	};

	struct SkeletonData {
		std::vector<SkeletonSpaceInstance> jointInstance; // JointのTransformとMatrix
		std::vector<StructuredBuffer<SkeletonMatrixPaletteWell>> matrixPalettes; // GPU用Matrix
	};

public:
	AnimatedMeshInstance() noexcept(false);

	/// <summary>
	/// 引数付きコンストラクタ
	/// </summary>
	/// <param name="file">メッシュファイル名</param>
	/// <param name="animationName">アニメーション名</param>
	/// <param name="isLoop">ループするかどうか</param>
	explicit AnimatedMeshInstance(const std::string& file, const std::string& animationName = "", bool isLoop = false) noexcept(false);
	virtual ~AnimatedMeshInstance() noexcept = default;

	__NON_COPYABLE_CLASS(AnimatedMeshInstance)

public:
	void begin();
	void begin_rendering() noexcept override;
	void draw() const override;

#ifdef _DEBUG
public:
	void debug_gui() override;
	void draw_skeleton();
#endif // _DEBUG

private:
	std::unique_ptr<NodeAnimationPlayer> nodeAnimation; // NodeAnimation
	std::shared_ptr<const SkeletonResource> skeletonResrouce; // Skeleton関連

	SkeletonData skeletonData; // SkeletonのTransform関連

	std::vector<MeshInstance> boneMeshTest; // デバッグ用
};
