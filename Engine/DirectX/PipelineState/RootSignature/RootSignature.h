#pragma once

#include <d3d12.h>
#include <wrl/client.h>

class RootSignature {
public:
	RootSignature() = default;
	~RootSignature() = default;

private:
	RootSignature(const RootSignature&) = delete;
	RootSignature operator=(const RootSignature&) = delete;

public:
	void initialize();
	Microsoft::WRL::ComPtr<ID3D12RootSignature>& get_root_signature();
	const Microsoft::WRL::ComPtr<ID3D12RootSignature>& get_root_signature() const;

private:
	void create_root_signature();

private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
};
