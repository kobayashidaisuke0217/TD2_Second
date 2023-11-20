#include "AimBulletEnemy.h"
#include "game/Player.h"
#include "game/Scenes/gameScene.h"
AimBulletEnemy::AimBulletEnemy()
{
}

AimBulletEnemy::~AimBulletEnemy()
{
}

void AimBulletEnemy::Initialize(const Transform& transform, const Vector3& velocity, float moveSpeed, uint32_t texture){
	sphere_ = std::make_unique<Sphere>();
	sphere_->Initialize();

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
	type_ = kAimbulletheight;
	isAlive_ = true;
	atackCount_ = 5;
	coolTime_ = 60;
}

void AimBulletEnemy::Update()
{
  currentCollTime_++;
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
	sphere_->Draw({ 1.0f,1.0f,1.0f,1.0f }, worldTransform_, texindex_, viewProjection);
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
	newBullet->Initialize({ 0.0f,-1.0f,0.0f }, { worldTransform_.scale_,worldTransform_.rotation_,worldTransform_.translation_ }, texindex_);
	gameScene_->AddEnemyBullet(newBullet);
}