#include "ParticleEmitterInstance.h"

#include <Library/Math/Definition.h>
#include <Library/Utility/Tools/RandomEngine.h>

#include "../WorldManager.h"
#include "./DrawSystem/ParticleDrawSystemMesh.h"
#include "./DrawSystem/ParticleDrawSystemRect.h"
#include "Engine/Assets/PolygonMesh/PolygonMeshLibrary.h"
#include "Engine/Assets/Texture/TextureLibrary.h"
#include "Engine/Runtime/WorldClock/WorldClock.h"

ParticleEmitterInstance::ParticleEmitterInstance(std::filesystem::path jsonFile, uint32_t MaxParticle) :
	WorldInstance(),
	numMaxParticle(MaxParticle),
	jsonResource("Particle" / jsonFile),
	timer(0) {
	drawType = static_cast<ParticleDrawType>(jsonResource.try_emplace<int>("DrawType"));
	useResourceName = jsonResource.try_emplace<std::string>("useResourceName");
	switch (drawType) {
	case ParticleDrawType::Mesh:
		drawSystemData = std::monostate();
		break;
	case ParticleDrawType::Rect:
	{
		drawSystemData = Rect{};
		auto& data = std::get<Rect>(drawSystemData);
		data.rect = jsonResource.try_emplace<Vector2>("Size");
		data.pivot = jsonResource.try_emplace<Vector2>("Pivot");
		break;
	}
	default:
		break;
	}
	jsonResource.register_value(__JSON_RESOURCE_REGISTER(isLoop));
	jsonResource.register_value(__JSON_RESOURCE_REGISTER(duration));
	jsonResource.register_value(__JSON_RESOURCE_REGISTER(emission));
	jsonResource.register_value(__JSON_RESOURCE_REGISTER(particleInit));
	jsonResource.register_value(__JSON_RESOURCE_REGISTER(particleFinal));
	create_draw_system();
}

void ParticleEmitterInstance::update() {
	timer += WorldClock::DeltaSeconds();
	if (isLoop && is_end()) {
		restart();
	}
	// パティクルの更新
	for (std::unique_ptr<Particle>& particle : particles) {
		if (particle->is_active()) {
			particle->update();
		}
	}
	// 削除
	particles.remove_if(
		[&](std::unique_ptr<Particle>& particle) {
		if (particle->is_destroy()) {
			return true;
		}
		return false;
	});
	// 生成
	if (is_active() && !is_end()) {
		emit();
	}
}

void ParticleEmitterInstance::transfer() {
	if (!drawSystem) {
		return;
	}
	for (uint32_t index = 0; std::unique_ptr<Particle>&particle : particles) {
		drawSystem->write_to_buffer(
			index,
			particle->world_affine().to_matrix(),
			particle->create_uv_matrix(),
			particle->get_color()
		);
		++index;
	}
}

void ParticleEmitterInstance::draw() const {
	if (!isActive || !drawSystem) {
		return;
	}

	drawSystem->draw_command(particles.size());
}

void ParticleEmitterInstance::restart() {
	timer = 0;
}

void ParticleEmitterInstance::emit() {
	uint32_t numEmits = emission.Count;
	for (uint32_t i = 0; i < numEmits; ++i) {
		size_t numParticle = particles.size();
		if (numParticle < numMaxParticle) {
			emit_once();
		}
	}
}

