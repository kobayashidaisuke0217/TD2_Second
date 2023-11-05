#pragma once
#include "DirectXCommon.h"
#include"MyMath.h"
#include "BlueMoon.h"
#include "DirectionalLight.h"
#include "TextureManager.h"
#include "Worldtransform.h"
#include "ViewProjection.h"
#include "Descriptorheap.h"
class Particle
{
public:
	void Initialize();
	void Draw(const WorldTransform& transform, const ViewProjection& viewProjection, const Vector4& material, uint32_t index);
	void Finalize();

private:
	void SettingVertex();
	void SetColor();
	void TransformMatrix();
	void CreateSRV(uint32_t num);
private:
	Texturemanager* textureManager_;
	SrvDescriptorHeap* srvHeap_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	DirectXCommon* direct_;
	VertexData* vertexData_;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	Material* materialData_;
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;
	Transformmatrix* wvpData_;
	DirectionalLight* directionalLight_;
	static	const uint32_t kNumInstance_ = 10;
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_;
	Matrix4x4* instancingData;
	Transform transforms[kNumInstance_];
	uint32_t index_;
};

