//#include "Enemy.h"
//#include "ImguiManger.h"
//Enemy::Enemy()
//{
//}
//
//Enemy::~Enemy()
//{
//}
//
//void Enemy::Initialize(const WorldTransform& transform,const float& velo,  uint32_t Texture, ReflectionCount reflection)
//{
//	texManager_ = Texturemanager::GetInstance();
//	sphere_ = std::make_unique<Sphere>();
//	sphere_->Initialize();
//	worldTransform_.translation_.y = 10.0f;
//	worldTransform_.translation_.x = 10.0f;
//	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
//	worldTransform_.Initialize();
//	MoveSpeed_ = velo;
//	targetWordTransform_ = transform;
//	velocity_ = Subtract( targetWordTransform_.translation_, worldTransform_.translation_);
//	velocity_ = Normalise(velocity_);
//	texindex_ = Texture;
//	velocity_ = Multiply(MoveSpeed_, velocity_);
//	velocity_.y = -0.2f;
//	reflectionCount_ = reflection;
//	isAlive_ = true;
//	ishit_ = false;
//	cooltime_ = 0;
//	
//	reflectCount_ = 0;
//	isDown_ = false;
//}
//
//void Enemy::Update()
//{
//	prePos_ = worldTransform_.translation_;
//	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0.0f,0.0f,0.0f });
//	obb_.size = { worldTransform_.scale_.x/2.0f ,worldTransform_.scale_.y/2.0f ,worldTransform_.scale_.z/2.0f  };
//	obb_.center = worldTransform_.translation_;
//	GetOrientations(rotateMatrix, obb_.orientation);
//	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
//	
//	worldTransform_.UpdateMatrix();
//	if (ishit_ == true) {
//		cooltime_++;
//	}
//	if (cooltime_ >= 10) {
//		ishit_ = false;
//		cooltime_ = 0;
//	}
//	if (reflectionCount_ == reflect4) {
//		if (reflectCount_ >= 4) {
//			isAlive_ = false;
//		}
//	}
//	if (reflectionCount_ == ReflectInfinit) {
//		if (worldTransform_.translation_.y >= 11.0f && velocity_.y >= -MoveSpeed_) {
//			isDown_ = true;
//			velocity_.y -= 0.005f;
//		}
//	}
//	
//	ImGui::Begin("enemy");
//	ImGui::DragFloat3("velocity", &velocity_.x);
//	ImGui::DragFloat3("translate", &worldTransform_.translation_.x);
//
//	ImGui::End();
//	if (worldTransform_.translation_.y <= -5.0f) {
//		isAlive_ = false;
//	}
//}
//
//void Enemy::Draw(const ViewProjection& viewProjection)
//{
//	sphere_->Draw({ 1.0f,1.0f,1.0f,1.0f }, worldTransform_, texindex_, viewProjection);
//}
//
//void Enemy::isCollision(OBB partner)
//{
//	//isAlive_ = false;
//	if (!ishit_) {
//		
//		ishit_ = true;
//		reflectCount_++;
//		if (collisionpartner_ == flore) {
//			if (std::abs(obb_.center.y - partner.center.y) <= 2.0f) {
//				velocity_.y *= -1.0f;
//				worldTransform_.translation_ = prePos_;
//
//			}
//			else if (std::abs(obb_.center.x - partner.center.x) <= 2.0f) {
//
//				velocity_.x *= -1.0f;
//				worldTransform_.translation_ = prePos_;
//			}
//		}
//		else {
//			if (std::abs(obb_.center.x - partner.center.x) <= 2.0f) {
//
//				velocity_.x *= -1.0f;
//				worldTransform_.translation_ = prePos_;
//			}else if (std::abs(obb_.center.y - partner.center.y) <= 2.0f) {
//				velocity_.y *= -1.0f;
//				worldTransform_.translation_ = prePos_;
//
//			}
//		}
//			
//			
//	}
//}