#include "Player.h"
#include "Input.h"
#include <algorithm>
#include <math.h>
#include "Mymath.h"
void Player::Initialize(){
	worldTransform_.Initialize();
	worldTransform_.translation_.x = 2.0f;
	worldTransform_.translation_.y = 2.0f;
	velocity_ = {0,0,0};
	acceleration_ = {0,0,0};
	gravity_ = {0,-0.005f,0};
	direction_ = 1.0f;
	model_.reset(Model::CreateModelFromObj("Resource/cube", "cube.obj"));
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0,0,0 });
	obb_.size = { worldTransform_.scale_.x / 2.0f,worldTransform_.scale_.y / 2.0f,worldTransform_.scale_.z / 2.0f };
	obb_.center = worldTransform_.translation_;
	GetOrientations(rotateMatrix, obb_.orientation);

	obbFloatTrigger_.size = { worldTransform_.scale_.x / 2.0f,worldTransform_.scale_.y*2,worldTransform_.scale_.z / 2.0f };
	obbFloatTrigger_.center = worldTransform_.translation_;
	obbFloatTrigger_.center.y -= 2.5;
	GetOrientations(rotateMatrix, obbFloatTrigger_.orientation);
}


void Player::Update() {
	prePosition_ = worldTransform_.translation_;
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		velocity_.y = 0.0f;
		acceleration_ = { 0 ,0.07f,0 };
	}
	float kSpeed = 0.1f;
	acceleration_=acceleration_ + gravity_;
	velocity_ = velocity_+ acceleration_ + gravity_;
	//velocity_.x = direction_ * kSpeed;
	if (Input::GetInstance()->PressKey(DIK_LEFT)) {
		//velocity_.y = 0.0f;
		//acceleration_ = { 0 ,0.06f,0 };
		velocity_.x = -1.0f * kSpeed;
	}
	if (Input::GetInstance()->PressKey(DIK_RIGHT)) {
		//velocity_.y = 0.0f;
		//acceleration_ = { 0 ,0.06f,0 };
		velocity_.x = 1.0f * kSpeed;
	}
	velocity_.y = std::clamp(velocity_.y,-0.4f,0.4f);
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;
	worldTransform_.UpdateMatrix();
	obb_.center = worldTransform_.translation_;
	obbFloatTrigger_.center = worldTransform_.translation_;
	obbFloatTrigger_.center.y -= 1.5;
	isCollision_ = true;
	isCollisionFloor_ = true;
	isCollisionWall_ = true;
}

void Player::OnCollision(OBB& partner) {
	if (isCollision_) {
		if (std::abs(obb_.center.x - partner.center.x) < std::abs(obb_.center.y - partner.center.y)) {
			if ((obb_.center.y - prePosition_.y) < 0.0f) {
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
		isCollision_ = false;
	}
	
}

void Player::OnCollisionFloor(OBB& partner) {
	if (isCollisionFloor_ || 1) {
		if (std::abs(obb_.center.x - partner.center.x) < std::abs(obb_.center.y - partner.center.y)) {
			if ((obb_.center.y - prePosition_.y) < 0.0f) {
				//上に載ってるときの処理
				acceleration_ = { 0 ,0,0 };
				velocity_ = { 0,0,0 };
				worldTransform_.translation_.y = partner.center.y + 1.0f;
			}
			else {
				//デバッグ用
				//worldTransform_.translation_.y += 10.0f;
				acceleration_ = { 0 ,0,0 };
				velocity_ = { 0,0,0 };
				worldTransform_.translation_.y = partner.center.y - 1.0f;
			}
		}
		else {
			//横方向から当たったときの処理
			worldTransform_.translation_.x = partner.center.x + (obb_.center.x - partner.center.x)/(std::sqrtf(std::powf(obb_.center.x - partner.center.x,2))) * (obb_.size.x + partner.size.x);
			//direction_ *= -1.0f;
		}
		isCollisionFloor_ = false;
		worldTransform_.UpdateMatrix();
	}

}

void Player::OnCollisionWall(OBB& partner) {
	if (isCollisionWall_) {
	
		{
			//横方向から当たったときの処理
			worldTransform_.translation_.x = partner.center.x + (obb_.center.x - partner.center.x) / (std::sqrtf(std::powf(obb_.center.x - partner.center.x, 2))) * (obb_.size.x + partner.size.x);
			//direction_ *= -1.0f;
			if (isCollisionFloor_) {
				//velocity_.y = 0.0f;
				//acceleration_ = { 0 ,0.06f,0 };
			}
		}
		isCollisionWall_ = false;
		worldTransform_.UpdateMatrix();
	}

}

void Player::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_,viewProjection); }