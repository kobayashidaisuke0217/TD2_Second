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

	const char* groupName2 = "PlayerModel";
	globalVariables->AddItem(groupName2, "antena", antenaOffset_);
	globalVariables->AddItem(groupName2, "code", codeOffset_);
	globalVariables->AddItem(groupName2, "head", headOffset_);
	globalVariables->AddItem(groupName2, "body", bodyOffset_);
	globalVariables->AddItem(groupName2, "back", backOffset_);
	globalVariables->AddItem(groupName2, "left", leftOffset_);
	globalVariables->AddItem(groupName2, "right", rightOffset_);
	globalVariables->AddItem(groupName2, "legRotate", legRotate_);
	globalVariables->AddItem(groupName2, "bodyIdle", floatBodyIdle_);
	globalVariables->AddItem(groupName2, "bodyMove", floatBodyMove_);


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

	//model
	antena_.reset(Model::CreateModelFromObj("Resource/player/antena", "PlayerBall.obj"));
	code_.reset(Model::CreateModelFromObj("Resource/player/antena", "PlayerAntena.obj"));
	body_.reset(Model::CreateModelFromObj("Resource/player/body", "PlayerBody.obj"));
	back_.reset(Model::CreateModelFromObj("Resource/player/back", "PlayerBack.obj"));
	head_.reset(Model::CreateModelFromObj("Resource/player/head", "PlayerHead.obj"));
	leg_.reset(Model::CreateModelFromObj("Resource/player/leg", "PlayerLeg.obj"));

	worldTransformAntena_.Initialize();
	worldTransformbody_.Initialize();
	worldTransformbody_.parent_ = &worldTransform_;
	worldTransformback_.Initialize();
	worldTransformback_.parent_ = &worldTransformbody_;
	worldTransformCode_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformHead_.parent_ = &worldTransformbody_;
	worldTransformLeftLeg_.Initialize();
	worldTransformLeftLeg_.parent_ = &worldTransform_;
	worldTransformRightLeg_.Initialize();
	worldTransformRightLeg_.parent_ = &worldTransform_;
	Vector3 antenaPos = vectorTransform(antenaOffset_, worldTransformHead_.matWorld_);
	worldTransformAntena_.translation_ = Lerp(1.0f, worldTransformAntena_.translation_, antenaPos);
	worldTransformAntena_.UpdateMatrix();
	worldTransformCode_.translation_ = Lerp(0.5f, worldTransformAntena_.translation_, worldTransformHead_.GetWorldPos());
	Matrix4x4 rotateCode = DirectionToDirection({ 0,0,1.0f }, Normalise(worldTransformHead_.GetWorldPos() - worldTransformAntena_.translation_));
	worldTransformCode_.matWorld_ = Multiply(Multiply(MakeScaleMatrix(worldTransformCode_.scale_), rotateCode), MakeTranslateMatrix(worldTransformCode_.translation_));
	worldTransformCode_.TransferMatrix();
	isMove_ = false;
	theta_ = 0;
}

