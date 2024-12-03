#pragma once

#include <filesystem>

#include <json.hpp>

class JsonResource {
public:
	JsonResource() = default;
	~JsonResource() = default;

	JsonResource(const std::filesystem::path& file);

public:
	void load(const std::filesystem::path& file);

	void save() const;

	nlohmann::json& get();
	const nlohmann::json& cget() const;

private:
	nlohmann::json json;
	std::filesystem::path filePath;
};
