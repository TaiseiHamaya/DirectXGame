#pragma once

#include <Library/Math/Affine.h>
#include <Library/Math/Hierarchy.h>
#include <Library/Math/Transform3D.h>

#include <Engine/Assets/Json/JsonAsset.h>

class WorldManager;

class WorldInstance {
public:
	WorldInstance();
	virtual ~WorldInstance();

	WorldInstance(const WorldInstance&) = delete;
	WorldInstance& operator=(const WorldInstance&&) = delete;
	WorldInstance(WorldInstance&&);
	WorldInstance& operator=(WorldInstance&&);

public:
	/// <summary>
	/// 開始処理
	/// </summary>
	virtual void begin() {};

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void update() {};

	/// <summary>
	/// Affine更新直前処理
	/// </summary>
	virtual void fixed_update() {};

	/// <summary>
	/// Affine行列の更新
	/// </summary>
	virtual void update_affine();

	/// <summary>
	/// 遅延更新処理
	/// </summary>
	virtual void late_update() {};

private:
	/// <summary>
	/// WorldAffineの作成
	/// </summary>
	/// <returns></returns>
	Affine create_world_affine() const;

public:
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

	/// <summary>
	/// 階層構造の震度
	/// </summary>
	/// <returns></returns>
	uint32_t depth() const { return hierarchyDepth; };

	const Reference<WorldManager>& world_manager() const { return worldManager; };

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
	const Reference<const WorldInstance>& get_parent_address() const { return hierarchy.get_parent(); };

	/// <summary>
	/// World行列の取得
	/// </summary>
	/// <returns></returns>
	const Affine& world_affine() const { return affine; };

	/// <summary>
	/// WorldPositionの取得
	/// </summary>
	/// <returns></returns>
	const Vector3& world_position() const { return affine.get_origin(); };

	/// <summary>
	/// 親子付けを再設定
	/// </summary>
	/// <param name="instance">対象のInstance</param>
	/// <param name="isKeepPose">現在の姿勢を維持する</param>
	void reparent(Reference<const WorldInstance> instance, bool isKeepPose = true);

	void set_world_manager(Reference<WorldManager> worldManager_);

public:
	void from_json(const JsonAsset& json);
	void to_json(JsonAsset& json);

#ifdef _DEBUG
public:
	virtual void debug_gui();
#endif // _DEBUG

protected:
	Transform3D transform{}; // Transform
	Hierarchy hierarchy{}; // Hierarchy

private:
	Affine affine;

	Reference<WorldManager> worldManager{ nullptr };
	uint32_t hierarchyDepth{ 0 };

protected:
	bool isActive = true;
};

