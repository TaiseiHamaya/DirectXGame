#include "SkinningMeshDrawExecutor.h"

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/Texture/Texture.h"
#include "Engine/Module/World/Mesh/SkinningMeshInstance.h"

SkinningMeshDrawExecutor::SkinningMeshDrawExecutor(std::shared_ptr<const PolygonMesh> mesh, std::shared_ptr<const SkeletonAsset> skeleton, uint32_t maxInstance) {
	reinitialize(mesh, skeleton, maxInstance);
}

void SkinningMeshDrawExecutor::reinitialize(std::shared_ptr<const PolygonMesh> mesh_, std::shared_ptr<const SkeletonAsset> skeleton_, uint32_t maxInstance_) {
	mesh = mesh_;
	skeletonData = skeleton_;

	if (!skeletonData) {
		return;
	}

	maxInstance = maxInstance_;
	// Matrix初期化
	matrices.initialize(maxInstance);
	matrices.get_resource()->SetName(L"Transform");
	// Material初期化
	materials.resize(mesh->material_count());
	for (StructuredBuffer<MaterialBufferData>& material : materials) {
		material.initialize(maxInstance);
		material.get_resource()->SetName(L"Materials");
	}
	// Palette初期化
	matrixPalettes.resize(mesh->material_count());
	paletteSize.resize(mesh->material_count());
	for (uint32_t i = 0; MdStructuredBuffer<SkeletonMatrixPaletteWell>&matrixPalette : matrixPalettes) {
		const std::string& meshName = mesh->mesh_data(i)->meshName;
		const std::vector<uint32_t>* useJointIndexes = skeletonData->use_joint_indexes(meshName);
		if (!useJointIndexes) {
			continue;
		}
		// 各要素について初期化
		matrixPalette.initialize(maxInstance, static_cast<uint32_t>(useJointIndexes->size()));
		*paletteSize[i].get_data() = static_cast<uint32_t>(useJointIndexes->size());
		++i;
	}
}

void SkinningMeshDrawExecutor::draw_command() const {
	if (!mesh || !skeletonData || instanceCounter == 0) {
		return;
	}

	auto& commandList = DxCommand::GetCommandList();
	for (uint32_t i = 0; i < mesh->material_count(); ++i) {
		const std::string& meshName = mesh->mesh_data(i)->meshName;
		const VertexBuffer<VertexInfluenceData>* weightInfluence = skeletonData->weight_influence(meshName);
		if (!weightInfluence) {
			continue;
		}
		D3D12_VERTEX_BUFFER_VIEW vbv[2] = {
			mesh->get_vbv(i),
			weightInfluence->get_vbv()
		};

		commandList->IASetVertexBuffers(0, 2, vbv); // VBV
		commandList->IASetIndexBuffer(mesh->get_p_ibv(i));
		commandList->SetGraphicsRootDescriptorTable(0, matrices.get_handle_gpu());
		commandList->SetGraphicsRootDescriptorTable(1, materials[i].get_handle_gpu());
		commandList->SetGraphicsRootDescriptorTable(3, matrixPalettes[i].get_handle_gpu());
		commandList->SetGraphicsRootConstantBufferView(4, paletteSize[i].get_resource()->GetGPUVirtualAddress());

		commandList->DrawIndexedInstanced(mesh->index_size(i), instanceCounter, 0, 0, 0);
	}
}

void SkinningMeshDrawExecutor::write_to_buffer(Reference<const SkinningMeshInstance> instance) {
	if (!instance || !instance->is_draw()) {
		return;
	}

	uint32_t next;
	{
		std::lock_guard<std::mutex> lock{ writeBufferMutex };

		if (instanceCounter >= maxInstance) {
			return;
		}

		next = instanceCounter;
		++instanceCounter;
	}

	const Affine& affine = instance->world_affine();
	matrices[next] = {
		.world = affine,
		.itWorld = affine.inverse().get_basis().transposed()
	};
	const std::vector<StaticMeshInstance::Material>& instanceMaterials = instance->get_materials();
	const size_t numMaterial = mesh->material_count();
	for (uint32_t i = 0; i < numMaterial; ++i) {
		const StaticMeshInstance::Material& source = instanceMaterials[i];
		materials[i][next] = {
			source.color,
			source.lightingType,
			source.shininess,
			source.texture->index().value(),
			source.uvTransform.get_matrix(),
		};
	}
	// MatrixPalletの書き込み
	const std::vector<SkinningMeshInstance::SkeletonSpaceInstance>& joints = instance->joints();
	const Skeleton& skeleton = skeletonData->skeleton();
	for (uint32_t i = 0; i < numMaterial; ++i) {
		std::span<SkeletonMatrixPaletteWell> writeSpan = matrixPalettes[i][next];
		const std::string& meshName = mesh->mesh_data(i)->meshName;
		const std::vector<uint32_t>* useJointIndexes = skeletonData->use_joint_indexes(meshName); // JointとPaletteをつなぐ配列
		if (!useJointIndexes) {
			continue;
		}
		for (uint32_t paletteIndex = 0; paletteIndex < writeSpan.size(); ++paletteIndex) {
			SkeletonMatrixPaletteWell& paletteWell = writeSpan[paletteIndex]; // 編集するPalette
			uint32_t jointIndex = useJointIndexes->at(paletteIndex);
			Affine skeletonSpaceAffine;
			// Jointが存在する場合
			if (jointIndex < joints.size()) {
				const Affine& jointAffine = joints[jointIndex].affine;
				const Affine& ibpAffine = skeleton.joints[jointIndex].inverseBindPoseAffine;
				skeletonSpaceAffine = ibpAffine * jointAffine;
			}
			//// 存在しない場合
			paletteWell.skeletonSpaceMatrix = skeletonSpaceAffine.to_matrix();
			// 最後にInverse
			paletteWell.skeletonSpaceIT = skeletonSpaceAffine.inverse().get_basis().transposed();
		}
	}
}
