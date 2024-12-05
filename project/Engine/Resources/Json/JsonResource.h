#pragma once

#include <filesystem>
#include <string>

#include <json.hpp>

#ifdef _DEBUG
#include "Engine/Debug/ImGui/ImGuiJsonEditor/ImGuiValueEditor.h"
#endif // _DEBUG

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

	template<typename T>
	T try_emplace(const std::string& contains);

	template<typename T>
	void white(const std::string& name, const T& value);

public:
	template<typename T, typename ...Args>
	void register_value(const std::string& name, T* pValue, Args&& ...args);

#ifdef _DEBUG
	void show_imgui();
#endif // _DEBUG

private:
	nlohmann::json json;
	std::filesystem::path filePath;

#ifdef _DEBUG
	ImGuiValueEditor valueEditor;
#endif // _DEBUG
};

template<typename T>
inline T JsonResource::try_emplace(const std::string& contains) {
	if (json.contains(contains)) {
		return json.at(contains).get<T>();
	}
	else {
		nlohmann::json& result = json.emplace(contains, T{}).first.value();
		return result.get<T>();
	}
}

template<typename T>
inline void JsonResource::white(const std::string& name, const T& value) {
	json[name] = value;
}

template<typename T, typename ...Args>
inline void JsonResource::register_value(const std::string& name, T* pValue, [[maybe_unused]] Args&& ...args) {
	assert(pValue != nullptr);
	(*pValue) = std::move(this->try_emplace<T>(name));
#ifdef _DEBUG
	valueEditor.register_value(
		[&, &rValue = *pValue, name]() { this->white(name, rValue); },
		name, pValue, std::forward(args)...
	);
#endif // _DEBUG
}

#define __JSON_RESOURCE_REGISTER(variable) std::string{ #variable }, &variable
