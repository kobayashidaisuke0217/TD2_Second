#include "PlayerAimBullet.h"

void PlayerAimBullet::Initialize(Vector3 velocity, Transform transform, uint32_t tex)
{
	sphere_ = std::make_unique<Sphere>();
	sphere_->Initialize();
	velocity_ = velocity;
	worldTransform_.translation_ = transform.translate;
	worldTransform_.rotation_ = transform.rotate;
	worldTransform_.scale_ = transform.scale;
	texture_ = tex;
	worldTransform_.Initialize();
}

void PlayerAimBullet::Update()
{
	worldTransform_.translation_ = Add(velocity_, worldTransform_.translation_);
	worldTransform_.UpdateMatrix();
}

void PlayerAimBullet::Draw(const ViewProjection& view)
{
	sphere_->Draw({ 1.0f,1.0f,1.0f,1.0f }, worldTransform_, texture_, view);
}

void PlayerAimBullet::isCollision()
{
}
