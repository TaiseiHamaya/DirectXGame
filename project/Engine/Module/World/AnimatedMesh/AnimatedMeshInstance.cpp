#include "AnimatedMeshInstance.h"

#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/ConstantBuffer/Material/Material.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/ConstantBuffer/TransformMatrix/TransformMatrix.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Engine/Resources/Animation/NodeAnimation/NodeAnimationPlayer.h"
#include "Engine/Resources/Animation/Skeleton/SkeletonManager.h"
#include "Engine/Resources/Animation/Skeleton/SkeletonResource.h"
#include "Engine/Resources/PolygonMesh/PolygonMesh.h"
#include "Engine/Utility/Tools/SmartPointer.h"

AnimatedMeshInstance::AnimatedMeshInstance() noexcept(false) :
	MeshInstance() {
}

AnimatedMeshInstance::AnimatedMeshInstance(const std::string& meshName, const std::string& animationName, bool isLoop) {
	reset_animated_mesh(meshName, animationName, isLoop);

	// Skeletonが取得できない場合何もしない
	if (!skeletonResrouce) {
		return;
	}

	uint32_t jointSize = static_cast<uint32_t>(skeletonResrouce->joint_size());
	// Jointの配列を作成
	skeletonData.jointInstance.resize(jointSize);

	// SkeletonMatrixPaletteWellの生成
	// マルチメッシュ分生成
	skeletonData.matrixPalettes.resize(mesh->material_count());
	for (uint32_t i = 0; StructuredBuffer<SkeletonMatrixPaletteWell>&matrixPalette : skeletonData.matrixPalettes) {
		const std::string& meshName = mesh->mesh_data(i)->meshName;
		const std::vector<uint32_t>* useJointIndexes = skeletonResrouce->use_joint_indexes(meshName);
		if (!useJointIndexes) {
			continue;
		}
		// 各要素について初期化
		matrixPalette.initialize(static_cast<uint32_t>(useJointIndexes->size()));
		++i;
	}

	// ボーン描画用(削除予定)
	const Skeleton skeleton = skeletonResrouce->skeleton();
	boneMeshTest.resize(jointSize);
	for (uint32_t i = 0; i < jointSize; ++i) {
		MeshInstance& boneMesh = boneMeshTest[i];
		const Joint& joint = skeleton.joints[i];
		boneMesh.reset_mesh("bone.obj");

		if (joint.parent) {
			boneMesh.set_parent(boneMeshTest[joint.parent.value()]);
		}
		else {
			boneMesh.set_parent(*this);
		}
	}
}

AnimatedMeshInstance::~AnimatedMeshInstance() noexcept = default;

void AnimatedMeshInstance::begin() {
	if (!isActive) {
		return;
	}
	if (nodeAnimation) {
		nodeAnimation->update();
	}
}

void AnimatedMeshInstance::begin_rendering() noexcept {
	MeshInstance::begin_rendering();
	if (!isActive || !mesh || !skeletonResrouce) {
		return;
	}

	// Skeletonの行列計算
	const Skeleton skeleton = skeletonResrouce->skeleton();
	for (uint32_t jointIndex = 0; jointIndex < skeletonData.jointInstance.size(); ++jointIndex) {
		SkeletonSpaceInstance& jointInstance = skeletonData.jointInstance[jointIndex];
		const Joint& joint = skeleton.joints[jointIndex];

		// Transform更新
		jointInstance.transform.set_scale(nodeAnimation->calculate_scale(joint.name));
		jointInstance.transform.set_quaternion(nodeAnimation->calculate_rotate(joint.name));
		jointInstance.transform.set_translate(nodeAnimation->calculate_translate(joint.name));
		// 行列更新
		if (joint.parent) {
			jointInstance.affine = Affine::FromTransform3D(jointInstance.transform) * skeletonData.jointInstance[joint.parent.value()].affine;
		}
		else {
			jointInstance.affine = Affine::FromTransform3D(jointInstance.transform);
		}
	}

	// MatrixPalletの更新
	for (uint32_t i = 0; StructuredBuffer<SkeletonMatrixPaletteWell>&matrixPalette : skeletonData.matrixPalettes) {
		const std::string& meshName = mesh->mesh_data(i)->meshName;
		const std::vector<uint32_t>* useJointIndexes = skeletonResrouce->use_joint_indexes(meshName); // JointとPaletteをつなぐ配列
		if (!useJointIndexes) {
			continue;
		}
		std::span<SkeletonMatrixPaletteWell>& paletteArray = matrixPalette.get_array(); // Palette配列
		for (uint32_t paletteIndex = 0; paletteIndex < paletteArray.size(); ++paletteIndex) {
			SkeletonMatrixPaletteWell& paletteWell = paletteArray[paletteIndex]; // 編集するPalette
			uint32_t jointIndex = useJointIndexes->at(paletteIndex);
			Affine skeletonSpaceAffine;
			// Jointが存在する場合
			if (jointIndex < skeletonData.jointInstance.size()) {
				const Affine& jointAffine = skeletonData.jointInstance[jointIndex].affine;
				const Affine& ibpAffine = skeleton.joints[jointIndex].inverseBindPoseAffine;
				skeletonSpaceAffine = ibpAffine * jointAffine;
			}
			//// 存在しない場合
			paletteWell.skeletonSpaceMatrix = skeletonSpaceAffine.to_matrix();
			// 最後にInverse
			paletteWell.skeletonSpaceInv = skeletonSpaceAffine.inverse().to_matrix();
		}
		++i;
	}
}

