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
	isAlive_ = true;
}

void PlayerAimBullet::Update()
{
	worldTransform_.translation_ = Add(velocity_, worldTransform_.translation_);
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0.0f,0.0f,0.0f });
	obb_.size = { worldTransform_.scale_.x / 2.0f ,worldTransform_.scale_.y / 2.0f ,worldTransform_.scale_.z / 2.0f };
	obb_.center = worldTransform_.translation_;
	GetOrientations(rotateMatrix, obb_.orientation);
	worldTransform_.UpdateMatrix();
}

void PlayerAimBullet::Draw(const ViewProjection& view)
{
	sphere_->Draw({ 1.0f,1.0f,1.0f,1.0f }, worldTransform_, texture_, view);
}

void PlayerAimBullet::isCollision()
{
	isAlive_ = false;
}
