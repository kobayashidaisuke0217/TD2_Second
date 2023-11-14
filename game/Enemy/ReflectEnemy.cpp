#include "ReflectEnemy.h"

ReflectEnemy::ReflectEnemy()
{
}

ReflectEnemy::~ReflectEnemy()
{
}

void ReflectEnemy::Initialize(const Transform& transform, const Vector3& velocity, float moveSpeed, uint32_t texture, const WorldTransform& targettransform)
{
	texManager_ = Texturemanager::GetInstance();
	sphere_ = std::make_unique<Sphere>();
	sphere_->Initialize();

	
	worldTransform_.translation_ = transform.translate;
	worldTransform_.scale_ = transform.scale;
	worldTransform_.Initialize();
	MoveSpeed_ = moveSpeed;
	targetWordTransform_ = targettransform;
	velocity_ = velocity;
	velocity_.x = std::clamp(velocity_.x, -1.0f, 1.0f);
	velocity_.y = std::clamp(velocity_.y, -1.0f, 1.0f);
	velocity_.z = std::clamp(velocity_.z, -1.0f, 1.0f);
	texindex_ = texture;
	velocity_ = Multiply(MoveSpeed_, velocity_);
	isAlive_ = true;
	ishit_ = false;
	cooltime_ = 0;

	reflectCount_ = 0;
	isDown_ = false;
	type_ = kReflect;
}

void ReflectEnemy::Update()
{
	prePos_ = worldTransform_.translation_;
		Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0.0f,0.0f,0.0f });
		obb_.size = { worldTransform_.scale_.x/2.0f ,worldTransform_.scale_.y/2.0f ,worldTransform_.scale_.z/2.0f  };
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
		
			if (reflectCount_ >= 4) {
				isAlive_ = false;
			}
		
		
			
		
		
		ImGui::Begin("enemy");
		ImGui::DragFloat3("velocity", &velocity_.x);
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x);
	
		ImGui::End();
		
}

void ReflectEnemy::Draw(const ViewProjection& viewProjection)
{
	sphere_->Draw({ 1.0f,1.0f,1.0f,1.0f }, worldTransform_, texindex_, viewProjection);
}

void ReflectEnemy::isCollision(OBB pertner)
{
	if (!ishit_) {
				
				ishit_ = true;
				reflectCount_++;
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
					}else if (std::abs(obb_.center.y - pertner.center.y) <= worldTransform_.scale_.y * 2.0f) {
						velocity_.y *= -1.0f;
						worldTransform_.translation_ = prePos_;
		
					}
				}
					
					
			}
}
