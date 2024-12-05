#pragma once

#include <json.hpp>


namespace nlohmann {

#ifdef TRANSFORM3D_SERIALIZER
#define QUATERNION_SERIALIZER
#define VECTOR3_SERIALIZER
#include <Library/Math/Transform3D.h>
template<>
struct adl_serializer<Transform3D> {
	static void to_json(json& j, const Transform3D& rhs);

	static void from_json(const json& j, Transform3D& rhs);
};
#endif // TRANSFORM3D_SERIALIZER

#ifdef TRANSFORM2D_SERIALIZER
#define VECTOR2_SERIALIZER
#include <Library/Math/Transform2D.h>
template<>
struct adl_serializer<Transform2D> {
	static void to_json(json& j, const Transform2D& rhs);

	static void from_json(const json& j, Transform2D& rhs);
};
#endif // TRANSFORM3D_SERIALIZER


// Quaternion <-> Json Serializer
#ifdef QUATERNION_SERIALIZER
#define VECTOR3_SERIALIZER
#include <Library/Math/Quaternion.h>
template<>
struct adl_serializer<Quaternion> {
	static void to_json(json& j, const Quaternion& rhs);

	static void from_json(const json& j, Quaternion& rhs);
};
#endif // QUATERNION_SERIALIZER

// Vector3 <-> Json Serializer
#ifdef VECTOR3_SERIALIZER
#include <Library/Math/Vector3.h>
template<>
struct adl_serializer<Vector3> {
	static void to_json(json& j, const Vector3& rhs);

	static void from_json(const json& j, Vector3& rhs);
};
#endif // VECTOR3_SERIALIZER

// Vector2 <-> Json Serializer
#ifdef VECTOR2_SERIALIZER
#include <Library/Math/Vector2.h>
template<>
struct adl_serializer<Vector2> {
	static void to_json(json& j, const Vector2& rhs);

	static void from_json(const json& j, Vector2& rhs);
};
#endif // VECTOR2_SERIALIZER

// Color3 <-> Json Serializer
#ifdef COLOR3_SERIALIZER
#include <Library/Math/Color3.h>
template<>
struct adl_serializer<Color3> {
	static void to_json(json& j, const Color3& rhs);

	static void from_json(const json& j, Color3& rhs);
};
#endif // COLOR3_SERIALIZER

// Color4 <-> Json Serializer
#ifdef COLOR4_SERIALIZER
#include <Library/Math/Color4.h>
template<>
struct adl_serializer<Color4> {
	static void to_json(json& j, const Color4& rhs);

