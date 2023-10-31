#pragma once
#pragma region standardInclude
#include <wrl.h>
#include <array>
#pragma endregion
#include "DirectXCommon.h"
#include "externals/DirectXTex/DirectXTex.h"
#include"externals/DirectXTex/d3dx12.h"
class Texturemanager
{
public:
	void Initialize();
	static Texturemanager* GetInstance();
	static const int maxtex = 256;
	const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t textureHandle);

	uint32_t Load(const std::string& filePath );	

private:
	DirectXCommon* dirctXCommon_;
	Microsoft::WRL::ComPtr<ID3D12Resource>intermediateResource[maxtex];
	Microsoft::WRL::ComPtr<ID3D12Resource>textureResource[maxtex];
	uint32_t descriptorSizeSRV;
	uint32_t descriptorSizeRTV;
	uint32_t descriptorSizeDSV;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_[maxtex];
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_[maxtex];
	uint32_t textureIndex_;
private:
	D3D12_CPU_DESCRIPTOR_HANDLE GettextureSrvHandleCPU(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GettextureSrvHandleGPU(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index);
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadtextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, uint32_t index);
	DirectX::ScratchImage  LoadTexture(const std::string& filePath);
	void LoadTexture(const std::string& filePath, uint32_t index);
};

