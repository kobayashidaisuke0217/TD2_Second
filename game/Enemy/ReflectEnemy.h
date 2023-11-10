#pragma once
#include"IEnemy.h"
class ReflectEnemy: public IEnemy
{
public:
	ReflectEnemy();
	~ReflectEnemy();
	virtual void Initialize(const Transform& transform, const Vector3& velocity, float moveSpeed, uint32_t texture, const WorldTransform& targettransform) override;
	virtual void Update() override;
	virtual void Draw(const ViewProjection& viewProjection) override;
	virtual void isCollision(OBB pertner) override;
};

