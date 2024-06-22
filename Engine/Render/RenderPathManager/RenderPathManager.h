#pragma once

#include <unordered_map>
#include <string>

class RenderPath;

class RenderPathManager {
public:
	RenderPathManager() = default;
	~RenderPathManager() noexcept = default;

	RenderPathManager(const RenderPathManager&) = delete;
	RenderPathManager& operator=(const RenderPathManager&) = delete;

public:
	static RenderPathManager& GetInstance();
	static void Initialize();
	static void RegisterPath(std::string&& name, RenderPath&& path);
	static void SetPath(const std::string& name);
	static bool BeginFrame();
	static bool Next();
	static void IsEnd();

private:
	// use renderingPath reference. dont delete.
	RenderPath* nowPath = nullptr;
	std::unordered_map<std::string, RenderPath> renderingPath;
};
