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
	if (mesh) {
		commandList->IASetVertexBuffers(0, 1, &mesh->get_vbv(0));
		commandList->IASetIndexBuffer(mesh->get_p_ibv(0));
		commandList->SetGraphicsRootDescriptorTable(0, particleBuffer.get_handle_gpu());
		commandList->SetGraphicsRootDescriptorTable(2, texture->get_gpu_handle());

		commandList->DrawIndexedInstanced(mesh->index_size(0), static_cast<UINT>(InstanceCount), 0, 0, 0);
	}
}

void ParticleDrawSystemMesh::set_mesh(const std::string& meshName) {
	mesh = PolygonMeshManager::GetPolygonMesh(meshName);
	if (mesh) {
		if (mesh->material_count()) {
			texture = TextureManager::GetTexture(mesh->material_data(0)->textureFileName);
		}
		else {
			texture = TextureManager::GetTexture("Error.png");
		}
	}
}
