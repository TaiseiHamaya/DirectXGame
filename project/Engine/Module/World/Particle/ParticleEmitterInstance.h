#pragma once

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

#include <list>
#include <memory>
#include <variant>

#include "./DrawSystem/BaseParticleDrawSystem.h"
#include "./Particle/Particle.h"

#define VECTOR3_SERIALIZER
#define VECTOR2_SERIALIZER
#define COLOR4_SERIALIZER
#define QUATERNION_SERIALIZER
#include "Engine/Assets/Json/JsonSerializer.h"

class ParticleEmitterInstance : public WorldInstance {
public:
	template<typename T>
	struct Randomize {
		T min;
		T max;
	};

	struct ParticleInit {
		Randomize<float> lifetime;
		Randomize<float> speed;
		struct Direction {
			enum class Mode {
				Constant,
				EmissionShape,
				AngleRange,
			} mode = Mode::Constant;
			struct Constant {
				Vector3 direction;
			};
			struct AngleRange {
				Vector3 baseDirection;
				float angle;
			};
			std::variant<Constant, std::monostate, AngleRange> data;
		} direction;
		Randomize<Vector3> acceleration;
		Randomize<Vector3> size;
		struct Rotation {
			Particle::RotationType mode;
			struct Constant {
				Vector3 radian;
				Quaternion rotation;
			};
			struct Random {
				Randomize<float> angularVelocity;
			};
			std::variant<Constant, std::monostate, Random> data;
		} rotation;
		Randomize<Color4> color;

		void debug_gui(const char* tag);
	};

	struct ParticleFinal {
		Randomize<Vector3> size;
		Randomize<Color4> color;

		void debug_gui(const char* tag);
	};

	struct Emission {
		float Time;
		uint32_t Count;
		uint32_t Cycles;
		float Interval;

		struct Shape {
			enum class ShapeType {
				Point,
				Sphere,
				Cone,
				Box,
				//Circle,
			} shapeType{ ShapeType::Point };
			struct Sphere {
				float radius;
			};
			struct Cone {
				float radius;
				Vector3 direction;
				float angle;
			};
			struct Box {
				Vector3 size;
				Vector3 offset;
			};
			std::variant<std::monostate, Sphere, Cone, Box> data;
		} shape;

		void debug_gui(const char* tag);
	};

public: // Constructor/Destructor
	ParticleEmitterInstance(std::filesystem::path jsonFile, uint32_t MaxParticle);
	virtual ~ParticleEmitterInstance() = default;

public: // Member function
	virtual void update();
	virtual void transfer();
	void draw() const;

	virtual void on_emit(Particle* const particle) {};
	virtual void restart();

	void emit();

protected:
	void emit_once();

public:
	bool is_end_all() const;
	bool is_end() const;

private:
	void create_draw_system();

public: // Getter/Setter

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

protected: // Member variable
	float timer;
	bool isLoop;
	float duration;

	ParticleDrawType drawType;
	std::string useResourceName;
	struct Rect {
		Vector2 rect;
		Vector2 pivot;
	};
	std::variant<std::monostate, Rect> drawSystemData;

	ParticleInit particleInit;
	ParticleFinal particleFinal;
	Emission emission;

	JsonAsset jsonResource;

private:
	uint32_t numMaxParticle{ 0 };
	std::list<std::unique_ptr<Particle>> particles;
	std::unique_ptr<BaseParticleDrawSystem> drawSystem;
};

namespace nlohmann {

template<>
struct adl_serializer<ParticleEmitterInstance::ParticleInit> {
	static void to_json(json& j, const ParticleEmitterInstance::ParticleInit& rhs);

	static void from_json(const json& j, ParticleEmitterInstance::ParticleInit& rhs);
};

template<>
struct adl_serializer<ParticleEmitterInstance::ParticleFinal> {
	static void to_json(json& j, const ParticleEmitterInstance::ParticleFinal& rhs);

