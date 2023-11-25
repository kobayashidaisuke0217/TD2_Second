#include "AImBulletWidthEnemy.h"
#include "game/Player.h"
#include "game/Scenes/gameScene.h"
AImBulletWidthEnemy::AImBulletWidthEnemy()
{
}

AImBulletWidthEnemy::~AImBulletWidthEnemy()
{
}

void AImBulletWidthEnemy::Initialize(const Transform& transform, const Vector3& velocity, float moveSpeed, uint32_t texture)
{
	sphere_ = std::make_unique<Sphere>();
	sphere_->Initialize();

	worldTransform_.Initialize();
	worldTransform_.translation_ = transform.translate;
	worldTransform_.scale_ = transform.scale;
	BulletScale_ = { 1.0f,1.0f,1.0f };
	MoveSpeed_ = moveSpeed;
	velocity_ = { 0.0f,-1.0f,0.0f };
	velocity_.x = std::clamp(velocity_.x, -1.0f, 1.0f);
	velocity_.y = std::clamp(velocity_.y, -1.0f, 1.0f);
	velocity_.z = std::clamp(velocity_.z, -1.0f, 1.0f);
	texindex_ = texture;
	velocity_ = Multiply(MoveSpeed_, velocity_);
	type_ = kAimBulletWidth;
	isAlive_ = true;
	atackCount_ = 5;
	coolTime_ = 60;
}

void AImBulletWidthEnemy::Update()
{
	currentCollTime_++;
	if (player_) {
		worldTransform_.translation_.y = player_->GetWorldTransform().translation_.y;
	}
	if (currentAtackCount_ <= atackCount_)
		if (currentCollTime_ >= coolTime_) {
			Atack();
			currentCollTime_ = 0;
			currentAtackCount_++;
		}
	if (currentAtackCount_ >= atackCount_) {
		isAlive_ = false;
	}
	worldTransform_.UpdateMatrix();
}

void AImBulletWidthEnemy::Draw(const ViewProjection& viewProjection)
{
	sphere_->Draw({ 1.0f,1.0f,1.0f,1.0f }, worldTransform_, texindex_, viewProjection);
}

void AImBulletWidthEnemy::isCollision(OBB pertner)
{
}

void AImBulletWidthEnemy::TextureInitialize()
{
}

void AImBulletWidthEnemy::Atack()
{
	PlayerAimBullet* newBullet = new PlayerAimBullet();
	Vector3 bulletvel;
	if (worldTransform_.translation_.x > targetWordTransform_.GetWorldPos().x) {
		bulletvel = { -1.0f,0.0f,0.0f };
	}
	else {
		bulletvel = { 1.0f,0.0f,0.0f };
	}
	newBullet->Initialize(bulletvel, { BulletScale_,worldTransform_.rotation_,worldTransform_.translation_ }, texindex_);
	gameScene_->AddEnemyBullet(newBullet);
}