void ParticleEmitterInstance::emit_once() {
	// 位置
	Vector3 offset;
	switch (emission.shape.shapeType) {
	case Emission::Shape::ShapeType::Point:
		offset = CVector3::ZERO;
		break;
	case Emission::Shape::ShapeType::Sphere:
	{
		const auto& data = std::get<ParticleEmitterInstance::Emission::Shape::Sphere>(emission.shape.data);
		float cos = -2.0f * RandomEngine::Random01MOD() + 1.0f;
		float sin = std::sqrt(1.0f - cos * cos);
		float phi = PI2 * RandomEngine::Random01MOD();
		float radius = std::cbrt(RandomEngine::Random01MOD()) * data.radius;
		offset = { sin * std::cos(phi), sin * std::sin(phi), cos };
		offset *= radius;
		break;
	}
	case Emission::Shape::ShapeType::Cone:
	{
		const auto& data = std::get<ParticleEmitterInstance::Emission::Shape::Cone>(emission.shape.data);
		float maxCos = std::cos(data.angle);
		float cos = maxCos + (1 - maxCos) * RandomEngine::Random01MOD();
		float sin = std::sqrt(1.0f - cos * cos);
		float phi = PI2 * RandomEngine::Random01MOD();
		float radius = std::cbrt(RandomEngine::Random01MOD()) * data.radius;
		Vector3 base = { sin * std::cos(phi), sin * std::sin(phi), cos };
		Quaternion rotation1 = Quaternion::FromToRotation(CVector3::FORWARD, base);
		Quaternion rotation2 = Quaternion::FromToRotation(CVector3::FORWARD, data.direction);
		offset = CVector3::FORWARD * (rotation2 * rotation1) * radius;
		break;
	}
	case Emission::Shape::ShapeType::Box:
	{
		const auto& data = std::get<ParticleEmitterInstance::Emission::Shape::Box>(emission.shape.data);
		Vector3 base = {
			std::lerp(0.0f, data.size.x, RandomEngine::Random01MOD()),
			std::lerp(0.0f, data.size.y, RandomEngine::Random01MOD()),
			std::lerp(0.0f, data.size.z, RandomEngine::Random01MOD()),
		};
		offset = base - Vector3::Multiply(data.size, data.offset);
		break;
	}
	default:
		break;
	}
	// 方向
	float speed = std::lerp(particleInit.speed.min, particleInit.speed.max, RandomEngine::Random01Closed());
	Vector3 direction;
	switch (particleInit.direction.mode) {
	case ParticleInit::Direction::Mode::Constant:
	{
		const auto& data = std::get<ParticleEmitterInstance::ParticleInit::Direction::Constant>(particleInit.direction.data);
		direction = data.direction;
		break;
	}
	case ParticleInit::Direction::Mode::EmissionShape:
		direction = offset.normalize_safe();
		break;
	case ParticleInit::Direction::Mode::AngleRange:
	{
		const auto& data = std::get<ParticleEmitterInstance::ParticleInit::Direction::AngleRange>(particleInit.direction.data);
		float angle = std::lerp(0.0f, data.angle, RandomEngine::Random01Closed());
		float phi = std::lerp(0.0f, PI2, RandomEngine::Random01Closed());
		direction = data.baseDirection *
			Quaternion::AngleAxis(CVector3::UP, phi) * Quaternion::AngleAxis(CVector3::FORWARD, angle);
	}
	break;
	default:
		break;
	}
	// 回転
	std::variant<Particle::Constant, std::monostate, Particle::Random> rotation{ std::monostate{} };
	switch (particleInit.rotation.mode) {
	case Particle::RotationType::Constant:
	{
		const auto& data = std::get<ParticleInit::Rotation::Constant>(particleInit.rotation.data);
		rotation = Particle::Constant{ data.rotation };
		break;
	}
	case Particle::RotationType::Velocity:
	case Particle::RotationType::LookAt:
		break;
	case Particle::RotationType::Random:
	{
		const auto& data = std::get<ParticleInit::Rotation::Random>(particleInit.rotation.data);
		float cos = -2.0f * RandomEngine::Random01MOD() + 1.0f;
		float sin = std::sqrt(1.0f - cos * cos);
		float phi = PI2 * RandomEngine::Random01MOD();
		float radius = std::pow(RandomEngine::Random01MOD(), 1.0f / 3.0f);
		Vector3 axis = { sin * std::cos(phi), sin * std::sin(phi), cos };
		rotation = Particle::Random{
			.axis = axis.normalize_safe(),
			.angle = std::lerp(data.angularVelocity.min, data.angularVelocity.max, RandomEngine::Random01Closed())
		};
		break;
	}
	default:
		break;
	}

	// 生成
	auto& newParticle = particles.emplace_back(
		world_manager()->create<Particle>(
			nullptr, false,
			world_position() + offset,
			std::lerp(particleInit.lifetime.min, particleInit.lifetime.max, RandomEngine::Random01Closed()),
			direction * speed,
			Vector3::LerpElement(particleInit.acceleration.min, particleInit.acceleration.max,
				{ RandomEngine::Random01Closed(), RandomEngine::Random01Closed(), RandomEngine::Random01Closed() }),
			Color4::LerpElement(particleInit.color.min, particleInit.color.max,
				{ RandomEngine::Random01Closed(), RandomEngine::Random01Closed(), RandomEngine::Random01Closed(), RandomEngine::Random01Closed() }),
			Color4::LerpElement(particleFinal.color.min, particleFinal.color.max,
				{ RandomEngine::Random01Closed(), RandomEngine::Random01Closed(), RandomEngine::Random01Closed(), RandomEngine::Random01Closed() }),
			Vector3::LerpElement(particleInit.size.min, particleInit.size.max,
				{ RandomEngine::Random01Closed(), RandomEngine::Random01Closed(), RandomEngine::Random01Closed() }),
			Vector3::LerpElement(particleFinal.size.min, particleFinal.size.max,
				{ RandomEngine::Random01Closed(), RandomEngine::Random01Closed(), RandomEngine::Random01Closed() }),
			particleInit.rotation.mode, rotation
		)
	);
}

