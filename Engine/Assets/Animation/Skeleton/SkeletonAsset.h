#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "Engine/GraphicsAPI/DirectX/DxResource/VertexBuffer/VertexBuffer.h"

#include <Library/Math/Affine.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

struct Joint {
	std::string name; // BoneName(== NodeName)
	Affine inverseBindPoseAffine;
	std::optional<u32> parent;
	std::vector<u32> children;
};

struct VertexInfluenceData {
	inline static constexpr u32 NumMaxInfluence = 4;
	std::array<r32, NumMaxInfluence> weights;
	std::array<u32, NumMaxInfluence> jointIndex;
};

struct Skeleton {
	std::vector<Joint> joints;
	std::unordered_map<std::string, std::vector<u32>> useJointIndexesByMeshName;
};

class SkeletonAsset {
public:
	SkeletonAsset(Skeleton& skeleton_, std::unordered_map<std::string, VertexBuffer<VertexInfluenceData>>& influenceBuffers_);
	~SkeletonAsset() = default;

	__CLASS_NON_COPYMOVEABLE(SkeletonAsset)

public:
	const Skeleton& skeleton() const;
	const VertexBuffer<VertexInfluenceData>* weight_influence(const std::string& bindMeshName) const;
	const std::vector<u32>* use_joint_indexes(const std::string& bindMeshName) const;
	u32 joint_size() const;

private:
	Skeleton skeletonData;

	/// <summary>
	/// Key : Mesh名
	/// Value : VertexBuffer
	/// </summary>
	std::unordered_map<std::string, VertexBuffer<VertexInfluenceData>> influenceBuffers;
};
