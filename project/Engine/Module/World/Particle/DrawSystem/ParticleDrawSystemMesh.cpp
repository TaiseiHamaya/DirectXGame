#include "ParticleDrawSystemMesh.h"

#include "Engine/Resources/PolygonMesh/PolygonMeshManager.h"
#include "Engine/Resources/Texture/TextureManager.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"

ParticleDrawSystemMesh::ParticleDrawSystemMesh(const std::string& meshName) {
	set_mesh(meshName);
}

void ParticleDrawSystemMesh::draw_command(size_t InstanceCount) const {
	auto& commandList = DirectXCommand::GetCommandList();
	std::shared_ptr<PolygonMesh> lockedMesh = mesh.lock();
	if (lockedMesh) {
		commandList->IASetVertexBuffers(0, 1, lockedMesh->get_p_vbv(0));
		commandList->IASetIndexBuffer(lockedMesh->get_p_ibv(0));
		commandList->SetGraphicsRootDescriptorTable(0, particleBuffer.get_handle_gpu());
		commandList->SetGraphicsRootDescriptorTable(2, texture.lock()->get_gpu_handle());

		commandList->DrawIndexedInstanced(lockedMesh->index_size(0), static_cast<UINT>(InstanceCount), 0, 0, 0);
	}
}

void ParticleDrawSystemMesh::set_mesh(const std::string& meshName) {
	mesh = PolygonMeshManager::GetPolygonMesh(meshName);
	auto meshLocked = mesh.lock();
	if (meshLocked) {
		if (meshLocked->material_count()) {
			texture = TextureManager::GetTexture(meshLocked->material_data(0)->textureFileName);
		}
		else {
			texture = TextureManager::GetTexture("Error.png");
		}
	}
}
