#pragma once

#ifdef _DEBUG

#include <vector>
#include <filesystem>

#include <Library/Math/Vector3.h>

#include "Engine/Rendering/DirectX/DirectXResourceObject/VertexBuffer/VertexBuffer.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/IndexBuffer/IndexBuffer.h"

class LineGroupResource final {
public:
	LineGroupResource() = default;
	~LineGroupResource() = default;

	LineGroupResource(std::filesystem::path fileName);

	LineGroupResource(
		std::vector<Vector3> vertices_,
		std::vector<uint32_t> indices
	);

public:
	/// <summary>
	/// VertexBufferViewを取得
	/// </summary>
	/// <returns>VertexBufferView Pointer</returns>
	const D3D12_VERTEX_BUFFER_VIEW& get_vbv() const;

	/// <summary>
	/// IndexBufferViewを取得
	/// </summary>
	/// <returns>IndexBufferView Pointer</returns>
	const D3D12_INDEX_BUFFER_VIEW* const get_p_ibv() const;

	/// <summary>
	/// IndexBufferのサイズ
	/// </summary>
	/// <returns>UINT型で返す</returns>
	const UINT index_size() const;

private:
	VertexBuffer<Vector3> vertexBuffer;
	IndexBuffer indexBuffer;
};

#endif // _DEBUG
