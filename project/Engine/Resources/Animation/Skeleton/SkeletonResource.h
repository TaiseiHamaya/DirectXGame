#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "Engine/Utility/Tools/ConstructorMacro.h"

struct Joint {
	std::string name; // BoneName(== NodeName)
	std::optional<uint32_t> parent;
	std::vector<uint32_t> children;
};

struct Skeleton {
	Joint* root;
	std::vector<Joint> joints;
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

	const Skeleton* skeleton(const std::string& bindMeshName) const;

private:
	std::unordered_map<std::string, Skeleton> skeletons;
};
