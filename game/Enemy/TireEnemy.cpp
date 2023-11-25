#include "TireEnemy.h"

TireEnemy::TireEnemy()
{
}

TireEnemy::~TireEnemy()
{
}

void TireEnemy::Initialize(const Transform& transform, const Vector3& velocity, float moveSpeed, uint32_t texture)
{
	sphere_ = std::make_unique<Sphere>();
	sphere_->Initialize();

	worldTransform_.Initialize();
	worldTransform_.translation_ = transform.translate;
	worldTransform_.scale_ = transform.scale;

	MoveSpeed_ = moveSpeed;
	velocity_ = velocity;
	velocity_.y = 0.0f;
	texindex_ = texture;
	velocity_ = Multiply(MoveSpeed_, velocity_);
	type_ = kBullet;
	isAlive_ = true;
	ishit_ = false;
}

void TireEnemy::Update()
{
	prePos_ = worldTransform_.translation_;
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0.0f,0.0f,0.0f });
	obb_.size = { worldTransform_.scale_.x / 2.0f ,worldTransform_.scale_.y / 2.0f ,worldTransform_.scale_.z / 2.0f };
	obb_.center = worldTransform_.translation_;
	GetOrientations(rotateMatrix, obb_.orientation);
	if (!ishit_) {
		velocity_.y = -0.5f;
	}
	else {
		velocity_.y = 0.0f;
	}
	worldTransform_.rotation_.y += velocity_.x;
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
	ishit_ = false;
}

void TireEnemy::Draw(const ViewProjection& viewProjection)
{
	sphere_->Draw({ 1.0f,1.0f,1.0f,1.0f }, worldTransform_, texindex_, viewProjection);
}

void TireEnemy::isCollision(OBB pertner)
{
	
		if (obb_.center.y + obb_.size.y > pertner.center.y + pertner.size.y) {
			ishit_ = true;
			worldTransform_.translation_.y = prePos_.y;
		}
	
	else if (std::abs(obb_.center.x - pertner.center.x) <= worldTransform_.scale_.x * 2.0f) {
		if (std::abs(obb_.center.x - pertner.center.x) <= worldTransform_.scale_.x * 2.0f) {
			velocity_.x *= -1.0f;
			worldTransform_.translation_.x = prePos_.x;
		}
	}
}
