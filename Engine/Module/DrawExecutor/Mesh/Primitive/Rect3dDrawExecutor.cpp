#include "Rect3dDrawExecutor.h"

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/Texture/Texture.h"

#include "Engine/Module/World/Mesh/Primitive/Rect3d.h"

#include "Engine/Assets/PrimitiveGeometry/PrimitiveGeometryAsset.h"
#include "Engine/Assets/PrimitiveGeometry/PrimitiveGeometryLibrary.h"
#include "Engine/Assets/Texture/TextureLibrary.h"

void Rect3dDrawExecutor::reinitialize(PrimitiveType type_, uint32_t maxInstance_) {
	type = type_;
	maxInstance = maxInstance_;
	matrices.initialize(maxInstance);
	rectData.initialize(maxInstance);
	material.initialize(maxInstance);
}

void Rect3dDrawExecutor::draw_command() const {
	if (instanceCounter == 0) {
		return;
	}

	auto& commandList = DxCommand::GetCommandList();
	commandList->IASetVertexBuffers(0, 0, nullptr);
	auto geometry = PrimitiveGeometryLibrary::GetPrimitiveGeometry("Rect3D");
	commandList->IASetIndexBuffer(geometry->get_p_ibv());
	commandList->SetGraphicsRootDescriptorTable(0, matrices.get_handle_gpu());
	commandList->SetGraphicsRootDescriptorTable(1, rectData.get_handle_gpu());
	commandList->SetGraphicsRootDescriptorTable(2, material.get_handle_gpu());

	commandList->DrawIndexedInstanced(6, instanceCounter, 0, 0, 0);
}

void Rect3dDrawExecutor::write_to_buffer(Reference<const Rect3d> instance) {
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
	rectData[next] = instance->get_data();
	const PrimitiveMaterial& source = instance->get_material();
	material[next] = {
		source.color,
		source.lightingType,
		source.shininess,
		0,
		source.uvTransform.get_matrix(),
	};
	if (!source.texture) {
		material[next].textureIndex = TextureLibrary::GetTexture("Error.png")->index().value();
	}
	else if (source.texture->index().has_value()) {
		material[next].textureIndex = source.texture->index().value();
	}
	else {
		material[next].textureIndex = TextureLibrary::GetTexture("Error.png")->index().value();
	}
}

