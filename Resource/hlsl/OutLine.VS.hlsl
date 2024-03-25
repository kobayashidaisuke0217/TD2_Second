#include "OutLine.hlsli"

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
ConstantBuffer<ViewProjectionMatrix> gViewProjectionMatrix : register(b1);

ConstantBuffer<OutLineData> gOutLineData : register(b2);


struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
	
};

VertexShaderOutput main(VertexShaderInput input) {


	VertexShaderOutput output;
	float32_t4x4 wvp= mul(mul(gTransformationMatrix.matWorld , gViewProjectionMatrix.view), gViewProjectionMatrix.projection);
	 wvp = mul(gOutLineData.Scale,wvp);
	output.position = mul(input.position, wvp);
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float32_t3x3)gTransformationMatrix.WorldInverseTranspose)); output.color = gOutLineData.Color;
	return output;
}