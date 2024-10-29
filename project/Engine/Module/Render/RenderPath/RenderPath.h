#pragma once

#include <vector>
#include <memory>
#include <initializer_list>

class BaseRenderNode;

class RenderPath final {
public:
	RenderPath() = default;
	~RenderPath() noexcept = default;

	RenderPath(const RenderPath&) = delete;
	RenderPath& operator=(const RenderPath&) = delete;
	RenderPath(RenderPath&&) = default;
	RenderPath& operator=(RenderPath&&) = default;

public:
	/// <summary>
	/// vector配列で初期化
	/// </summary>
	/// <param name="list"></param>
	void initialize(std::vector<std::weak_ptr<BaseRenderNode>>&& list);

	/// <summary>
	/// initializer_listで初期化
	/// </summary>
	/// <param name="list"></param>
	void initialize(std::initializer_list<std::weak_ptr<BaseRenderNode>>&& list);

	void use();

	/// <summary>
	/// 開始処理
	/// </summary>
	/// <returns></returns>
	bool begin();

	/// <summary>
	/// 次のノードに遷移
	/// </summary>
	/// <returns>次に行ったかどうか</returns>
	bool next();

	/// <summary>
	/// ノードがすべて終わっているかどうか
	/// </summary>
	/// <returns></returns>
	bool is_end();

private:
	std::vector<std::weak_ptr<BaseRenderNode>>::iterator nowNode;
	std::vector<std::weak_ptr<BaseRenderNode>> renderNodeList;
};
