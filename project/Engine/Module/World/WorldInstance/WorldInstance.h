#pragma once

#include "Library/Math/Transform3D.h"
#include "Library/Math/Hierarchy.h"
#include "Library/Math/Affine.h"

#include <Engine/Resources/Json/JsonResource.h>

class WorldInstance {
public:
	WorldInstance();
	virtual ~WorldInstance() = default;

	WorldInstance(const WorldInstance&) = delete;
	WorldInstance& operator=(const WorldInstance&&) = delete;
	WorldInstance(WorldInstance&&) = default;
	WorldInstance& operator=(WorldInstance&&) = default;

public:
	/// <summary>
	/// 行列の更新
	/// </summary>
	void update_affine();

	/// <summary>
	/// Targetの方向を向く
	/// </summary>
	/// <param name="target">向く方向</param>
	/// <param name="upward">上方向</param>
	void look_at(const WorldInstance& target, const Vector3& upward = CVector3::BASIS_Y) noexcept;

	/// <summary>
	/// pointの方向を向く
	/// </summary>
	/// <param name="point">World座標系上の点</param>
	/// <param name="upward">上方向</param>
	void look_at(const Vector3& point, const Vector3& upward = CVector3::BASIS_Y) noexcept;

private:
	/// <summary>
	/// WorldMatrixの作成
	/// </summary>
	/// <returns></returns>
	Affine create_world_affine() const;

public:
	/// <summary>
	/// アクティブフラグの設定
	/// </summary>
	/// <param name="isActive_"></param>
	void set_active(bool isActive_) { isActive = isActive_; };

	/// <summary>
	/// アクティブフラグの取得
	/// </summary>
	/// <returns></returns>
	bool is_active() const { return isActive; };

	// 削除するかも？
	const Hierarchy& get_hierarchy() const { return hierarchy; };
	Hierarchy& get_hierarchy() { return hierarchy; };

	/// <summary>
	/// Transformの取得(Const)
	/// </summary>
	/// <returns></returns>
	const Transform3D& get_transform() const { return transform; };

	/// <summary>
	/// Transformの取得(非Const)
	/// </summary>
	/// <returns></returns>
	Transform3D& get_transform() { return transform; };

	/// <summary>
	/// Hierarchyの親アドレスの取得
	/// </summary>
	/// <returns>存在しなければnullptr</returns>
	const WorldInstance* get_parent_address() const { return hierarchy.get_parent_address(); };

	/// <summary>
	/// World行列の取得
	/// </summary>
	/// <returns></returns>
	const Affine& world_affine() const { return affine; };

	/// <summary>
	/// WorldPositionの取得
	/// </summary>
	/// <returns></returns>
	Vector3 world_position() const { return affine.get_origin(); };

	/// <summary>
	/// 親子付け
	/// </summary>
	/// <param name="hierarchy_"></param>
	void set_parent(const WorldInstance& hierarchy_) { hierarchy.set_parent(hierarchy_); };

public:
	void from_json(const JsonResource& json);
	void to_json(JsonResource& json);

#ifdef _DEBUG
public:
	virtual void debug_gui();
#endif // _DEBUG

protected:
	Transform3D transform{}; // Transform
	Hierarchy hierarchy{}; // Hierarchy

private:
	Affine affine;

protected:
	bool isActive = true;
};

