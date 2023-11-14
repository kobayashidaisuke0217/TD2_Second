#include "Player.h"
#include "Input.h"
#include <algorithm>
#include <math.h>
#include "Mymath.h"
#include "Globalvariables.h"
#include "GameController.h"
void Player::Initialize(){

	GlovalVariables* globalVariables = GlovalVariables::GetInstance();
	const char* groupName = "Player";
	//jumpAccerelation_ = globalVariables->GetVector3Value(groupName, "jump");
	//moveSpeed_ = globalVariables->GetFloatValue(groupName, "speed");
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "gravity", gravity_);
	globalVariables->AddItem(groupName, "jump", jumpAccerelation_);
	globalVariables->AddItem(groupName, "speed", moveSpeed_);
	globalVariables->AddItem(groupName, "jumpDampingX", jumpDampingX_);
	globalVariables->AddItem(groupName, "jumpReceptionLength",jumpReceptionLength_);


	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f,1.0f,1.0f};
	worldTransform_.translation_.x = 0.0f;
	worldTransform_.translation_.y = 2.0f;
	velocity_ = {0,0,0};
	acceleration_ = {0,0,0};
	//gravity_ = {0,-0.005f,0};
	direction_ = 1.0f;
	model_.reset(Model::CreateModelFromObj("Resource/cube", "cube.obj"));
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0,0,0 });
	obb_.size = { worldTransform_.scale_.x ,worldTransform_.scale_.y,worldTransform_.scale_.z };
	obb_.center = worldTransform_.translation_;
	GetOrientations(rotateMatrix, obb_.orientation);

	obbFloatTrigger_.size = { worldTransform_.scale_.x ,worldTransform_.scale_.y*2,worldTransform_.scale_.z };
	obbFloatTrigger_.center = worldTransform_.translation_;
	obbFloatTrigger_.center.y -= 2.5;
	GetOrientations(rotateMatrix, obbFloatTrigger_.orientation);
	jumpAble_ = true;
	isJumpReception_ = false;
}


void Player::Update() {
		ApplyGlobalVariables();
	prePosition_ = worldTransform_.translation_;
	if (GameController::GetInstance()->Jump() && jumpAble_) {
		velocity_.y = 0.0f;
		velocity_.y = jumpAccerelation_.y;
		jumpAble_ = false;
		isJumpReception_ = true;
		jumpReceptionRest_ = jumpReceptionLength_;
	}
	if ((!GameController::GetInstance()->ContinueJump()) && isJumpReception_) {
		velocity_.y *= 0.6f;
		isJumpReception_ = false;
		//velocity_.y = jumpAccerelation_.y;
		jumpReceptionRest_--;
		if (jumpReceptionRest_<0) {
			isJumpReception_ = false;
		}
	}
	else {
		jumpReceptionRest_--;
		if (jumpReceptionRest_ < 0) {
			isJumpReception_ = false;
		}
		//velocity_.y = 0.0f;
		//isJumpReception_ = false;
	}

	float kSpeed = 0.1f;
	acceleration_=  gravity_;
	velocity_ = velocity_+ acceleration_ ;
	//velocity_.x = direction_ * kSpeed;
	if (isCollisionFloor_) {
		velocity_.x *= jumpDampingX_;
		if (std::abs(velocity_.x) <= 0.01f) {
			//velocity_.x = 0;
		}
	}
	if (GameController::GetInstance()->Left()) {
		//velocity_.y = 0.0f;
		//acceleration_ = { 0 ,0.06f,0 };
		velocity_.x = -1.0f * moveSpeed_;
	}
	if (GameController::GetInstance()->Right()) {
		//velocity_.y = 0.0f;
		//acceleration_ = { 0 ,0.06f,0 };
		velocity_.x = 1.0f * moveSpeed_;
	}
	velocity_.y = std::clamp(velocity_.y,-0.8f,0.8f);
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;
	worldTransform_.UpdateMatrix();
	obb_.center = worldTransform_.translation_;
	obbFloatTrigger_.center = worldTransform_.translation_;
	obbFloatTrigger_.center.y -= 2.5;
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

bool Player::OnCollisionFloorVertical(OBB& partner) {
	if (isCollisionFloor_ || 1) {
		if (std::abs(obb_.center.x - partner.center.x) <= std::abs(obb_.center.y - partner.center.y)) {
			jumpAble_ = true;
			//acceleration_ = { 0 ,0,0 };
			velocity_ = { 0,0,0 };
			if ((obb_.center.y - partner.center.y) / (std::sqrtf(std::powf(obb_.center.y - partner.center.y, 2))) * (obb_.size.y + partner.size.y) < 0) {
				worldTransform_.UpdateMatrix();
			}
			worldTransform_.translation_.y = partner.center.y + (obb_.center.y - partner.center.y) / (std::sqrtf(std::powf(obb_.center.y - partner.center.y, 2))) * (obb_.size.y + partner.size.y);
			obb_.center = worldTransform_.translation_;
			isCollisionFloor_ = false;
			worldTransform_.UpdateMatrix();
			return true;
		}
		else {
			//横方向から当たったときの処理
			//worldTransform_.translation_.x = partner.center.x + (obb_.center.x - partner.center.x)/(std::sqrtf(std::powf(obb_.center.x - partner.center.x,2))) * (obb_.size.x + partner.size.x);
			//direction_ *= -1.0f;
			//obb_.center = worldTransform_.translation_;
		}
		
	}
	return false;

}

void Player::OnCollisionFloorHorizon(OBB& partner) {
	if (isCollisionFloor_ || 1) {
		if (std::abs(obb_.center.x - partner.center.x) <= std::abs(obb_.center.y - partner.center.y)) {
			//jumpAble_ = true;
			//acceleration_ = { 0 ,0,0 };
			//velocity_ = { 0,0,0 };
			//worldTransform_.translation_.y = partner.center.y + (obb_.center.y - partner.center.y) / (std::sqrtf(std::powf(obb_.center.y - partner.center.y, 2))) * (obb_.size.y + partner.size.y);
			//obb_.center = worldTransform_.translation_;
			//isCollisionFloor_ = false;
			//worldTransform_.UpdateMatrix();
		}
		else {
			//横方向から当たったときの処理
			worldTransform_.translation_.x = partner.center.x + (obb_.center.x - partner.center.x)/(std::sqrtf(std::powf(obb_.center.x - partner.center.x,2))) * (obb_.size.x + partner.size.x);
			//direction_ *= -1.0f;
			obb_.center = worldTransform_.translation_;
			isCollisionFloor_ = false;
			worldTransform_.UpdateMatrix();
		}

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

void Player::OnCollisionEnemy() {

}

void Player::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_,viewProjection); }

void Player::ApplyGlobalVariables()
{
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();
	const char* groupName = "Player";
	gravity_ = globalVariables->GetVector3Value(groupName, "gravity");
	jumpAccerelation_ = globalVariables->GetVector3Value(groupName, "jump");
	moveSpeed_ = globalVariables->GetFloatValue(groupName, "speed");
	jumpDampingX_ = globalVariables->GetFloatValue(groupName, "jumpDampingX");
	jumpReceptionLength_ = globalVariables->GetIntValue(groupName, "jumpReceptionLength");
}
