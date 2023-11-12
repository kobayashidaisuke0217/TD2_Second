#include "FollowCamera.h"
#include "Globalvariables.h"
void FollowCamera::Initialize()
{
	viewProjection_.Initialize();
	GlovalVariables* grovalVariables = GlovalVariables::GetInstance();
	const char* groupName = "FollowCamera";
	grovalVariables->CreateGroup(groupName);
	grovalVariables->AddItem(groupName, "offset", offset_);
	grovalVariables->AddItem(groupName, "forcusPoint", forcusPoint_);
}

void FollowCamera::Update()
{
	ApplyGlobalVariables();
	if (target_)
	{
		//Vector3 offset = { 0.0f,2.0f,-20.0f };

		Vector3 playerPosition = target_->GetWorldPos();
		forcusPoint_.y = playerPosition.y;
		Vector3 direction = forcusPoint_ - playerPosition;

		Matrix4x4 rotateMatrix = DirectionToDirection({0,0,1.0f},Normalise(direction));

		//offset = TransformNormal(offset, rotateMatrix);

		//interTargert_ = Lerp(interTargert_, target_->GetWorldPosition(), cameraDelay_);

		viewProjection_.translation_ = target_->GetWorldPos() + TransformNormal(offset_, rotateMatrix);;
		//viewProjection_.translation_.x += target_->GetWorldPos().x;
	
		//手動アップデート
		viewProjection_.matView = Inverse(Multiply(Multiply(MakeScaleMatrix({1.0f,1.0f,1.0f}) , rotateMatrix),MakeTranslateMatrix(viewProjection_.translation_)) );
		viewProjection_.UpdateProjectionMatrix();
	}
}

void FollowCamera::ApplyGlobalVariables()
{
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();
	const char* groupName = "FollowCamera";
	//cameraDelay_ = globalVariables->GetFloatValue(groupName, "CameraDelay");
	offset_ = globalVariables->GetVector3Value(groupName, "offset");
	forcusPoint_ = globalVariables->GetVector3Value(groupName, "forcusPoint");
}
