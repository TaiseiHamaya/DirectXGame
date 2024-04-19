#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

class DirectXCore {
public:
	DirectXCore() = default;
	~DirectXCore() = default;

public:
	static void Initialize(const HWND& hWnd);
	static void BeginFrame();
	static void EndFrame();
	static void Finalize();

private:
	static DirectXCore* instance;

private:
	void initialize(const HWND& hWnd);

	void begin_frame();
	void end_frame();

private:

private:
	class Debug {
	private:
		Debug() = default;

	public:
		~Debug();

	private:
		Debug(const Debug&) = delete;
		Debug& operator=(const Debug&) = delete;

	public:
		static void Initialize();
		static void InfoQueue();

	private:
		static DirectXCore::Debug* const GetInstance();

	private:
	};
};