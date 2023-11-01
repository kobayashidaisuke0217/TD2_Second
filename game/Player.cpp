#include "Player.h"
#include "Input.h"
#include <algorithm>
#include <math.h>
#include "Mymath.h"
void Player::Initialize(){
	worldTransform_.Initialize();
	worldTransform_.translation_.x = 4.0f;
	worldTransform_.translation_.y = 4.0f;
	velocity_ = {0,0,0};
	acceleration_ = {0,0,0};
	gravity_ = {0,-0.01f,0};
	direction_ = 0.1f;
	model_.reset(Model::CreateModelFromObj("Resource/cube", "cube.obj"));
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0,0,0 });
	obb_.size = { worldTransform_.scale_.x / 2.0f,worldTransform_.scale_.y / 2.0f,worldTransform_.scale_.z / 2.0f };	GetOrientations(rotateMatrix, obb_.orientation);
	obb_.center = worldTransform_.translation_;
	GetOrientations(rotateMatrix, obb_.orientation);
}


void Player::Update() {
	//acceleration_=acceleration_ + gravity_;
	velocity_ = velocity_+ acceleration_ + gravity_;
	velocity_.x = direction_;
	velocity_.y = std::clamp(velocity_.y,-0.4f,0.4f);
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;
	worldTransform_.UpdateMatrix();
	obb_.center = worldTransform_.translation_;
}

void Player::OnCollision(OBB& partner) {
	if (std::abs(obb_.center.x - partner.center.x) < std::abs(obb_.center.y - partner.center.y)) {
		if ((obb_.center.x - partner.center.x)>0.0f){
			//上に載ってるときの処理
			acceleration_ = { 0 ,0,0 };
			velocity_ = { 0,0,0 };
			worldTransform_.translation_.y = partner.center.y + 1.0f;
		}
		else {
			//デバッグ用
			//worldTransform_.translation_.y += 10.0f;
		}
	}
	else {
		//横方向から当たったときの処理
		worldTransform_.translation_.x = partner.center.x + Normalise(obb_.center - partner.center).x * 1.0f;
		direction_ *= -1.0f;
	}
}

void Player::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_,viewProjection); }