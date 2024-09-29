#pragma once

#ifdef _DEBUG

class ImGuiManager final {
private:
	ImGuiManager() noexcept = default;

public:
	~ImGuiManager() noexcept = default;

private:
	ImGuiManager(const ImGuiManager&) = delete;
	ImGuiManager& operator=(const ImGuiManager&) = delete;

private:
	static ImGuiManager& GetInstance() noexcept;

public:
	static void Initialize();
	static void Finalize();
	static void BeginFrame();
	static void EndFrame();

private:

};

#endif // _DEBUG