bool ParticleEmitterInstance::is_end_all() const {
	return is_end() && particles.empty();
}

bool ParticleEmitterInstance::is_end() const {
	return timer >= duration;
}

void ParticleEmitterInstance::create_draw_system() {
	drawSystem.reset();
	switch (drawType) {
	case ParticleDrawType::Mesh:
		drawSystem = std::make_unique<ParticleDrawSystemMesh>(useResourceName);
		break;
	case ParticleDrawType::Rect:
	{
		auto rectSystem = std::make_unique<ParticleDrawSystemRect>(useResourceName);
		const auto& data = std::get<Rect>(drawSystemData);
		rectSystem->create_rect(data.rect, data.pivot);
		drawSystem = std::move(rectSystem);
		break;
	}
	default:
		break;
	}
	drawSystem->create_buffers(numMaxParticle);
}

#ifdef _DEBUG
#include <imgui.h>
void ParticleEmitterInstance::debug_gui() {
	if (ImGui::CollapsingHeader("Emitter")) {
		WorldInstance::debug_gui();
	}
	if (ImGui::CollapsingHeader("Particles")) {
		ImGui::Text("Now/Max : %d/%d", particles.size(), numMaxParticle);
	}
	ImGui::Text("DrawType");
	if (ImGui::RadioButton("Mesh", drawType == ParticleDrawType::Mesh)) {
		drawType = ParticleDrawType::Mesh;
		drawSystemData = std::monostate();
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Rect", drawType == ParticleDrawType::Rect)) {
		drawType = ParticleDrawType::Rect;
		drawSystemData = Rect{};
	}
	ImGui::Text("Draw use : %s", useResourceName.c_str());
	switch (drawType) {
	case ParticleDrawType::Mesh:
		PolygonMeshLibrary::MeshListGui(useResourceName);
		break;
	case ParticleDrawType::Rect:
	{
		TextureLibrary::TextureListGui(useResourceName);
		auto& data = std::get<Rect>(drawSystemData);

		ImGui::DragFloat2("Size", &data.rect.x, 0.1f, 0.0f, 1e10f);
		ImGui::DragFloat2("Pivot", &data.pivot.x, 0.01f);
		break;
	}
	default:
		break;
	}
	if (ImGui::Button("Apply")) {
		create_draw_system();
	}
	ImGui::Separator();
	jsonResource.show_imgui();
	if (ImGui::Button("Save")) {
		jsonResource.write("DrawType", drawType);
		jsonResource.write("useResourceName", useResourceName);
		if (drawType == ParticleDrawType::Rect) {
			const auto& data = std::get<Rect>(drawSystemData);
			jsonResource.write("Size", data.rect);
			jsonResource.write("Pivot", data.pivot);
		}
		jsonResource.save();
	}
}

