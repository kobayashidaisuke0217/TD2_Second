#pragma once
#include <math.h>
#include<stdint.h>
#include<cassert>
#include <vector>
#include <string>
#include <cmath>
#define _USE_MATH_DEFINES
#include <algorithm>
struct Vector4 {
	float x;
	float y;
	float z;
	float w;
};
struct Vector3 {
	float x;
	float y;
	float z;
};
struct Vector2 {
	float x;
	float y;
};
struct Matrix4x4 final {
	float m[4][4];
};
struct Matrix3x3 {
	float m[3][3];
};
struct Transform
{
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};
struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};
struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};
struct Transformmatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};
struct DirectionalLightData {
	Vector4 color;//ライトの色
	Vector3 direction;//ライトの向き
	float intensity;//輝度
};

struct MaterialData {
	std::string textureFilePath;
};
struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};
struct AABB {
	Vector3 min;
	Vector3 max;
};

struct OBB {
	Vector3 center;
	Vector3 orientation[3];
	Vector3 size;
};
struct StructSphere {
	Vector3 center;
	float radius;
};
inline Vector3 operator-(const Vector3& v) {
	return { -v.x, -v.y, -v.z };
}
inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

inline Vector3 operator*(const Vector3& v, float s) {
	return { v.x * s, v.y * s, v.z * s };
}
inline Vector3 operator*(float s, const Vector3& v) {
	return { s * v.x, s * v.y, s * v.z };
}

inline Vector3 operator*(const Vector3& v, const Matrix4x4& m) {
	return {
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2] };
}
Matrix4x4 MakeRotateXMatrix(float theta);
Matrix4x4 MakeRotateYMatrix(float theta);

Matrix4x4 MakeRotateZMatrix(float theta);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
//1　行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
//２　行列の減法
Matrix4x4 Sub(const Matrix4x4& m1, const Matrix4x4& m2);
//３　行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
//４　逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
//５　転置行列
Matrix4x4 Transpose(const Matrix4x4& m);
//６　単位行列
Matrix4x4 MakeIdentity4x4();
//透視投射行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
float cot(float theta);
// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
//ビューポート行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

Vector3 Normalise(const Vector3& v);
Vector3 Add(const Vector3& a, const Vector3& b);


Vector3 Subtract(const Vector3& v1, const Vector3& v2);
Vector3 Multiply(float scalar, const Vector3& v);
float Dot(const Vector3& v1, const Vector3& v2);
float Length(const Vector3& v);
Vector3 vectorTransform(const Vector3& vector, const Matrix4x4& matrix);
Vector3 Slerp(float t, const Vector3& s, const Vector3& e);
Vector3 Lerp(float t, const Vector3& s, const Vector3& e);
Vector3 Distance(const Vector3& v1, const Vector3& v2);
Matrix4x4 MakeRotateMatrix(Vector3 theta);
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
Vector3 Cross(const Vector3& v1, const Vector3& v2);
/// obbの衝突判定
inline Matrix4x4& SetTranslate(Matrix4x4& m, const Vector3& v) {
	m.m[3][0] = v.x, m.m[3][1] = v.y, m.m[3][2] = v.z;
	return m;
}

inline Matrix4x4 MakeInverseMatrix(const Matrix4x4& rotate, const Vector3& translate) {
	Matrix4x4 RT = Transpose(rotate);
	return SetTranslate(RT, -translate * RT);
}
inline Matrix4x4 MakeRotateMatrixFromOrientations(const Vector3 orientations[3]) {
	return {
		orientations[0].x,orientations[0].y,orientations[0].z,0.0f,
		orientations[1].x,orientations[1].y,orientations[1].z,0.0f,
		orientations[2].x,orientations[2].y,orientations[2].z,0.0f,
		0.0f,0.0f,0.0f,1.0f };
}
inline bool IsCollision(const AABB& aabb, const StructSphere& sphere) {
	Vector3 clossestPoint{
		std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
		std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
		std::clamp(sphere.center.z, aabb.min.z, aabb.max.z)
	};

	float distance = Length(Subtract(clossestPoint, sphere.center));

	return distance <= sphere.radius;
}
inline bool IsCollision(const OBB& obb, const StructSphere& sphere) {
	Matrix4x4 obbWorldInverse = MakeInverseMatrix(MakeRotateMatrixFromOrientations(obb.orientation), obb.center);
	Vector3 centerInOBBLocalSpace = sphere.center * obbWorldInverse;
	AABB aabbOBBLocal{ .min = -obb.size, .max = obb.size };
	StructSphere sphereObbLocal{ centerInOBBLocalSpace, sphere.radius };

	return IsCollision(aabbOBBLocal, sphereObbLocal);
}
inline Vector3 GetXAxis(const Matrix4x4& m) {
	return { m.m[0][0],m.m[0][1],m.m[0][2] };
}
inline Vector3 GetYAxis(const Matrix4x4& m) {
	return { m.m[1][0],m.m[1][1],m.m[1][2] };
}inline Vector3 GetZAxis(const Matrix4x4& m) {
	return { m.m[2][0],m.m[2][1],m.m[2][2] };
}
inline void GetOrientations(const Matrix4x4& m, Vector3 orientations[3]) {
	orientations[0] = GetXAxis(m);
	orientations[1] = GetYAxis(m);
	orientations[2] = GetZAxis(m);
}
inline Matrix4x4 MakeRotateXYZMatrix(const Vector3& rotate) {
	Vector3 s = { std::sin(rotate.x), std::sin(rotate.y), std::sin(rotate.z) };
	Vector3 c = { std::cos(rotate.x), std::cos(rotate.y), std::cos(rotate.z) };
	return {
		c.y * c.z,						c.y * s.z,						-s.y,		0.0f,
		s.x * s.y * c.z - c.x * s.z,	s.x * s.y * s.z + c.x * c.z,	s.x * c.y,	0.0f,
		c.x * s.y * c.z + s.x * s.z,	c.x * s.y * s.z - s.x * c.z,	c.x * c.y,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f };
}
inline Vector3 Reflect(const Vector3& input, const Vector3& normal) {
	Vector3 r;
	r = input - 2 * Dot(input, normal) * normal;
	r = r * 0.8f;
	return r;
}
