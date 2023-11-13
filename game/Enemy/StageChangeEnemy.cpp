#include "StageChangeEnemy.h"

StageChangeEnemy::StageChangeEnemy()
{
}

StageChangeEnemy::~StageChangeEnemy()
{
}

void StageChangeEnemy::Initialize(const Transform& transform, const Vector3& velocity, float moveSpeed, uint32_t texture, const WorldTransform& targettransform)
{
	sphere_ = std::make_unique<Sphere>();
	sphere_->Initialize();

	worldTransform_.Initialize();
	worldTransform_.translation_ = transform.translate;
	worldTransform_.scale_ = transform.scale;

	MoveSpeed_ = moveSpeed;
	targetWordTransform_ = targettransform;
	velocity_ = velocity;
	texindex_ = texture;
	velocity_ = Multiply(MoveSpeed_, velocity_);
	type_ = kBullet;
	isAlive_ = true;
}

void StageChangeEnemy::Update()
{
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
	ImGui::Begin("enemy");
	ImGui::DragFloat3("velocity", &velocity_.x);
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x);

	ImGui::End();
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0.0f,0.0f,0.0f });
	obb_.size = { worldTransform_.scale_.x / 2.0f ,worldTransform_.scale_.y / 2.0f ,worldTransform_.scale_.z / 2.0f };
	obb_.center = worldTransform_.translation_;
	GetOrientations(rotateMatrix, obb_.orientation);
}

void StageChangeEnemy::Draw(const ViewProjection& viewProjection)
{
	sphere_->Draw({ 1.0f,1.0f,1.0f,1.0f }, worldTransform_, texindex_, viewProjection);
}

void StageChangeEnemy::isCollision(OBB pertner)
{
	isAlive_ = false;
}