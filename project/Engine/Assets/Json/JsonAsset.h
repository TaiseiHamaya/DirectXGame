#pragma once

#include <filesystem>
#include <string>

#include <json.hpp>

#include "Engine/Application/Output.h"

#ifdef DEBUG_FEATURES_ENABLE
#include "Engine/Debug/ImGui/ImGuiJsonEditor/ImGuiValueEditor.h"
#endif // _DEBUG

template<typename T>
concept UseabelJson = requires(nlohmann::json json, T t) {
	{ json.get<T>() } -> std::same_as<T>;
	{ json["Name"] = t };
};

class JsonAsset {
public:
	JsonAsset() = default;
	~JsonAsset() = default;

	JsonAsset(const std::filesystem::path& file);

public:
	void load(const std::filesystem::path& file);

	void save() const;

	nlohmann::json& get();
	const nlohmann::json& cget() const;

	template<UseabelJson T>
	T try_emplace(const std::string& contains);

	template<UseabelJson T>
	void write(const std::string& name, const T& value);

public:
	template<UseabelJson T, typename ...Args>
	void register_value(const std::string& name, T* pValue, Args&& ...args);

#ifdef DEBUG_FEATURES_ENABLE
	void show_imgui();
#endif // _DEBUG

private:
	nlohmann::json json;
	std::filesystem::path filePath;

#ifdef DEBUG_FEATURES_ENABLE
	ImGuiValueEditor valueEditor;
#endif // _DEBUG
};

template<UseabelJson T>
inline T JsonAsset::try_emplace(const std::string& contains) {
	if (json.contains(contains)) {
		return json.at(contains).get<T>();
	}
	else {
		nlohmann::json& result = json.emplace(contains, T{}).first.value();
		return result.get<T>();
	}
}

template<UseabelJson T>
inline void JsonAsset::write(const std::string& name, const T& value) {
	json[name] = value;
}

template<UseabelJson T, typename ...Args>
inline void JsonAsset::register_value(const std::string& name, T* pValue, [[maybe_unused]] Args&& ...args) {
	if (pValue == nullptr) {
		Warning("Registering value is nullptr. Name\'{}\'", name);
		return;
	}
	(*pValue) = std::move(this->try_emplace<T>(name));
#ifdef DEBUG_FEATURES_ENABLE
	valueEditor.register_value(
		[&, &rValue = *pValue, name]() { this->write(name, rValue); },
		name, pValue, std::forward(args)...
	);
#endif // _DEBUG
}

#define __JSON_RESOURCE_REGISTER(variable) std::string{ #variable }, &variable
