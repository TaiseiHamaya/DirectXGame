#include "Beam.h"

#include <algorithm>

#include <Engine/Application/EngineSettings.h>
#include <Engine/Runtime/Input/Input.h>
#include <Engine/Application/WinApp.h>
#include <Engine/Utility/Tools/SmartPointer.h>
#include <Engine/Runtime/WorldClock/WorldClock.h>
#include <Engine/Module/World/Camera/Camera2D.h>
#include <Library/Math/VectorConverter.h>

#include "Game/GameScene/Player/RailCamera.h"

void Beam::initialize() {
	reticle = {
		static_cast<float>(EngineSettings::CLIENT_WIDTH) / 2,
		static_cast<float>(EngineSettings::CLIENT_HEIGHT) / 2
	};

	sprite = eps::CreateUnique<SpriteInstance>("reticle.png", Vector2{ 0.5f,0.5f });
	beam = eps::CreateUnique<MeshInstance>("beam.obj");
	beam->initialize();
}

void Beam::begin() {
	input();
	beam->begin();
}

void Beam::update() {
	reticle += keyInput * 100.0f * reticleMoveSpeed * WorldClock::DeltaSeconds();
	reticle = {
		std::clamp(reticle.x, 64.0f, static_cast<float>(EngineSettings::CLIENT_WIDTH) - 64) ,
		std::clamp(reticle.y, 64.0f, static_cast<float>(EngineSettings::CLIENT_HEIGHT) - 64)
	};

	sprite->get_transform().set_translate(reticle);

	if (camera) {
		Matrix4x4 viewport = Camera3D::MakeViewportMatrix(CVector2::ZERO, { static_cast<float>(EngineSettings::CLIENT_WIDTH),static_cast<float>(EngineSettings::CLIENT_HEIGHT) });
		Vector3 reticleScreenNear = Transform3D::Homogeneous(Converter::ToVector3(reticle, 0), Camera2D::GetVPMatrix() * viewport);
		Vector3 reticleScreenFar = reticleScreenNear;
		reticleScreenFar.z = 1.0f;
		Matrix4x4 vpv = camera->vp_matrix() * viewport;
		Matrix4x4 vpvInv = vpv.inverse();
		Vector3 nearReticle = Transform3D::Homogeneous(reticleScreenNear, vpvInv);
		Vector3 farReticle = Transform3D::Homogeneous(reticleScreenFar, vpvInv);
		beamRay.origin = nearReticle;
		beamRay.direction = (farReticle - nearReticle).normalize_safe();

		Vector3 cameraUpward = CVector3::BASIS_Y * camera->get_transform().get_quaternion();
		beam->look_at(beamRay.origin + beamRay.direction * 10.f, cameraUpward);
	}

	beam->update();
}

void Beam::begin_rendering() {
	sprite->begin_rendering();
	beam->begin_rendering();
}

void Beam::late_update() {
	beam->late_update();
}

void Beam::draw() const {
	if (isBeaming) {
		beam->draw();
	}
}

void Beam::draw_reticle() const {
	sprite->draw();
}

void Beam::input() {
	keyInput = CVector2::ZERO;
	keyInput = InputAdvanced::PressArrow();

	if (Input::IsTriggerKey(KeyID::Space)) {
		isBeaming ^= 1;
	}
}

void Beam::set_camera(const RailCamera* camera_) {
	camera = camera_;
	if (camera) {
		beam->set_parent(*camera);
		beam->get_transform().set_translate(initialPointOffset);
	}
}

#ifdef _DEBUG
#include <imgui.h>
void Beam::debug_gui() {
	ImGui::Begin("Beam");
	if (ImGui::DragFloat3("Offset", &initialPointOffset.x, 0.01f)) {
		beam->get_transform().set_translate(initialPointOffset);
	}
	ImGui::DragFloat("ReticleMoveSpeed", &reticleMoveSpeed, 0.1f);
	ImGui::End();
}
#endif // _DEBUG
