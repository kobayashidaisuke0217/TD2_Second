#include "BeamEnemy.h"

BeamEnemy::BeamEnemy()
{
}

BeamEnemy::~BeamEnemy()
{
}

void BeamEnemy::Initialize(const Transform& transform, const Vector3& velocity, float moveSpeed, uint32_t texture)
{
	sphere_ = std::make_unique<Sphere>();
	sphere_->Initialize();

	worldTransform_.Initialize();
	worldTransform_.translation_ = transform.translate;
	worldTransform_.scale_ = transform.scale;

	MoveSpeed_ = moveSpeed;
	velocity_ = velocity;
	velocity_.x = std::clamp(velocity_.x, -1.0f, 1.0f);
	velocity_.y = std::clamp(velocity_.y, -1.0f, 1.0f);
	velocity_.z = std::clamp(velocity_.z, -1.0f, 1.0f);
	texindex_ = texture;
	velocity_ = Multiply(MoveSpeed_, velocity_);
	type_ = kRaser;
	isAlive_ = true;
}

void BeamEnemy::Update()
{
	count_++;
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0.0f,0.0f,0.0f });
	obb_.size = { worldTransform_.scale_.x / 2.0f ,worldTransform_.scale_.y  ,worldTransform_.scale_.z / 2.0f };
	obb_.center = worldTransform_.translation_;
	GetOrientations(rotateMatrix, obb_.orientation);
	if (!ishit_) {
		worldTransform_.scale_ = Add(worldTransform_.scale_, velocity_);
	}
	else {
		worldTransform_.scale_.y = worldTransform_.translation_.y- target_.translate.y;
	}
	worldTransform_.UpdateMatrix();
	if (count_ >= 60) {
		isAlive_ = false;
	}
}

void BeamEnemy::Draw(const ViewProjection& viewProjection)
{
	sphere_->Draw({ 1.0f,1.0f,1.0f,1.0f }, worldTransform_, texindex_, viewProjection);
}

void BeamEnemy::isCollision(OBB pertner)
{
	ishit_ = true;
	target_.translate = pertner.center;
	target_.scale = pertner.size;

}
