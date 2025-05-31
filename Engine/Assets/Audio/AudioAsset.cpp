#include "AudioAsset.h"

#include <array>
#include <cstring>
#include <fstream>

#include "Engine/Application/Output.h"

bool IsEqualArrayChunkId(const std::array<i8, 4>& read, std::string&& id) {
	return std::strncmp(read.data(), id.c_str(), 4) == 0;
}

bool AudioAsset::load(const std::filesystem::path& filePath) {
	// ローカル変数定義
	struct ChunkHeader {
		std::array<i8, 4> id;
		i32 size;
	};
	struct RiffHeader {
		ChunkHeader header;
		std::array<i8, 4> type;
	};
	struct FormatChunk {
		ChunkHeader header;
		WAVEFORMATEXTENSIBLE format;
	};

	Information("Start load .wave file. file-\'{}\'", filePath.string());

	// ファイル読み込み
	std::ifstream file;
	file.open(filePath, std::ios_base::binary);

	// 失敗したら何もしない
	if (!file.is_open()) {
		Error("Failed open file. \'{}\'", filePath.filename().string());
		return false;
	}

	// riff読み込み
	RiffHeader riff;
	file.read((i8*)&riff, sizeof(riff));

	// RIFFじゃなかったらエラー処理
	if (!IsEqualArrayChunkId(riff.header.id, "RIFF")) {
		Error("Failed loading. File \'{}\' don't have RIFF chunk.", filePath.filename().string());
		return false;
	}

	// WAVEフォーマットじゃなかったらエラー処理
	if (!IsEqualArrayChunkId(riff.type, "WAVE")) {
		Error("Failed loading. File \'{}\' is not WAVE format.", filePath.filename().string());
		return false;
	}

	// chunk読み込み
	FormatChunk formatChunk{};
	while (file.read((i8*)&formatChunk.header, sizeof(ChunkHeader))) {
		if (IsEqualArrayChunkId(formatChunk.header.id, "fmt ")) {
			break;
		}
		else if (IsEqualArrayChunkId(formatChunk.header.id, "JUNK")) {
			file.seekg(formatChunk.header.size, std::ios_base::cur);
		}
		else if (IsEqualArrayChunkId(formatChunk.header.id, "LIST")) {
			file.seekg(formatChunk.header.size, std::ios_base::cur);
		}
		else {
			file.seekg(formatChunk.header.size, std::ios_base::cur);
		}
	}
	// chunk.sizeよりformatが小さくないとread時にエラーになる
	if (formatChunk.header.size > sizeof(formatChunk.format)) {
		Error("This fmt chunk format is not support. Size-\'{}\'", formatChunk.header.size);
		return false;
	}

	// 読み込み
	file.read((i8*)&formatChunk.format, formatChunk.header.size);
	// データ読み込み
	ChunkHeader data;
	while (file.read((i8*)&data, sizeof(data))) {
		if (IsEqualArrayChunkId(data.id, "data")) {
			break;
		}
		else if (IsEqualArrayChunkId(data.id, "JUNK")) {
			file.seekg(data.size, std::ios_base::cur);
		}
		else if (IsEqualArrayChunkId(data.id, "LIST")) {
			file.seekg(data.size, std::ios_base::cur);
		}
		else {
			file.seekg(data.size, std::ios_base::cur);
		}
	}

	// dataチャンクがなかったらエラー
	if (!IsEqualArrayChunkId(data.id, "data")) {
		Error("Failed loading. \'data\' chunk is not found. File-\'{}\'", filePath.string());
		return false;
	}

	// 読み込んだデータをコピーして保持
	format_ = formatChunk.format;
	buffer_.resize(data.size);
	file.read((i8*)buffer_.data(), data.size);
	bufferSize_ = data.size;
	file.close();

	Information("Succeeded.");
	return true;
}

u32 AudioAsset::size() const noexcept {
	return bufferSize_;
}

const std::basic_string<BYTE>& AudioAsset::buffer_data() const noexcept {
	return buffer_;
}

const WAVEFORMATEX& AudioAsset::format() const noexcept {
	return format_.Format;
}
