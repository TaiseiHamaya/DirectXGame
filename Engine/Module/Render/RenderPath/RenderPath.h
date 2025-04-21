#pragma once

#include <initializer_list>
#include <memory>
#include <vector>

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
	void initialize(std::vector<std::shared_ptr<BaseRenderNode>>&& list);

	/// <summary>
	/// initializer_listで初期化
	/// </summary>
	/// <param name="list"></param>
	void initialize(std::initializer_list<std::shared_ptr<BaseRenderNode>>&& list);

	/// <summary>
	/// 使用禁止
	/// </summary>
	[[deprecated]] void use();

	/// <summary>
	/// 開始処理
	/// </summary>
	/// <returns></returns>
	bool begin();

	/// <summary>
	/// 次のノードに遷移
	/// </summary>
	/// <returns>正しく処理さてたかの可否</returns>
	bool next();

	/// <summary>
	/// ノードがすべて終わっているかどうか
	/// </summary>
	/// <returns></returns>
	bool is_end();

private:
	std::vector<std::shared_ptr<BaseRenderNode>>::iterator nowNode;
	std::vector<std::shared_ptr<BaseRenderNode>> renderNodeList;
};
