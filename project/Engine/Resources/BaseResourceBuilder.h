#pragma once

#include <filesystem>

#include "Engine/Utility/Tools/ConstructorMacro.h"

class BaseResourceBuilder {
public:
	BaseResourceBuilder() = default;
	virtual ~BaseResourceBuilder() = default;

	__NON_COPYMOVEABLE_CLASS(BaseResourceBuilder)

public:
	/// <summary>
	/// Run on "RegisterLoadQue"
	/// </summary>
	virtual void preprocess() = 0;

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
	/// Transfer resource manager
	/// </summary>
	virtual void transfer() = 0;

public:
	const std::filesystem::path& filepath() { return filePath; };

protected:
	std::filesystem::path filePath;
};
