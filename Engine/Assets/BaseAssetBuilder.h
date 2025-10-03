#pragma once

#include <filesystem>

#include "Library/Utility/Tools/ConstructorMacro.h"

class BaseAssetBuilder {
public:
	BaseAssetBuilder() = default;
	virtual ~BaseAssetBuilder() = default;

	__CLASS_NON_COPYMOVEABLE(BaseAssetBuilder)

public:
	/// <summary>
	/// Run on default timing
	/// </summary>
	/// <returns>Succeed / Failed</returns>
	virtual bool run() = 0;

	/// <summary>
	/// Run on before "Transfer"
	/// </summary>
	virtual void postprocess() = 0;

	/// <summary>
	/// Transfer asset library
	/// </summary>
	virtual void transfer() = 0;

public:
	const std::filesystem::path& filepath() { return filePath; };

public:
	static std::filesystem::path ResolveFilePath(const std::filesystem::path& filePath, const std::string& subPath = "");

protected:
	std::filesystem::path filePath;
};
