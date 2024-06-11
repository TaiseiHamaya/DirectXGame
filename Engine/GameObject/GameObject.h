#pragma once

#include <memory>
#include <string>

class Transform2D;
class Transform3D;
class PolygonMesh;
class Texture;
class Material;
class TransformMatrix;
class Color;

class GameObject {
public:
	GameObject() noexcept(false);
	explicit GameObject(const std::string& meshName_) noexcept(false);
	~GameObject() noexcept;

	GameObject(GameObject&&) noexcept;
	GameObject& operator=(GameObject&&) noexcept;

private:
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;

public:
	const Transform3D& get_transform() noexcept;
	void update();
	void begin_rendering() noexcept;
	void draw() const;

	/// <summary>
	/// Texutre、Materialパラメータ、UVデータのリセットそ行う
	/// </summary>
	void reset_default();

#ifdef _DEBUG
	void debug_gui();
#endif // _DEBUG

private:
	std::weak_ptr<PolygonMesh> mesh;
	std::weak_ptr<Texture> texture;

	std::string meshName;
	std::string textureName;

	std::unique_ptr<Material> material;
	std::unique_ptr<TransformMatrix> transformMatrix;

protected:
	Color& color;

	std::unique_ptr<Transform3D> transform;
	std::unique_ptr<Transform2D> uvTransform;
};
