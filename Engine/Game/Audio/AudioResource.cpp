#include "AudioResource.h"

#include <fstream>
#include <cassert>

#include <Engine/Utility/Utility.h>

bool AudioResource::load(const std::string& directoryPath,const std::string& fileName) {
	// ローカル変数定義
	struct ChunkHeader {
		char id[4];
		std::int32_t size;
	};
	struct RiffHeader {
		ChunkHeader chunk;
		char type[4];
	};
	struct FormatChunk {
		ChunkHeader chunk;
		WAVEFORMATEX format;
	};

	Log("[AudioResource] Start load .wave file. file-\'" + directoryPath + "/" + fileName + "\'\n");

	// ファイル読み込み
	std::ifstream file;
	file.open(directoryPath + "/" + fileName, std::ios_base::binary);

	// 失敗したら何もしない
	if (!file.is_open()) {
		Log(std::format("[AudioResource] Can't open file. \'{}\'\n", fileName));
		return false;
	}

	// riff読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	// RIFFじゃなかったらエラー処理
	if (std::strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		Log(std::format("[AudioResource] File \'{}\' is not RIFF chunk.\n", fileName));
		return false;
	}

	// WAVEフォーマットじゃなかったらエラー処理
	if (std::strncmp(riff.type, "WAVE", 4) != 0) {
		Log(std::format("[AudioResource] File \'{}\' is not .WAVE format_.\n", fileName));
		return false;
	}

	// chunk読み込み
	FormatChunk formatChunk{};
	file.read((char*)&formatChunk, sizeof(ChunkHeader));
	// なにかおかしい
	if (std::strncmp(formatChunk.chunk.id, "fmt ", 4)) {
		assert(false);
		return false;
	}
	// chunk.sizeよりformatが小さくないとread時にエラーになる
	assert(formatChunk.chunk.size <= sizeof(formatChunk.format));
	// 読み込み
	file.read((char*)&formatChunk.format, formatChunk.chunk.size);

	// データ読み込み
	ChunkHeader data;
	while (file.read((char*)&data, sizeof(data))) {
		if (std::strncmp(data.id, "data", 4) == 0) {
			break;
		}

		if (std::strncmp(data.id, "JUNK", 4) == 0) {
			file.seekg(data.size, std::ios_base::cur);
		}
		else {
			Log(std::format("[AudioResource] Unknown chunk found: {}.\n", std::string(data.id, 4)));
			return false;
		}
	}

	// dataチャンクがなかったらエラー
	if (std::strncmp(data.id, "data", 4) != 0) {
		assert(false);
	}

	// 読み込んだデータをコピーして保持
	format_ = formatChunk.format;
	buffer_.resize(data.size);
	file.read((char*)buffer_.data(), data.size);
	bufferSize_ = data.size;
	file.close();

	Log("[AudioResource] Success\n");
	return true;
}

std::uint32_t AudioResource::size() const noexcept {
	return bufferSize_;
}

const std::basic_string<BYTE>& AudioResource::buffer_data() const noexcept {
	return buffer_;
}

const WAVEFORMATEX& AudioResource::format() const noexcept {
	return format_;
}
