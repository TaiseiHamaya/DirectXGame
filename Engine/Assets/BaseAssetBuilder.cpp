#include "BaseAssetBuilder.h"

std::filesystem::path BaseAssetBuilder::ResolveFilePath(const std::filesystem::path& filePath, const std::string& subPath) {
	// 相対ディレクトリで始まる場合
	if (filePath.native().starts_with(L".\\") || filePath.native().starts_with(L"./")) {
		return filePath;
	}
	else if (filePath.native().starts_with(L"[[szg]]") || filePath.native().starts_with(L"[[SZG]]")) {
		// エンジンリソース
		std::filesystem::path result{ "./DirectXGame/EngineResources/" };
		if (!subPath.empty()) {
			result /= subPath;
		}
		return result / filePath.native().substr(8);
	}
	// ファイル名のみor一部ディレクトリの続きの場合
	else {
		std::filesystem::path result{ "./Game/Resources/" };
		if (!subPath.empty()) {
			result /= subPath;
		}
		return result / filePath;
	}
}
