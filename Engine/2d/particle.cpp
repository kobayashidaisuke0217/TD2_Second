#include "particle.h"
#include <assert.h>
#include "BlueMoon.h"

void Particle::Initialize()
{
	direct_ = DirectXCommon::GetInstance();

	textureManager_ = Texturemanager::GetInstance();
	srvHeap_ = SrvDescriptorHeap::GetInstance();
	directionalLight_ = DirectionalLight::GetInstance();
	SettingVertex();
	SetColor();
	TransformMatrix();
}
void Particle::TransformMatrix()
{

	instancingResource_ = direct_->CreateBufferResource(direct_->GetDevice().Get(), sizeof(Matrix4x4) * kNumInstance_);
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));
	CreateSRV(1);
	for (uint32_t index = 0; index < kNumInstance_; ++index) {
		instancingData[index] = MakeIdentity4x4();
		transforms[index].scale = { 1.0f,1.0f,1.0f };
		transforms[index].rotate = { 0.0f,0.0f,0.0f };
		transforms[index].translate = { index * 0.1f,index * 0.1f,index * 0.1f };

	}
}
void Particle::CreateSRV(uint32_t num)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;//2Dテクスチャ
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = kNumInstance_;
	srvDesc.Buffer.StructureByteStride = sizeof(Matrix4x4);
	srvHeap_->AddIndex();
	srvHeap_->SetGPUHandle(direct_->GetSrvHeap().Get(), srvHeap_->GetSizeSRV(), srvHeap_->GetIndex());//direct_->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	srvHeap_->SetGPUHandle(direct_->GetSrvHeap().Get(), srvHeap_->GetSizeSRV(), srvHeap_->GetIndex());//srvHandleCPU_ = textureManager_->GettextureSrvHandleCPU(direct_->GetSrvHeap().Get(), textureManager_->GetSizeSRV(), num);
	//srvHandleGPU_ = textureManager_->GettextureSrvHandleGPU(direct_->GetSrvHeap().Get(), textureManager_->GetSizeSRV(), num);
	direct_->GetDevice().Get()->CreateShaderResourceView(instancingResource_.Get(), &srvDesc, srvHeap_->GetCPUHandle(srvHeap_->GetIndex()));
}
void Particle::SetColor() {
	materialResource_ = DirectXCommon::CreateBufferResource(direct_->GetDevice().Get(), sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

}

void Particle::Draw(const WorldTransform& transform, const ViewProjection& viewProjection, const Vector4& material, uint32_t index)
{
	Transform uvTransform = { { 1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} };

	Matrix4x4 uvtransformMtrix = MakeScaleMatrix(uvTransform.scale);
	uvtransformMtrix = Multiply(uvtransformMtrix, MakeRotateZMatrix(uvTransform.rotate.z));
	uvtransformMtrix = Multiply(uvtransformMtrix, MakeTranslateMatrix(uvTransform.translate));
	for (uint32_t i = 0; i < kNumInstance_; ++i) {
		instancingData[i] = MakeAffineMatrix(transforms[i].scale, transforms[i].rotate, transforms[i].translate);
		instancingData[i] = Multiply(instancingData[i], transform.matWorld_);
	}
	*materialData_ = { material,false };
	materialData_->uvTransform = uvtransformMtrix;

	direct_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
	//形状を設定。PS0にせっていしているものとはまた別。同じものを設定すると考えておけばいい
	direct_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//material
	direct_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//worldTransform
	direct_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvHandleGPU_);

	direct_->GetCommandList()->SetGraphicsRootConstantBufferView(4, viewProjection.constBuff_->GetGPUVirtualAddress());
	//Light
	direct_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLight_->GetResource()->GetGPUVirtualAddress());

	//texture
	direct_->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->GetGPUHandle(index));


	//描画！(DrawCall/ドローコール)・3頂点で1つのインスタンス。インスタンスについては今後
	direct_->GetCommandList()->DrawInstanced(6, kNumInstance_, 0, 0);

}
void Particle::Finalize()
{

}

void Particle::SettingVertex() {

	vertexResource_ = DirectXCommon::CreateBufferResource(direct_->GetDevice().Get(), sizeof(VertexData) * 6);
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	vertexData_[0].position = { -0.5f,-0.5f,0.0f,1.0f };

	//上
	vertexData_[1].position = { -0.5f,0.5f,0.0f,1.0f };

	//右下
	vertexData_[2].position = { 0.5f,-0.5f,0.0f,1.0f };

	vertexData_[3].position = { -0.5f,0.5,0.0f,1.0f };
	vertexData_[4].position = { 0.5f,0.5f,0.0f,1.0f };
	vertexData_[5].position = { 0.5f,-0.5f,0.0f,1.0f };
	vertexData_[0].texcoord = { 0.0f,1.0f };
	vertexData_[1].texcoord = { 0.0f,0.0f };
	vertexData_[2].texcoord = { 1.0f,1.0f };
	vertexData_[3].texcoord = { 0.0f,0.0f };
	vertexData_[4].texcoord = { 1.0f,0.0f };
	vertexData_[5].texcoord = { 1.0f,1.0f };
	for (int i = 0; i < 6; i++) {
		vertexData_[i].normal = { 0.0f,0.0f,-1.0f };
	}
}