#pragma once
#pragma region standardInclude
#include <string>
#pragma endregion
#include "DirectXCommon.h"
#include"MyMath.h"
#include "BlueMoon.h"
#include "worldTransform.h"
#include "ViewProjection.h"
#include "Texturemanager.h"
#include"DirectionalLight.h"
class Model
{
public:
	struct OutLineData {
		Vector4 color;
		Matrix4x4 scale;
	};
	void Initialize(const std::string& directoryPath, const std::string& filename);
	void Draw(const WorldTransform& transform, const ViewProjection& viewProjection);
	void OutLineDraw(const WorldTransform& transform, const ViewProjection& viewProjection);
	void Finalize();

	static Model* CreateModelFromObj(const std::string& directoryPath, const std::string& filename);
	ModelData modelData_;
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	void SetColor(Vector4 col) { color = col; }
	void SetOutLineColor(const Vector4& color) { outlineData_->color = color; };
	void SetOutLineWidth(const Vector3& wid) { outlineData_->scale = MakeScaleMatrix(wid); };

	void setIsLighting(bool flag) { material_->enableLighting = flag; }
private:
	Texturemanager* textureManager_;
	DirectXCommon* dxCommon_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	VertexData* vertexData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;
	Transformmatrix* wvpData_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Material* material_;
	BlueMoon* engine_;
	uint32_t texture_;
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	DirectionalLight* directionalLight_;
	/*D3D12_VERTEX_BUFFER_VIEW VBVOutline_;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResourceOutLine;
	VertexDataOutLine* vertexDataOutLine_;*/
	OutLineData* outlineData_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> outlineResource_;
private:
	void CreateVartexData();

	void SetColor();
	void TransformMatrix();
	void CreateOutLineData();

};

