#include "AudioResource.h"

#include <fstream>
#include <array>

#include "Engine/Debug/Output.h"

bool IsEqualArrayChunkId(const std::array<char, 4>& read, std::string&& id) {
	return std::strncmp(read.data(), id.c_str(), 4) == 0;
}

bool AudioResource::load(const std::string& directoryPath, const std::string& fileName) {
	// ローカル変数定義
	struct ChunkHeader {
		std::array<char, 4> id;
		std::int32_t size;
	};
	struct RiffHeader {
		ChunkHeader header;
		std::array<char, 4> type;
	};
	struct FormatChunk {
		ChunkHeader header;
		WAVEFORMATEXTENSIBLE format;
	};

	Console("[AudioResource] Start load .wave file. file-\'{}/{}\'\n", directoryPath, fileName);

	// ファイル読み込み
	std::ifstream file;
	file.open(directoryPath + "/" + fileName, std::ios_base::binary);

	// 失敗したら何もしない
	if (!file.is_open()) {
		Console("[AudioResource] Can't open file. \'{}\'\n", fileName);
		return false;
	}

	// riff読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	// RIFFじゃなかったらエラー処理
	if (!IsEqualArrayChunkId(riff.header.id, "RIFF")) {
		Console("[AudioResource] File \'{}\' is not RIFF chunk.\n", fileName);
		return false;
	}

	// WAVEフォーマットじゃなかったらエラー処理
	if (!IsEqualArrayChunkId(riff.type, "WAVE")) {
		Console("[AudioResource] File \'{}\' is not .WAVE format_.\n", fileName);
		return false;
	}

	// chunk読み込み
	FormatChunk formatChunk{};
	while (file.read((char*)&formatChunk.header, sizeof(ChunkHeader))) {
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
		return false;
	}

	// 読み込み
	file.read((char*)&formatChunk.format, formatChunk.header.size);
	// データ読み込み
	ChunkHeader data;
	while (file.read((char*)&data, sizeof(data))) {
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
		Console("[AudioResource] \'data\' chunk is not found. File-\'{}/{}\'\n", directoryPath, fileName);
		return false;
	}

	// 読み込んだデータをコピーして保持
	format_ = formatChunk.format;
	buffer_.resize(data.size);
	file.read((char*)buffer_.data(), data.size);
	bufferSize_ = data.size;
	file.close();

	Console("[AudioResource] Success\n");
	return true;
}

std::uint32_t AudioResource::size() const noexcept {
	return bufferSize_;
}

const std::basic_string<BYTE>& AudioResource::buffer_data() const noexcept {
	return buffer_;
}

const WAVEFORMATEX& AudioResource::format() const noexcept {
	return format_.Format;
}
