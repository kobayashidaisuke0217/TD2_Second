#pragma once
#include"IEnemy.h"
class BoundEnemy :public IEnemy
{
public:
	BoundEnemy();
	~BoundEnemy();
	virtual void Initialize(const Transform& transform, const Vector3& velocity, float moveSpeed, uint32_t texture, const WorldTransform& targettransform) override;
	virtual void Update() override;
	virtual void Draw(const ViewProjection& viewProjection) override;
	virtual void isCollision(OBB pertner) override;
private:

};