void AnimatedMeshInstance::draw() const {
	// 非アクティブ時は描画しない
	if (!isActive || !isDraw) {
		return;
	}
	// メッシュが存在しなければ描画しない
	if (!mesh || !skeletonResrouce) {
		return;
	}
	// 設定したデータをコマンドに積む
	const auto& commandList = DirectXCommand::GetCommandList();
	const Skeleton& skeleton = skeletonResrouce->skeleton();
	for (int i = 0; i < mesh->material_count(); ++i) {
		const std::string& meshName = mesh->mesh_data(i)->meshName;

		const VertexBuffer<VertexInfluenceData>* weightInfluence = skeletonResrouce->weight_influence(meshName);
		if (!weightInfluence) {
			continue;
		}

		D3D12_VERTEX_BUFFER_VIEW vbv[2] = {
			mesh->get_vbv(i),
			weightInfluence->get_vbv()
		};
		commandList->IASetVertexBuffers(0, 2, vbv); // VBV
		commandList->IASetIndexBuffer(mesh->get_p_ibv(i)); // IBV
		commandList->SetGraphicsRootConstantBufferView(0, transformMatrix->get_resource()->GetGPUVirtualAddress()); // Matrix
		commandList->SetGraphicsRootConstantBufferView(2, meshMaterials[i].material->get_resource()->GetGPUVirtualAddress()); // Material
		commandList->SetGraphicsRootDescriptorTable(4, meshMaterials[i].texture->get_gpu_handle());// Texture
		commandList->SetGraphicsRootDescriptorTable(5, skeletonData.matrixPalettes[i].get_handle_gpu()); // BoneMatrix
		commandList->DrawIndexedInstanced(mesh->index_size(i), 1, 0, 0, 0); // 描画コマンド
	}
}

void AnimatedMeshInstance::reset_animated_mesh(const std::string& meshName, const std::string& animationName, bool isLoop) {
	reset_mesh(meshName);
	skeletonResrouce = SkeletonManager::GetSkeleton(meshName);
	nodeAnimation = eps::CreateUnique<NodeAnimationPlayer>(meshName, animationName, isLoop);
}

NodeAnimationPlayer* const AnimatedMeshInstance::get_animation() {
	return nodeAnimation.get();
}

#ifdef _DEBUG
void AnimatedMeshInstance::debug_gui() {
	MeshInstance::debug_gui();
	ImGui::Separator();
	if (nodeAnimation) {
		nodeAnimation->debug_gui();
	}
	ImGui::Separator();
	if (skeletonResrouce) {
		ImGui::Text("NumSkeleton : %d", skeletonResrouce->joint_size());
	}
	else {
		ImGui::Text("Skeleton is not bind.");
	}
}

void AnimatedMeshInstance::draw_skeleton() {
	for (uint32_t i = 0; auto & drawMesh : boneMeshTest) {
		SkeletonSpaceInstance& jointInstance = skeletonData.jointInstance[i];
		drawMesh.get_transform().copy(jointInstance.transform);

		drawMesh.begin_rendering();
		drawMesh.draw();
		++i;
	}
}
#endif // _DEBUG
