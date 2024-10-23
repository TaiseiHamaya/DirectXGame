#include "ParticleSystemModel.h"

#include "Engine/Module/PolygonMesh/PolygonMeshManager.h"
#include "Engine/Module/TextureManager/TextureManager.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"

void ParticleSystemModel::draw() const {
	auto& commandList = DirectXCommand::GetCommandList();
	std::shared_ptr<PolygonMesh> lockedMesh = mesh.lock();
	if (lockedMesh) {
		commandList->IASetVertexBuffers(0, 1, lockedMesh->get_p_vbv(0));
		commandList->IASetIndexBuffer(lockedMesh->get_p_ibv(0));
		commandList->SetGraphicsRootDescriptorTable(0, particleBuffer.get_handle_gpu());
		commandList->SetGraphicsRootDescriptorTable(2, texture.lock()->get_gpu_handle());

		commandList->DrawIndexedInstanced(lockedMesh->index_size(0), static_cast<UINT>(get_particles().size()), 0, 0, 0);
	}
}

void ParticleSystemModel::set_mesh(const std::string& meshName) {
	mesh = PolygonMeshManager::GetPolygonMesh(meshName);
	auto meshLocked = mesh.lock();
	if (meshLocked) {
		if (meshLocked->material_count()) {
			texture = TextureManager::GetTexture(meshLocked->texture_name(0));
		}
		else {
			texture = TextureManager::GetTexture("Error.png");
		}
	}
}
