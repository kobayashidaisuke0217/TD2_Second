#include "BoundEnemy.h"

BoundEnemy::BoundEnemy()
{
}

BoundEnemy::~BoundEnemy()
{
}

void BoundEnemy::Initialize(const Transform& transform, const Vector3& velocity, float moveSpeed, uint32_t texture)
{
	texManager_ = Texturemanager::GetInstance();
	sphere_ = std::make_unique<Sphere>();
	sphere_->Initialize();

	worldTransform_.translation_ = transform.translate;
	worldTransform_.scale_ = transform.scale;
	worldTransform_.Initialize();
	MoveSpeed_ = moveSpeed;
	velocity_ = velocity;
	velocity_.x = std::clamp(velocity_.x, -1.0f, 1.0f);
	velocity_.y = std::clamp(velocity_.y, -1.0f, 1.0f);
	velocity_.z = std::clamp(velocity_.z, -1.0f, 1.0f);
	texindex_ = texture;
	velocity_ = Multiply(MoveSpeed_, velocity_);
	//velocity_.y = -MoveSpeed_;
	
	isAlive_ = true;
	ishit_ = false;
	cooltime_ = 0;

	reflectCount_ = 0;
	isDown_ = false;
	type_ = kBound;
}

void BoundEnemy::Update()
{
	prePos_ = worldTransform_.translation_;
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0.0f,0.0f,0.0f });
	obb_.size = { worldTransform_.scale_.x / 2.0f ,worldTransform_.scale_.y / 2.0f ,worldTransform_.scale_.z / 2.0f };
	obb_.center = worldTransform_.translation_;
	GetOrientations(rotateMatrix, obb_.orientation);
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	worldTransform_.UpdateMatrix();
	if (ishit_ == true) {
		cooltime_++;
	}
	if (cooltime_ >= 10) {
		ishit_ = false;
		cooltime_ = 0;
	}
	
	
		if (worldTransform_.translation_.y >= 11.0f /*&& velocity_.y >= MoveSpeed_*/) {
		
			isDown_ = true;
		}
		if (isDown_) {
			if (velocity_.y >= -MoveSpeed_) {
				velocity_.y -= MoveSpeed_/5.0f;
			}
			else {
				isDown_ = false;
			}
	}

	ImGui::Begin("enemy");
	ImGui::DragFloat3("velocity", &velocity_.x);
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x);

	ImGui::End();
	if (worldTransform_.translation_.y <= -5.0f) {
		isAlive_ = false;
	}if (std::abs(worldTransform_.translation_.x) > 150.0f) {
		isAlive_ = false;
	}
}

void BoundEnemy::Draw(const ViewProjection& viewProjection)
{
	sphere_->Draw({ 1.0f,1.0f,1.0f,1.0f }, worldTransform_, texindex_, viewProjection);
}

void BoundEnemy::isCollision(OBB pertner)
{
	if (!ishit_) {

		ishit_ = true;
		isDown_ = false;
		if (collisionpartner_ == kflore) {
			if (std::abs(obb_.center.y - pertner.center.y) <= worldTransform_.scale_.y * 2.0f) {
				velocity_.y *= -1.0f;
				worldTransform_.translation_ = prePos_;

			}
			else if (std::abs(obb_.center.x - pertner.center.x) <= worldTransform_.scale_.x * 2.0f) {

				velocity_.x *= -1.0f;
				worldTransform_.translation_ = prePos_;
			}
		}
		else {

			if (std::abs(obb_.center.x - pertner.center.x) <= worldTransform_.scale_.x * 2.0f) {

				velocity_.x *= -1.0f;
				worldTransform_.translation_ = prePos_;
			}
			else if (std::abs(obb_.center.y - pertner.center.y) <= worldTransform_.scale_.y * 2.0f) {
				velocity_.y *= -1.0f;
				worldTransform_.translation_ = prePos_;

			}
		}


	}
}
