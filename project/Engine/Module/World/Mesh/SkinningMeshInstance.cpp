#include "SkinningMeshInstance.h"

#include <Library/Utility/Tools/SmartPointer.h>

#include "Engine/Assets/Animation/NodeAnimation/NodeAnimationPlayer.h"
#include "Engine/Assets/Animation/Skeleton/SkeletonAsset.h"
#include "Engine/Assets/Animation/Skeleton/SkeletonLibrary.h"
#include "Engine/Assets/PolygonMesh/PolygonMesh.h"
#include "Engine/Assets/PolygonMesh/PolygonMeshLibrary.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/Texture/Texture.h"

SkinningMeshInstance::SkinningMeshInstance() noexcept(false) :
	StaticMeshInstance() {
	nodeAnimation = eps::CreateUnique<NodeAnimationPlayer>();
}

SkinningMeshInstance::SkinningMeshInstance(const std::string& meshName, const std::string& animationName, bool isLoop) :
	SkinningMeshInstance() {
	reset_animated_mesh(meshName, animationName, isLoop);
}

SkinningMeshInstance::~SkinningMeshInstance() noexcept = default;

void SkinningMeshInstance::begin() {
	if (!isActive) {
		return;
	}
	nodeAnimation->update();
}

void SkinningMeshInstance::update_animation() {
	if (!isActive || !skeletonResrouce) {
		return;
	}

	// Skeletonの行列計算
	const Skeleton& skeleton = skeletonResrouce->skeleton();
	for (uint32_t jointIndex = 0; jointIndex < jointInstances.size(); ++jointIndex) {
		SkeletonSpaceInstance& jointInstance = jointInstances[jointIndex];
		const Joint& joint = skeleton.joints[jointIndex];

		// Transform更新
		// HOTFIX : Animationが存在しないかつデフォルトのTransformが変更されている場合に正しくない
		// Mixamoとかこれの典型例なので注意
		jointInstance.transform.set_scale(nodeAnimation->calculate_scale(joint.name));
		jointInstance.transform.set_quaternion(nodeAnimation->calculate_rotate(joint.name));
		jointInstance.transform.set_translate(nodeAnimation->calculate_translate(joint.name));
		// 行列更新
		if (joint.parent) {
			jointInstance.affine = Affine::FromTransform3D(jointInstance.transform) * jointInstances[joint.parent.value()].affine;
		}
		else {
			jointInstance.affine = Affine::FromTransform3D(jointInstance.transform);
		}
	}
}

void SkinningMeshInstance::reset_animated_mesh(const std::string& meshName, const std::string& animationName, bool isLoop) {
	reset_mesh(meshName);
	skeletonResrouce = SkeletonLibrary::GetSkeleton(meshName);
	reset_animation(meshName, animationName, isLoop);
	create_skeleton();
}

void SkinningMeshInstance::reset_animation(const std::string& fileName, const std::string& animationName, bool isLoop) {
	nodeAnimation->reset_animation(fileName + '-' + animationName);
	nodeAnimation->set_loop(isLoop);
}

NodeAnimationPlayer* const SkinningMeshInstance::get_animation() {
	return nodeAnimation.get();
}

bool SkinningMeshInstance::is_draw() const {
	return StaticMeshInstance::is_draw() && skeletonResrouce;
}

void SkinningMeshInstance::create_skeleton() {
	// Skeletonが取得できない場合何もしない
	if (!skeletonResrouce) {
		return;
	}

	uint32_t jointSize = static_cast<uint32_t>(skeletonResrouce->joint_size());

	// Jointがのサイズが0の場合おかしいので強制終了
	if (jointSize == 0) {
		skeletonResrouce = nullptr;
		return;
	}

	// Jointの配列を作成
	jointInstances.resize(jointSize);

#ifdef DEBUG_FEATURES_ENABLE
	// ボーン描画用(削除予定)
	const Skeleton skeleton = skeletonResrouce->skeleton();
	boneMeshTest.resize(jointSize);
	for (uint32_t i = 0; i < jointSize; ++i) {
		StaticMeshInstance& boneMesh = boneMeshTest[i];
		const Joint& joint = skeleton.joints[i];
		boneMesh.reset_mesh("bone.obj");

		if (joint.parent) {
			boneMesh.reparent(boneMeshTest[joint.parent.value()]);
		}
		else {
			boneMesh.reparent(*this);
		}
	}
#endif // _DEBUG
}

#ifdef DEBUG_FEATURES_ENABLE
#include <Engine/Assets/Texture/TextureLibrary.h>
void SkinningMeshInstance::debug_gui() {
	if (PolygonMeshLibrary::MeshListGui(meshName)) {
		reset_animated_mesh(meshName);
	}
	if (ImGui::Button("ResetMaterialData")) {
		default_material();
	}
	ImGui::Checkbox("Draw", &isDraw);
	ImGui::Separator();
	WorldInstance::debug_gui();
	ImGui::Separator();
	ImGui::Text("Materials");
	for (int i = 0; auto & meshMaterial : materials) {
		std::string treeNodeName;
		auto meshData = mesh->mesh_data(i);
		if (meshData) {
			treeNodeName = meshData->materialName;
		}
		if (treeNodeName.empty()) {
			treeNodeName = "UnknownMaterialName" + std::to_string(i);
		}
		std::string textureName = meshMaterial.texture->name();
		if (ImGui::TreeNodeEx(treeNodeName.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
			if (TextureLibrary::TextureListGui(textureName)) {
				set_texture(textureName, i);
			}

			meshMaterial.uvTransform.debug_gui();

			meshMaterial.color.debug_gui();

			if (ImGui::RadioButton("None", meshMaterial.lightingType == LighingType::None)) {
				meshMaterial.lightingType = LighingType::None;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Lambert", meshMaterial.lightingType == LighingType::Lambert)) {
				meshMaterial.lightingType = LighingType::Lambert;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Half lambert", meshMaterial.lightingType == LighingType::HalfLambert)) {
				meshMaterial.lightingType = LighingType::HalfLambert;
			}

			ImGui::DragFloat("Shininess", &meshMaterial.shininess, 0.1f, 0.0f, std::numeric_limits<float>::max());

			ImGui::TreePop();
		}
		++i;
	}
	//ここからAnimation専用処理
	ImGui::Separator();
	nodeAnimation->debug_gui();
	ImGui::Separator();
	if (skeletonResrouce) {
		ImGui::Text("NumSkeleton : %d", skeletonResrouce->joint_size());
	}
	else {
		ImGui::Text("Skeleton is not bind.");
	}
	if (ImGui::Button("Restart")) {
		get_animation()->restart();
	}
}

#endif // _DEBUG