void Player::Reset() {
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.translation_.x = 0.0f;
	worldTransform_.translation_.y = 2.0f;
	velocity_ = { 0,0,0 };
	acceleration_ = { 0,0,0 };
	//gravity_ = {0,-0.005f,0};
	direction_ = 1.0f;
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0,0,0 });
	obb_.size = { worldTransform_.scale_.x ,worldTransform_.scale_.y,worldTransform_.scale_.z };
	obb_.center = worldTransform_.translation_;
	GetOrientations(rotateMatrix, obb_.orientation);

	obbFloatTrigger_.size = { worldTransform_.scale_.x ,worldTransform_.scale_.y * 2,worldTransform_.scale_.z };
	obbFloatTrigger_.center = worldTransform_.translation_;
	obbFloatTrigger_.center.y -= 2.5;
	GetOrientations(rotateMatrix, obbFloatTrigger_.orientation);
	jumpAble_ = true;
	isJumpReception_ = false;

	Vector3 antenaPos = vectorTransform(antenaOffset_, worldTransformHead_.matWorld_);
	worldTransformAntena_.translation_ = Lerp(1.0f, worldTransformAntena_.translation_, antenaPos);
	worldTransformAntena_.UpdateMatrix();
	worldTransformCode_.translation_ = Lerp(0.5f, worldTransformAntena_.translation_, worldTransformHead_.GetWorldPos());
	Matrix4x4 rotateCode = DirectionToDirection({ 0,0,1.0f }, Normalise(worldTransformHead_.GetWorldPos() - worldTransformAntena_.translation_));
	worldTransformCode_.matWorld_ = Multiply(Multiply(MakeScaleMatrix(worldTransformCode_.scale_), rotateCode), MakeTranslateMatrix(worldTransformCode_.translation_));
	worldTransformCode_.TransferMatrix();
	isMove_ = false;
	theta_ = 0;
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
	isMove_ = false;
	if (GameController::GetInstance()->Left()) {
		//velocity_.y = 0.0f;
		//acceleration_ = { 0 ,0.06f,0 };
		velocity_.x = -1.0f * moveSpeed_;
		direction_ = -1.0f;
		isMove_ = true;
	}
	if (GameController::GetInstance()->Right()) {
		//velocity_.y = 0.0f;
		//acceleration_ = { 0 ,0.06f,0 };
		velocity_.x = 1.0f * moveSpeed_;
		direction_ = 1.0f;
		isMove_ = true;
	}
	if (isMove_ && !isJumpReception_ && !isCollisionFloor_) {
		theta_ += legRotate_;
		floatAnimetion_ += floatBodyMove_;
	}
	else if(!isJumpReception_ && !isCollisionFloor_){
		theta_ = 0;
		floatAnimetion_ += floatBodyIdle_;
	}
	else {
		//theta_ = 0;
		floatAnimetion_ = 0;
	}
	velocity_.y = std::clamp(velocity_.y,-0.8f,0.8f);
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;
	worldTransform_.rotation_.y = 1.57f * direction_;
	worldTransform_.UpdateMatrix();
	obb_.center = worldTransform_.translation_;
	obbFloatTrigger_.center = worldTransform_.translation_;
	obbFloatTrigger_.center.y -= 2.5;
	isCollision_ = true;
	isCollisionFloor_ = true;
	isCollisionWall_ = true;

	worldTransformbody_.translation_ = bodyOffset_;
	worldTransformbody_.translation_.y += std::sin(floatAnimetion_)*0.5f;
	worldTransformbody_.UpdateMatrix();
	worldTransformback_.translation_ = backOffset_;
	worldTransformback_.UpdateMatrix();
	worldTransformHead_.translation_ = headOffset_;
	worldTransformHead_.UpdateMatrix();
	worldTransformLeftLeg_.rotation_.x = std::sin(theta_);
	worldTransformLeftLeg_.translation_ = leftOffset_;
	worldTransformLeftLeg_.UpdateMatrix();
	worldTransformRightLeg_.rotation_.x = -std::sin(theta_);
	worldTransformRightLeg_.translation_ = rightOffset_;
	worldTransformRightLeg_.UpdateMatrix();

	Vector3 antenaPos = vectorTransform(antenaOffset_, worldTransformHead_.matWorld_);
	worldTransformAntena_.translation_ =  Lerp(0.1f, worldTransformAntena_.translation_,antenaPos);
	worldTransformAntena_.UpdateMatrix();
	worldTransformCode_.translation_ = Lerp(0.5f,worldTransformAntena_.translation_,worldTransformHead_.GetWorldPos());
	Matrix4x4 rotateCode = DirectionToDirection({0,0,1.0f},Normalise( worldTransformHead_.GetWorldPos() - worldTransformAntena_.translation_));
	worldTransformCode_.matWorld_ = Multiply(Multiply(MakeScaleMatrix(worldTransformCode_.scale_) , rotateCode),MakeTranslateMatrix(worldTransformCode_.translation_));
	worldTransformCode_.TransferMatrix();
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

void Player::Draw(const ViewProjection& viewProjection) {
	//model_->Draw(worldTransform_,viewProjection);
	body_->Draw(worldTransformbody_,viewProjection);
	back_->Draw(worldTransformback_, viewProjection);
	head_->Draw(worldTransformHead_, viewProjection);
	leg_->Draw(worldTransformLeftLeg_, viewProjection);
	leg_->Draw(worldTransformRightLeg_, viewProjection);

	antena_->Draw(worldTransformAntena_,viewProjection);
	code_->Draw(worldTransformCode_, viewProjection);
}

void Player::ApplyGlobalVariables()
{
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();
	const char* groupName = "Player";
	gravity_ = globalVariables->GetVector3Value(groupName, "gravity");
	jumpAccerelation_ = globalVariables->GetVector3Value(groupName, "jump");
	moveSpeed_ = globalVariables->GetFloatValue(groupName, "speed");
	jumpDampingX_ = globalVariables->GetFloatValue(groupName, "jumpDampingX");
	jumpReceptionLength_ = globalVariables->GetIntValue(groupName, "jumpReceptionLength");

	const char* groupName2 = "PlayerModel";
	antenaOffset_ = globalVariables->GetVector3Value(groupName2, "antena");
	codeOffset_ = globalVariables->GetVector3Value(groupName2, "code");
	headOffset_ = globalVariables->GetVector3Value(groupName2, "head");
	bodyOffset_ = globalVariables->GetVector3Value(groupName2, "body");
	backOffset_ = globalVariables->GetVector3Value(groupName2, "back");
	leftOffset_ = globalVariables->GetVector3Value(groupName2, "left");
	rightOffset_ = globalVariables->GetVector3Value(groupName2, "right");

	legRotate_ = globalVariables->GetFloatValue(groupName2, "legRotate");
	floatBodyIdle_ = globalVariables->GetFloatValue(groupName2, "bodyIdle");
	floatBodyMove_ = globalVariables->GetFloatValue(groupName2, "bodyMove");

}
