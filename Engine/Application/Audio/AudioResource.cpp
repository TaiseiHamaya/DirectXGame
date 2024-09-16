#include "AudioResource.h"

#include <fstream>
#include <format>

#include <Engine/Utility/Utility.h>

bool AudioResource::load(const std::string& directoryPath, const std::string& fileName) {
	// ローカル変数定義
	struct ChunkHeader {
		std::string id{ 4 };
		std::int32_t size;
	};
	struct RiffHeader {
		ChunkHeader header;
		std::string type{ 4 };
	};
	struct FormatChunk {
		ChunkHeader header;
		WAVEFORMATEXTENSIBLE format;
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
	if (riff.header.id != "RIFF") {
		Log(std::format("[AudioResource] File \'{}\' is not RIFF chunk.\n", fileName));
		return false;
	}

	// WAVEフォーマットじゃなかったらエラー処理
	if (riff.type != "WAVE") {
		Log(std::format("[AudioResource] File \'{}\' is not .WAVE format_.\n", fileName));
		return false;
	}

	// chunk読み込み
	FormatChunk formatChunk{};
	file.read((char*)&formatChunk, sizeof(ChunkHeader));
	while (file.read((char*)&formatChunk.header, sizeof(ChunkHeader))) {
		if (formatChunk.header.id == "fmt ") {
			break;
		}
		else if (formatChunk.header.id == "JUNK") {
			file.seekg(formatChunk.header.size, std::ios_base::cur);
		}
		else if (formatChunk.header.id == "LIST") {
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
		if (data.id == "data") {
			break;
		}
		else if (data.id == "JUNK") {
			file.seekg(data.size, std::ios_base::cur);
		}
		else if (data.id == "LIST") {
			file.seekg(data.size, std::ios_base::cur);
		}
		else {
			file.seekg(data.size, std::ios_base::cur);
		}
	}

	// dataチャンクがなかったらエラー
	if (data.id != "data") {
		Log(std::format("[AudioResource] data chunk is not found. File-\'{}/{}\'\n", directoryPath, fileName));
		return false;
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
	return format_.Format;
}
