#pragma once

#include <d3d12.h>
#include <memory>
#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"

class GameObject;
struct DirectionalLightData;
class Camera3D;

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
	static void EndFrame();
	static void Finalize();

	static void Set3DLight();

#ifdef _DEBUG
	/// <summary>
	/// カメラ及びライトのImGUIを表示
	/// </summary>
	static void ShowDebugTools();
#endif // _DEBUG
	
	/// <summary>
	/// デバッグ用グリッドの表示
	/// </summary>
	static void ShowGrid(const Camera3D& camera);

private:
	static DirectXCore& GetInstance();

private:
	void initialize();

	void begin_frame();
	void end_frame();

#ifdef _DEBUG
	void show_debug_tools();
#endif // _DEBUG

private:
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