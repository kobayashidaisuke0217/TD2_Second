struct VertexShaderOutput {
	float32_t4 position : SV_POSITION;
	float32_t2 texcoord : TEXCOORD;
	float32_t3 normal : NORMAL0;
	float32_t3 worldPosition : POSITION0;
	float32_t4 color : COLOR0;
};

struct TransformationMatrix {

	float32_t4x4 matWorld;
	float32_t4x4 WorldInverseTranspose;
};
struct ViewProjectionMatrix {
	float32_t4x4 view;
	float32_t4x4 projection;
	float32_t3 camera;
};
struct OutLineData {
	float32_t4 Color;
	float32_t4x4 Scale;
};