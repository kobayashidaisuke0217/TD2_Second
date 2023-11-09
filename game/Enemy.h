#pragma once
#include "worldTransform.h"
#include "viewProjection.h"
#include "Sphere.h"
#include <memory>
#include "textureManager.h"
enum ReflectionCount{
	ReflectInfinit,
	reflect4,
};
enum collisionPartner {
	wall,
	flore,
};
class Enemy
{
public:
	Enemy();
	~Enemy();
	void Initialize(const WorldTransform& transform, const float& velo,uint32_t Texture,ReflectionCount reflection);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	bool GetIsAlive() { return isAlive_; }
	OBB& GetOBB() { return obb_; };
	void isCollision(OBB pertner);
	WorldTransform worldTransform_;
	void SetReflection(Vector3 reflect) { reflection_ = reflect; }
	bool GetIsHit() { return ishit_; }
	ReflectionCount getReflection() { return reflectionCount_; }
	void SetPartener(collisionPartner partner) { collisionpartner_ = partner; }
private:
	Texturemanager* texManager_;
	std::unique_ptr<Sphere> sphere_;
	ReflectionCount reflectionCount_;
	WorldTransform targetWordTransform_;
	Vector3 velocity_;
	 float MoveSpeed_;
	uint32_t texindex_;
	bool isAlive_;
	OBB obb_;
	Vector3 reflection_;
	Vector3 acceleration_;
	Vector3 gravity_;
	bool ishit_;
	int cooltime_;
	int reflectCount_;
	Vector3 prePos_;
	bool isDown_;
	collisionPartner collisionpartner_;
};

