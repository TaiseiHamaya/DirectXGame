#pragma once

#ifdef _DEBUG

class ImGuiManager {
private:
	ImGuiManager() = default;

public:
	~ImGuiManager() = default;

private:
	ImGuiManager(const ImGuiManager&) = delete;
	ImGuiManager& operator=(const ImGuiManager&) = delete;

private:
	static ImGuiManager& GetInstance();

public:
	static void Initialize();
	static void Finalize();
	static void BeginFrame();
	static void EndFrame();

private:

};

#endif // _DEBUG