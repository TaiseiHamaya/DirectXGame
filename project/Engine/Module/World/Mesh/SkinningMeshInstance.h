#pragma once

#include <memory>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Module/World/Mesh/StaticMeshInstance.h"

class NodeAnimationPlayer;
class SkeletonAsset;

class SkinningMeshInstance : public StaticMeshInstance {
public:
	struct SkeletonSpaceInstance {
		Transform3D transform;
		Affine affine;
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

	void update_animation();

public:
	void reset_animated_mesh(const std::string& meshName, const std::string& animationName = "", bool isLoop = false);
	void reset_animation(const std::string& fileName, const std::string& animationName = "", bool isLoop = false);
	NodeAnimationPlayer* const get_animation();
	const std::vector<SkeletonSpaceInstance>& joints() const { return jointInstances; }
	bool is_draw() const override;

private:
	void create_skeleton();

#ifdef _DEBUG
public:
	void debug_gui() override;
#endif // _DEBUG

protected:
	std::unique_ptr<NodeAnimationPlayer> nodeAnimation; // NodeAnimation

private:
	std::vector<SkeletonSpaceInstance> jointInstances; // JointのTransformとMatrix
	std::shared_ptr<const SkeletonAsset> skeletonResrouce; // Skeleton関連

#ifdef _DEBUG
	std::vector<StaticMeshInstance> boneMeshTest; // デバッグ用
#endif // _DEBUG
};