void ParticleEmitterInstance::ParticleInit::debug_gui(const char* tag) {
	constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
	if (ImGui::TreeNode(tag)) {
		ImGui::DragFloatRange2("Lifetime", &lifetime.min, &lifetime.max, 0.01f, 0.0f, FLOAT_MAX);
		ImGui::DragFloatRange2("Speed", &speed.min, &speed.max, 0.1f, 0.0f, FLOAT_MAX);

		ImGui::Separator();
		if (ImGui::TreeNode("Direction")) {
			if (ImGui::RadioButton("Constant", direction.mode == Direction::Mode::Constant)) {
				direction.mode = Direction::Mode::Constant;
				direction.data = Direction::Constant{};
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("EmissionShape", direction.mode == Direction::Mode::EmissionShape)) {
				direction.mode = Direction::Mode::EmissionShape;
				direction.data = std::monostate();
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("AngleRange", direction.mode == Direction::Mode::AngleRange)) {
				direction.mode = Direction::Mode::AngleRange;
				direction.data = Direction::AngleRange{};
			}
			switch (direction.mode) {
			case Direction::Mode::Constant:
			{
				auto& data = std::get<ParticleEmitterInstance::ParticleInit::Direction::Constant>(direction.data);
				ImGui::DragFloat3("Direction", &data.direction.x, 0.01f, 0.0f, 1.0f);
				break;
			}
			case Direction::Mode::EmissionShape:
				// nothing
				break;
			case Direction::Mode::AngleRange:
			{
				auto& data = std::get<ParticleEmitterInstance::ParticleInit::Direction::AngleRange>(direction.data);
				ImGui::DragFloat3("Direction", &data.baseDirection.x, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Angle", &data.angle, 0.01f, 0.0f, PI);
				break;
			}
			default:
				break;
			}
			ImGui::TreePop();
		}
		ImGui::Separator();
		ImGui::DragFloat3("AccelerationMin", &acceleration.min.x, 0.1f);
		ImGui::DragFloat3("AccelerationMax", &acceleration.max.x, 0.1f);
		ImGui::Separator();
		ImGui::DragFloat3("SizeMin", &size.min.x, 0.1f);
		ImGui::DragFloat3("SizeMax", &size.max.x, 0.1f);

		ImGui::Separator();
		if (ImGui::TreeNode("Rotation")) {
			if (ImGui::RadioButton("Constant", rotation.mode == Particle::RotationType::Constant)) {
				rotation.mode = Particle::RotationType::Constant;
				rotation.data = Rotation::Constant{};
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("LookForward", rotation.mode == Particle::RotationType::Velocity)) {
				rotation.mode = Particle::RotationType::Velocity;
				rotation.data = std::monostate();
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("LookAt", rotation.mode == Particle::RotationType::LookAt)) {
				rotation.mode = Particle::RotationType::LookAt;
				rotation.data = std::monostate();
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Random", rotation.mode == Particle::RotationType::Random)) {
				rotation.mode = Particle::RotationType::Random;
				rotation.data = Rotation::Random();
			}
			switch (rotation.mode) {
			case Particle::RotationType::Constant:
			{
				auto& data = std::get<ParticleEmitterInstance::ParticleInit::Rotation::Constant>(rotation.data);
				ImGui::DragFloat3("RotateRadian", &data.radian.x, 0.1f);
				data.rotation = Quaternion::EulerRadian(data.radian);
				break;
			}
			case Particle::RotationType::Velocity:
			case Particle::RotationType::LookAt:
				break;
			case Particle::RotationType::Random:
			{
				auto& data = std::get<ParticleEmitterInstance::ParticleInit::Rotation::Random>(rotation.data);
				ImGui::DragFloatRange2("MinAnglerVelocity", &data.angularVelocity.min, &data.angularVelocity.max, 0.1f);
				break;
			}
			default:
				break;
			}
			ImGui::TreePop();
		}
		ImGui::Separator();
		color.min.debug_gui("ColorMin");
		color.max.debug_gui("ColorMax");

		ImGui::TreePop();
	}
}

void ParticleEmitterInstance::ParticleFinal::debug_gui(const char* tag) {
	if (ImGui::TreeNode(tag)) {
		ImGui::DragFloat3("SizeMin", &size.min.x, 0.1f);
		ImGui::DragFloat3("SizeMax", &size.max.x, 0.1f);
		ImGui::Separator();
		color.min.debug_gui("ColorMin");
		color.max.debug_gui("ColorMax");
		ImGui::TreePop();
	}
}

void ParticleEmitterInstance::Emission::debug_gui(const char* tag) {
	constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
	if (ImGui::TreeNode(tag)) {
		ImGui::DragFloat("Time", &Time, 0.1f, 0.0f, FLOAT_MAX);
		ImGui::InputScalar("Count", ImGuiDataType_U32, &Count);
		ImGui::InputScalar("Cycles", ImGuiDataType_U32, &Cycles);
		ImGui::DragFloat("Interval", &Interval, 0.1f, 0.0f, FLOAT_MAX);
		ImGui::Separator();
		if (ImGui::TreeNode("Shape")) {
			if (ImGui::RadioButton("Point", shape.shapeType == Shape::ShapeType::Point)) {
				shape.shapeType = Shape::ShapeType::Point;
				shape.data = std::monostate();
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Sphere", shape.shapeType == Shape::ShapeType::Sphere)) {
				shape.shapeType = Shape::ShapeType::Sphere;
				shape.data = Shape::Sphere{};
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Cone", shape.shapeType == Shape::ShapeType::Cone)) {
				shape.shapeType = Shape::ShapeType::Cone;
				shape.data = Shape::Cone{};
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Box", shape.shapeType == Shape::ShapeType::Box)) {
				shape.shapeType = Shape::ShapeType::Box;
				shape.data = Shape::Box{};
			}
			switch (shape.shapeType) {
			case ParticleEmitterInstance::Emission::Shape::ShapeType::Point:
				break;
			case ParticleEmitterInstance::Emission::Shape::ShapeType::Sphere:
			{
				auto& data = std::get<ParticleEmitterInstance::Emission::Shape::Sphere>(shape.data);
				ImGui::DragFloat("Radius", &data.radius, 0.1f, 0.0f, FLOAT_MAX);
				break;
			}
			case ParticleEmitterInstance::Emission::Shape::ShapeType::Cone:
			{
				auto& data = std::get<ParticleEmitterInstance::Emission::Shape::Cone>(shape.data);
				ImGui::DragFloat("Radius", &data.radius, 0.1f, 0.0f, FLOAT_MAX);
				ImGui::DragFloat3("Direction", &data.direction.x, 0.1f, -1.0f, 1.0f);
				ImGui::DragFloat("Angle", &data.angle, 0.1f, 0.0f, PI);
				break;
			}
			case ParticleEmitterInstance::Emission::Shape::ShapeType::Box:
			{
				auto& data = std::get<ParticleEmitterInstance::Emission::Shape::Box>(shape.data);
				ImGui::DragFloat3("Size", &data.size.x, 0.1f, 0.0f, FLOAT_MAX);
				ImGui::DragFloat3("Offset", &data.offset.x, 0.1f, 0.0f, FLOAT_MAX);
				break;
			}
			default:
				break;
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}
#endif // _DEBUG
