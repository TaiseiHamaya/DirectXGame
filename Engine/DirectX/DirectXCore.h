#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>

class PipelineState;

class DirectXCore {
private:
	DirectXCore();
	
public:
	~DirectXCore();

private:
	DirectXCore(const DirectXCore&) = delete;
	DirectXCore& operator=(const DirectXCore&) = delete;

public:
	static void Initialize();
	static void BeginFrame();
	static void EndFrame();
	static void Finalize();

private:
	static DirectXCore& GetInstance();

private:
	void initialize();

	void begin_frame();
	void end_frame();

private:
	D3D12_VIEWPORT viewPort;
	D3D12_RECT scissorRect;

	std::unique_ptr<PipelineState> pipelineState;

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
		static DirectXCore::Debug& GetInstance();

	private:
	};
};