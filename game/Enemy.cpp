#include "Enemy.h"
#include "ImguiManger.h"
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(const WorldTransform& transform,const float& velo )
{
	texManager_ = Texturemanager::GetInstance();
	sphere_ = std::make_unique<Sphere>();
	sphere_->Initialize();
	worldTransform_.translation_.y = 10.0f;
	worldTransform_.translation_.x = 10.0f;
	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };
	worldTransform_.Initialize();
	MoveSpeed_ = velo;
	targetWordTransform_ = transform;
	velocity_ = Subtract( targetWordTransform_.translation_, worldTransform_.translation_);
	velocity_ = Normalise(velocity_);
	
	velocity_ = Multiply(MoveSpeed_, velocity_);
	velocity_.y = -0.2f;
	texindex_ = texManager_->Load("resource/black.png");
	isAlive_ = true;
	ishit_ = false;
	cooltime_ = 0;
	
	reflectCount_ = 0;
	isDown_ = false;
}

void Enemy::Update()
{
	prePos_ = worldTransform_.translation_;
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0.0f,0.0f,0.0f });
	obb_.size = { worldTransform_.scale_.x ,worldTransform_.scale_.y ,worldTransform_.scale_.z  };
	obb_.center = worldTransform_.translation_;
	GetOrientations(rotateMatrix, obb_.orientation);
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	
	worldTransform_.UpdateMatrix();
	if (ishit_ == true) {
		cooltime_++;
	}
	if (cooltime_ >= 5) {
		ishit_ = false;
		cooltime_ = 0;
	}
	if (reflectCount_ >= 3) {
		isAlive_ = false;
	}
	if (worldTransform_.translation_.y >= 11.0f) {
		isDown_ = true;
		
	}
	if (isDown_ == true && velocity_.y >= -0.2f) {
		velocity_.y -= 0.005f;
		isDown_ = false;
	}
	ImGui::Begin("enemy");
	ImGui::DragFloat3("velocity", &velocity_.x);
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x);

	ImGui::End();
	if (worldTransform_.translation_.y <= -5.0f) {
		isAlive_ = false;
	}
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	sphere_->Draw({ 1.0f,1.0f,1.0f,1.0f }, worldTransform_, texindex_, viewProjection);
}

void Enemy::isCollision(OBB partner)
{
	//isAlive_ = false;
	if (!ishit_) {
		/*velocity_ = Reflect(velocity_, reflection_);
		velocity_ = Normalise(velocity_);
		velocity_ = Multiply(MoveSpeed_, velocity_);*/
		ishit_ = true;

		
			if (std::abs(obb_.center.x - partner.center.x) < std::abs(obb_.center.y - partner.center.y)) {
				velocity_.y *= -1.0f;
				worldTransform_.translation_ = prePos_;
				
			}
			
			else {
				velocity_.x *= -1.0f;
				
			}
			
	}
}
