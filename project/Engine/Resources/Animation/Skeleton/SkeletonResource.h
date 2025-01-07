#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "Engine/Rendering/DirectX/DirectXResourceObject/VertexBuffer/VertexBuffer.h"
#include "Engine/Utility/Tools/ConstructorMacro.h"

#include <Library/Math/Affine.h>

struct Joint {
	std::string name; // BoneName(== NodeName)
	Affine inverseBindPoseAffine;
	std::optional<uint32_t> parent;
	std::vector<uint32_t> children;
};

struct VertexInfluenceData {
	inline static constexpr uint32_t NumMaxInfluence = 4;
	std::array<float, NumMaxInfluence> weights;
	std::array<uint32_t, NumMaxInfluence> jointIndex;
};

struct Skeleton {
	std::vector<Joint> joints;
	std::unordered_map<std::string, std::vector<uint32_t>> useJointIndexesByMeshName;
};

class SkeletonResource {
public:

public:
	SkeletonResource() = default;
	~SkeletonResource() = default;

	__NON_COPYMOVEABLE_CLASS(SkeletonResource)

public:
	/// <summary>
	/// ロード関数
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>成功値</returns>
	bool load(const std::filesystem::path& filePath);

	const Skeleton& skeleton() const;
	const VertexBuffer<VertexInfluenceData>* weight_influence(const std::string& bindMeshName) const;
	const std::vector<uint32_t>* use_joint_indexes(const std::string& bindMeshName) const;
	uint32_t joint_size() const;

private:
	Skeleton skeletonData;

	/// <summary>
	/// Key : Mesh名
	/// Value : VertexBuffer
	/// </summary>
	std::unordered_map<std::string, VertexBuffer<VertexInfluenceData>> influenceBuffers;
};
