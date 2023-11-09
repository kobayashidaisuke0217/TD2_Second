#pragma once
#include "worldTransform.h"
#include "viewProjection.h"
#include "Sphere.h"
#include <memory>
#include "textureManager.h"
enum ReflectionCount {
	ReflectInfinit,
	reflect4,
};
enum collisionPartner {
	kwall,
	kflore,
};
enum EnemyType {
	kBullet,
	kReflect,
	kBound,
	kTire,
	kSpear,
	kRaser,
	kAimBullet,
	kAimBound,
	kStageUp,
	kStageDown,
	kHoming
};
class IEnemy
{
public:
	IEnemy();
	~IEnemy();
	virtual void Initialize(const Transform& transform, const Vector3& velocity,float moveSpeed,uint32_t texture,const WorldTransform& targettransform)=0;
	virtual void Update()=0;
	virtual void Draw(const ViewProjection& viewProjection)=0;
	virtual void isCollision(OBB pertner)=0;
	virtual bool GetIsAlive() { return isAlive_; }
	virtual OBB& GetOBB() { return obb_; };
	
	
	virtual bool GetIsHit() { return ishit_; }
	virtual void SetPartener(collisionPartner partner) { collisionpartner_ = partner; }
	virtual EnemyType GetType() { return type_; }
protected:
	Texturemanager* texManager_;
	std::unique_ptr<Sphere> sphere_;
	ReflectionCount reflectionCount_;
	WorldTransform targetWordTransform_;
	Vector3 velocity_;
	float MoveSpeed_;
	uint32_t texindex_;
	bool isAlive_;
	OBB obb_;
	bool ishit_;
	int cooltime_;
	int reflectCount_;
	Vector3 prePos_;
	bool isDown_;
	collisionPartner collisionpartner_;
	EnemyType type_;
	WorldTransform worldTransform_;
};

