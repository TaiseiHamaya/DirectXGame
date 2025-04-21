#include "StaticMeshDrawExecutor.h"

#include "Engine/Module/World/Mesh/StaticMeshInstance.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/Texture/Texture.h"

StaticMeshDrawExecutor::StaticMeshDrawExecutor(std::shared_ptr<const PolygonMesh> asset_, uint32_t maxInstance) {
	reinitialize(asset_, maxInstance);
}

void StaticMeshDrawExecutor::reinitialize(std::shared_ptr<const PolygonMesh> asset_, uint32_t maxInstance_) {
	asset = asset_;
	maxInstance = maxInstance_;
	matrices.initialize(maxInstance);
	materials.resize(asset->material_count());
	for (StructuredBuffer<MaterialDataBuffer3>& material : materials) {
		material.initialize(maxInstance);
	}
}

void StaticMeshDrawExecutor::draw_command() const {
	if (!asset || instanceCounter == 0) {
		return;
	}

	auto& commandList = DxCommand::GetCommandList();
	for (uint32_t i = 0; i < asset->material_count(); ++i) {
		commandList->IASetVertexBuffers(0, 1, &asset->get_vbv(i));
		commandList->IASetIndexBuffer(asset->get_p_ibv(i));
		commandList->SetGraphicsRootDescriptorTable(0, matrices.get_handle_gpu());
		commandList->SetGraphicsRootDescriptorTable(1, materials[i].get_handle_gpu());

		commandList->DrawIndexedInstanced(asset->index_size(i), instanceCounter, 0, 0, 0);
	}
}

void StaticMeshDrawExecutor::write_to_buffer(Reference<const StaticMeshInstance> instance) {
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

	const Affine& local = instance->local_affine();
	const Affine& world = instance->world_affine();
	Affine transformAffine = local * world;
	matrices[next] = {
		.world = transformAffine,
		.itWorld = transformAffine.inverse().get_basis().transposed()
	};
	const std::vector<IMultiMeshInstance::Material>& instanceMaterials = instance->get_materials();
	const size_t numMaterial = asset->material_count();
	for (uint32_t i = 0; i < numMaterial; ++i) {
		const IMultiMeshInstance::Material& source = instanceMaterials[i];
		materials[i][next] = {
			source.color,
			source.lightingType,
			source.shininess,
			source.texture->index().value(),
			source.uvTransform.get_matrix(),
		};
	}
}
