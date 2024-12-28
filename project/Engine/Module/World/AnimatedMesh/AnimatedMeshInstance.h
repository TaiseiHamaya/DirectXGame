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

private:
	using MeshInstance::reset_mesh;

public:
	AnimatedMeshInstance() noexcept(false);

	/// <summary>
	/// 引数付きコンストラクタ
	/// </summary>
	/// <param name="meshName">メッシュ名</param>
	/// <param name="animationName">アニメーション名</param>
	/// <param name="isLoop">ループするかどうか</param>
	explicit AnimatedMeshInstance(const std::string& meshName, const std::string& animationName = "", bool isLoop = false);
	virtual ~AnimatedMeshInstance() noexcept;

	__NON_COPYABLE_CLASS(AnimatedMeshInstance)

public:
	void begin();
	void begin_rendering() noexcept override;
	void draw() const override;

public:
	void reset_animated_mesh(const std::string& meshName, const std::string& animationName = "", bool isLoop = false);
	NodeAnimationPlayer* const get_animation();

#ifdef _DEBUG
public:
	void debug_gui() override;
	void draw_skeleton();
#endif // _DEBUG

protected:
	std::unique_ptr<NodeAnimationPlayer> nodeAnimation; // NodeAnimation
	
private:
	std::shared_ptr<const SkeletonResource> skeletonResrouce; // Skeleton関連

	SkeletonData skeletonData; // SkeletonのTransform関連

	std::vector<MeshInstance> boneMeshTest; // デバッグ用
};