	static void from_json(const json& j, ParticleEmitterInstance::ParticleFinal& rhs);
};

template<>
struct adl_serializer<ParticleEmitterInstance::Emission> {
	static void to_json(json& j, const ParticleEmitterInstance::Emission& rhs);

	static void from_json(const json& j, ParticleEmitterInstance::Emission& rhs);
};

inline void adl_serializer<ParticleEmitterInstance::ParticleInit>::to_json(json& j, const ParticleEmitterInstance::ParticleInit& rhs) {
	j["Lifetime"] = nlohmann::json::object();
	j["Lifetime"]["Min"] = rhs.lifetime.min;
	j["Lifetime"]["Max"] = rhs.lifetime.max;
	j["Speed"] = nlohmann::json::object();
	j["Speed"]["Min"] = rhs.speed.min;
	j["Speed"]["Max"] = rhs.speed.max;
	j["Direction"] = nlohmann::json::object();
	j["Direction"]["Mode"] = rhs.direction.mode;
	j["Direction"]["Data"] = nlohmann::json::object();
	switch (rhs.direction.mode) {
	case ParticleEmitterInstance::ParticleInit::Direction::Mode::Constant:
	{
		const auto& data = std::get<ParticleEmitterInstance::ParticleInit::Direction::Constant>(rhs.direction.data);
		j["Direction"]["Data"]["Direction"] = data.direction;
		break;
	}
	case ParticleEmitterInstance::ParticleInit::Direction::Mode::EmissionShape:
		// nothing
		break;
	case ParticleEmitterInstance::ParticleInit::Direction::Mode::AngleRange:
	{
		const auto& data = std::get<ParticleEmitterInstance::ParticleInit::Direction::AngleRange>(rhs.direction.data);
		j["Direction"]["Data"]["Direction"] = data.baseDirection;
		j["Direction"]["Data"]["Angle"] = data.angle;
		break;
	}
	default:
		break;
	}
	j["Acceleration"] = nlohmann::json::object();
	j["Acceleration"]["Min"] = rhs.acceleration.min;
	j["Acceleration"]["Max"] = rhs.acceleration.max;
	j["Size"] = nlohmann::json::object();
	j["Size"]["Min"] = rhs.size.min;
	j["Size"]["Max"] = rhs.size.max;
	j["Rotation"] = nlohmann::json::object();
	j["Rotation"]["Mode"] = rhs.rotation.mode;
	switch (rhs.rotation.mode) {
	case Particle::RotationType::Constant:
	{
		auto& data = std::get<ParticleEmitterInstance::ParticleInit::Rotation::Constant>(rhs.rotation.data);
		j["Rotation"]["Data"] = data.rotation;
		break;
	}
	case Particle::RotationType::Velocity:
	case Particle::RotationType::LookAt:
		break;
	case Particle::RotationType::Random:
	{
		auto& data = std::get<ParticleEmitterInstance::ParticleInit::Rotation::Random>(rhs.rotation.data);
		j["Rotation"]["Data"] = nlohmann::json::object();
		j["Rotation"]["Data"]["Min"] = data.angularVelocity.min;
		j["Rotation"]["Data"]["Max"] = data.angularVelocity.max;
		break;
	}
	default:
		break;
	}

	j["Color"] = nlohmann::json::object();
	j["Color"]["Min"] = rhs.color.min;
	j["Color"]["Max"] = rhs.color.max;
}

inline void adl_serializer<ParticleEmitterInstance::ParticleInit>::from_json(const json& j, ParticleEmitterInstance::ParticleInit& rhs) {
	if (j.contains("Lifetime")) {
		if (j["Lifetime"].contains("Min")) {
			rhs.lifetime.min = j["Lifetime"]["Min"];
		}
		if (j["Lifetime"].contains("Max")) {
			rhs.lifetime.max = j["Lifetime"]["Max"];
		}
	}
	if (j.contains("Speed")) {
		if (j["Speed"].contains("Min")) {
			rhs.speed.min = j["Speed"]["Min"];
		}
		if (j["Speed"].contains("Max")) {
			rhs.speed.max = j["Speed"]["Max"];
		}
	}
	if (j.contains("Direction")) {
		if (j["Direction"].contains("Mode")) {
			rhs.direction.mode = j["Direction"]["Mode"];
		}
		if (j["Direction"].contains("Data")) {
			switch (rhs.direction.mode) {
			case ParticleEmitterInstance::ParticleInit::Direction::Mode::Constant:
				rhs.direction.data = ParticleEmitterInstance::ParticleInit::Direction::Constant{ j["Direction"]["Data"]["Direction"] };
				break;
			case ParticleEmitterInstance::ParticleInit::Direction::Mode::EmissionShape:
				rhs.direction.data = std::monostate();
				// nothing
				break;
			case ParticleEmitterInstance::ParticleInit::Direction::Mode::AngleRange:
				rhs.direction.data = ParticleEmitterInstance::ParticleInit::Direction::AngleRange{
					j["Direction"]["Data"]["Direction"],
					j["Direction"]["Data"]["Angle"]
				};
			default:
				break;
			}
		}
	}
	if (j.contains("Acceleration")) {
		if (j["Acceleration"].contains("Min")) {
			rhs.acceleration.min = j["Acceleration"]["Min"];
		}
		if (j["Acceleration"].contains("Max")) {
			rhs.acceleration.max = j["Acceleration"]["Max"];
		}
	}
	if (j.contains("Size")) {
		if (j["Size"].contains("Min")) {
			rhs.size.min = j["Size"]["Min"];
		}
		if (j["Size"].contains("Max")) {
			rhs.size.max = j["Size"]["Max"];
		}
	}
	if (j.contains("Rotation")) {
		if (j["Rotation"].contains("Mode")) {
			rhs.rotation.mode = j["Rotation"]["Mode"];
			switch (rhs.rotation.mode) {
			case Particle::RotationType::Constant:
			{
				//auto& data = std::get<ParticleEmitterInstance::ParticleInit::Rotation::Constant>(rhs.rotation.data);
				rhs.rotation.data = ParticleEmitterInstance::ParticleInit::Rotation::Constant{ j["Rotation"]["Data"] };
				break;
			}
			case Particle::RotationType::Velocity:
			case Particle::RotationType::LookAt:
				rhs.rotation.data = std::monostate();
				break;
			case Particle::RotationType::Random:
			{
				//auto& data = std::get<ParticleEmitterInstance::ParticleInit::Rotation::Random>(rhs.rotation.data);
				rhs.rotation.data = ParticleEmitterInstance::ParticleInit::Rotation::Random{
					j["Rotation"]["Data"]["Min"],
					j["Rotation"]["Data"]["Max"]
				};
				break;
			}
			default:
				break;
			}
		}
	}
	if (j.contains("Color")) {
		if (j["Color"].contains("Min")) {
			rhs.color.min = j["Color"]["Min"];
		}
		if (j["Color"].contains("Max")) {
			rhs.color.max = j["Color"]["Max"];
		}
	}
}

inline void adl_serializer<ParticleEmitterInstance::ParticleFinal>::to_json(json& j, const ParticleEmitterInstance::ParticleFinal& rhs) {
	j["Color"] = nlohmann::json::object();
	j["Color"]["Min"] = rhs.color.min;
	j["Color"]["Max"] = rhs.color.max;
	j["Size"] = nlohmann::json::object();
	j["Size"]["Min"] = rhs.size.min;
	j["Size"]["Max"] = rhs.size.max;
}

inline void adl_serializer<ParticleEmitterInstance::ParticleFinal>::from_json(const json& j, ParticleEmitterInstance::ParticleFinal& rhs) {
	if (j.contains("Size")) {
		if (j["Size"].contains("Min")) {
			rhs.size.min = j["Size"]["Min"];
		}
		if (j["Size"].contains("Max")) {
			rhs.size.max = j["Size"]["Max"];
		}
	}
	if (j.contains("Color")) {
		if (j["Color"].contains("Min")) {
			rhs.color.min = j["Color"]["Min"];
		}
		if (j["Color"].contains("Max")) {
			rhs.color.max = j["Color"]["Max"];
		}
	}
}

inline void adl_serializer<ParticleEmitterInstance::Emission>::to_json(json& j, const ParticleEmitterInstance::Emission& rhs) {
	j["Time"] = rhs.Time;
	j["Count"] = rhs.Count;
	j["Cycles"] = rhs.Cycles;
	j["Interval"] = rhs.Interval;
	j["Shape"] = nlohmann::json::object();
	j["Shape"]["Type"] = rhs.shape.shapeType;
	j["Shape"]["Data"] = nlohmann::json::object();
	switch (rhs.shape.shapeType) {
	case ParticleEmitterInstance::Emission::Shape::ShapeType::Point:
		break;
	case ParticleEmitterInstance::Emission::Shape::ShapeType::Sphere:
	{
		const auto& data = std::get<ParticleEmitterInstance::Emission::Shape::Sphere>(rhs.shape.data);
		j["Shape"]["Data"]["Radius"] = data.radius;
		break;
	}
	case ParticleEmitterInstance::Emission::Shape::ShapeType::Cone:
	{
		const auto& data = std::get<ParticleEmitterInstance::Emission::Shape::Cone>(rhs.shape.data);
		j["Shape"]["Data"]["Radius"] = data.radius;
		j["Shape"]["Data"]["Direction"] = data.direction;
		j["Shape"]["Data"]["Angle"] = data.angle;
		break;
	}
	case ParticleEmitterInstance::Emission::Shape::ShapeType::Box:
	{
		const auto& data = std::get<ParticleEmitterInstance::Emission::Shape::Box>(rhs.shape.data);
		j["Shape"]["Data"]["Size"] = data.size;
		j["Shape"]["Data"]["Offset"] = data.offset;
		break;
	}
	default:
		break;
	}
}

inline void adl_serializer<ParticleEmitterInstance::Emission>::from_json(const json& j, ParticleEmitterInstance::Emission& rhs) {
	if (j.contains("Time")) {
		rhs.Time = j["Time"];
	}
	if (j.contains("Count")) {
		rhs.Count = j["Count"];
	}
	if (j.contains("Cycles")) {
		rhs.Cycles = j["Cycles"];
	}
	if (j.contains("Interval")) {
		rhs.Interval = j["Interval"];
	}
	if (j.contains("Shape")) {
		if (j["Shape"].contains("Type")) {
			rhs.shape.shapeType = j["Shape"]["Type"];
		}
		if (j["Shape"].contains("Data")) {
			auto& json = j["Shape"]["Data"];
			switch (rhs.shape.shapeType) {
			case ParticleEmitterInstance::Emission::Shape::ShapeType::Point:
				rhs.shape.data = std::monostate();
				break;
			case ParticleEmitterInstance::Emission::Shape::ShapeType::Sphere:
				rhs.shape.data = ParticleEmitterInstance::Emission::Shape::Sphere{
					json["Radius"],
				};
				break;
			case ParticleEmitterInstance::Emission::Shape::ShapeType::Cone:
				rhs.shape.data = ParticleEmitterInstance::Emission::Shape::Cone{
					json["Radius"],
					json["Direction"],
					json["Angle"]
				};
				break;
			case ParticleEmitterInstance::Emission::Shape::ShapeType::Box:
				rhs.shape.data = ParticleEmitterInstance::Emission::Shape::Box{
					json["Size"],
					json["Offset"]
				};
				break;
			default:
				break;
			}
		}
	}
}

};