	static void from_json(const json& j, Color4& rhs);
};
#endif // COLOR4_SERIALIZER


/// ------------------------------------------------------
/// -------------------- ここから定義 --------------------
/// ------------------------------------------------------

#ifdef TRANSFORM3D_SERIALIZER
inline void adl_serializer<Transform3D>::to_json(json& j, const Transform3D& rhs) {
	j["Scale"] = rhs.get_scale();
	j["Rotation"] = rhs.get_quaternion();
	j["Translate"] = rhs.get_translate();
}

inline void adl_serializer<Transform3D>::from_json(const json& j, Transform3D& rhs) {
	if (j.contains("Scale")) {
		rhs.set_scale(j["Scale"].get<Vector3>());
	}
	if (j.contains("Rotation")) {
		rhs.set_quaternion(j["Rotation"].get<Quaternion>());
	}
	if (j.contains("Translate")) {
		rhs.set_translate(j["Translate"].get<Vector3>());
	}
}
#endif // TRANSFORM3D_SERIALIZER

#ifdef TRANSFORM2D_SERIALIZER
inline void adl_serializer<Transform2D>::to_json(json& j, const Transform2D& rhs) {
	j["Scale"] = rhs.get_scale();
	j["Rotation"] = rhs.get_rotate();
	j["Translate"] = rhs.get_translate();
}

inline void adl_serializer<Transform2D>::from_json(const json& j, Transform2D& rhs) {
	if (j.contains("Scale")) {
		rhs.set_scale(j["Scale"].get<Vector2>());
	}
	if (j.contains("Rotation")) {
		rhs.set_rotate(j["Rotation"].get<float>());
	}
	if (j.contains("Translate")) {
		rhs.set_translate(j["Translate"].get<Vector2>());
	}
}
#endif // TRANSFORM2D_SERIALIZER

// Quaternion
#ifdef QUATERNION_SERIALIZER
inline void adl_serializer<Quaternion>::to_json(json& j, const Quaternion& rhs) {
	j["XYZ"] = rhs.vector();
	j["W"] = rhs.real();
}

inline void adl_serializer<Quaternion>::from_json(const json& j, Quaternion& rhs) {
	Vector3 xyz{};
	float w = 0;
	if (j.contains("XYZ") && j.at("XYZ").is_object()) {
		xyz = j["XYZ"];
	}
	if (j.contains("W") && j.at("W").is_number_float()) {
		w = j["W"];
	}
	rhs = Quaternion{ xyz, w };
}
#endif // QUATERNION_SERIALIZER

#ifdef VECTOR3_SERIALIZER
inline void adl_serializer<Vector3>::to_json(json& j, const Vector3& rhs) {
	j["X"] = rhs.x;
	j["Y"] = rhs.y;
	j["Z"] = rhs.z;
}

inline void adl_serializer<Vector3>::from_json(const json& j, Vector3& rhs) {
	if (j.contains("X") && j.at("X").is_number_float()) {
		rhs.x = j["X"];
	}
	if (j.contains("Y") && j.at("Y").is_number_float()) {
		rhs.y = j["Y"];
	}
	if (j.contains("Z") && j.at("Z").is_number_float()) {
		rhs.z = j["Z"];
	}
}
#endif // VECTOR3_SERIALIZER

#ifdef VECTOR2_SERIALIZER
inline void adl_serializer<Vector2>::to_json(json& j, const Vector2& rhs) {
	j["X"] = rhs.x;
	j["Y"] = rhs.y;
}

inline void adl_serializer<Vector2>::from_json(const json& j, Vector2& rhs) {
	if (j.contains("X") && j.at("X").is_number_float()) {
		rhs.x = j["X"];
	}
	if (j.contains("Y") && j.at("Y").is_number_float()) {
		rhs.y = j["Y"];
	}
}
#endif // VECTOR2_SERIALIZER

#ifdef COLOR3_SERIALIZER
inline void adl_serializer<Color3>::to_json(json& j, const Color3& rhs) {
	j["R"] = rhs.red;
	j["G"] = rhs.green;
	j["B"] = rhs.blue;
}

inline void adl_serializer<Color3>::from_json(const json& j, Color3& rhs) {
	if (j.contains("R") && j.at("R").is_number_float()) {
		rhs.red = j["R"];
	}
	if (j.contains("G") && j.at("G").is_number_float()) {
		rhs.green = j["G"];
	}
	if (j.contains("B") && j.at("B").is_number_float()) {
		rhs.blue = j["B"];
	}
}
#endif // COLOR3_SERIALIZER

#ifdef COLOR4_SERIALIZER
inline void adl_serializer<Color4>::to_json(json& j, const Color4& rhs) {
	j["R"] = rhs.red;
	j["G"] = rhs.green;
	j["B"] = rhs.blue;
	j["A"] = rhs.alpha;
}

inline void adl_serializer<Color4>::from_json(const json& j, Color4& rhs) {
	if (j.contains("R") && j.at("R").is_number_float()) {
		rhs.red = j["R"];
	}
	if (j.contains("G") && j.at("G").is_number_float()) {
		rhs.green = j["G"];
	}
	if (j.contains("B") && j.at("B").is_number_float()) {
		rhs.blue = j["B"];
	}
	if (j.contains("A") && j.at("A").is_number_float()) {
		rhs.alpha = j["A"];
	}
}
#endif // COLOR4_SERIALIZER

}
