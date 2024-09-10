#pragma once

#include <unordered_map>
#include <string>
#include <memory>

class RenderPath;
class Object3DNode;

class RenderPathManager final {
public:
	RenderPathManager();
	~RenderPathManager() noexcept;

	RenderPathManager(const RenderPathManager&) = delete;
	RenderPathManager& operator=(const RenderPathManager&) = delete;

public:
	static RenderPathManager& GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// パスの登録
	/// </summary>
	/// <param name="name">登録名</param>
	/// <param name="path">実体</param>
	static void RegisterPath(std::string&& name, RenderPath&& path);

	/// <summary>
	/// パスの登録解除
	/// </summary>
	/// <param name="name">解除するパス名</param>
	static void UnregisterPath(std::string&& name);

	/// <summary>
	/// 使用するパスの設定
	/// </summary>
	/// <param name="name">パス名</param>
	static void SetPath(const std::string& name);

	/// <summary>
	/// 描画パスの開始
	/// </summary>
	/// <returns></returns>
	static bool BeginFrame();

	/// <summary>
	/// 次の描画ノードに遷移
	/// </summary>
	/// <returns></returns>
	static bool Next();

	/// <summary>
	/// 設定している終了パスがすべて終了しているか
	/// </summary>
	/// <returns></returns>
	static bool IsEnd();

private:
	void create_default();

private:
	// use renderingPath reference. dont delete.
	RenderPath* nowPath = nullptr;
	std::unordered_map<std::string, RenderPath> renderingPath;

	std::shared_ptr<Object3DNode> node;
};
