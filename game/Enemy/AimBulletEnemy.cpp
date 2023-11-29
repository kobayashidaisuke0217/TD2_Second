#include "AimBulletEnemy.h"
#include "game/Player.h"
#include "game/Scenes/gameScene.h"
AimBulletEnemy::AimBulletEnemy()
{
}

AimBulletEnemy::~AimBulletEnemy()
{
}

void AimBulletEnemy::Initialize(const Transform& transform, const Vector3& velocity, float moveSpeed, uint32_t texture,  Model* model){
	/*sphere_ = std::make_unique<Sphere>();
	sphere_->Initialize();*/
	mainmodel.reset(Model::CreateModelFromObj("Resource/Enemy", "bulletMother.obj"));
	worldTransform_.Initialize();
	worldTransform_.translation_ = transform.translate;
	worldTransform_.scale_ = transform.scale;
	
	MoveSpeed_ = moveSpeed;
	velocity_ = { 0.0f,-1.0f,0.0f };
	velocity_.x = std::clamp(velocity_.x, -1.0f, 1.0f);
	velocity_.y = std::clamp(velocity_.y, -1.0f, 1.0f);
	velocity_.z = std::clamp(velocity_.z, -1.0f, 1.0f);
	texindex_ = texture;
	velocity_ = Multiply(MoveSpeed_, velocity_);
	type_ = kAimBulletHeight;
	isAlive_ = true;
	atackCount_ = 5;
	coolTime_ = 60;
	model_ = model;
	model_->setIsLighting(false);
}

void AimBulletEnemy::Update()
{
  currentCollTime_++;
  worldTransform_.rotation_.y += currentAtackCount_ / 15.0f;
  if (player_) {
		worldTransform_.translation_.x = player_->GetWorldTransform().translation_.x;
	}
	if(currentAtackCount_<=atackCount_)
	if (currentCollTime_>=coolTime_) {
		Atack();
		currentCollTime_ = 0;
		currentAtackCount_++;
	}
	if (currentAtackCount_ >= atackCount_) {
		isAlive_ = false;
	}
	worldTransform_.UpdateMatrix();
}

void AimBulletEnemy::Draw(const ViewProjection& viewProjection)
{
	//sphere_->Draw({ 1.0f,1.0f,1.0f,1.0f }, worldTransform_, texindex_, viewProjection);
	mainmodel->Draw(worldTransform_, viewProjection);
}


void AimBulletEnemy::isCollision(OBB pertner)
{
}

void AimBulletEnemy::TextureInitialize()
{
	
		
	
}

void AimBulletEnemy::Atack()
{
	PlayerAimBullet* newBullet = new PlayerAimBullet();
	newBullet->Initialize(Multiply(MoveSpeed_,{ 0.0f,-1.0f,0.0f }), { {1.5f,2.0f,2.0f},worldTransform_.rotation_,worldTransform_.translation_}, texindex_, model_);
	gameScene_->AddEnemyBullet(newBullet);
}
