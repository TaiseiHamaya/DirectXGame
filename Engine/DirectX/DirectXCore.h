#pragma once

#include <d3d12.h>
#include <memory>
#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"

class GameObject;
struct DirectionalLightData;

class DirectXCore final {
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
	static void SetScreenRenderTarget();
	static void EndFrame();
	static void Finalize();

#ifdef _DEBUG
	/// <summary>
	/// カメラ及びライトのImGUIを表示
	/// </summary>
	static void ShowDebugTools();
#endif // _DEBUG
	
	/// <summary>
	/// デバッグ用グリッドの表示
	/// </summary>
	static void ShowGrid();

private:
	static DirectXCore& GetInstance();

private:
	void initialize();

	void begin_frame();
	void screen_render_target();
	void end_frame();

	void createDefaultPSO();
	void createPosteffectPSO();

#ifdef _DEBUG
	void show_debug_tools();
#endif // _DEBUG

private:
	D3D12_VIEWPORT viewPort;
	D3D12_RECT scissorRect;

	//std::unique_ptr<PipelineState> posteffectPipeline;

	std::unique_ptr<GameObject> gridMesh;
	std::unique_ptr<ConstantBuffer<DirectionalLightData>> light;

private:
	class Debug final {
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