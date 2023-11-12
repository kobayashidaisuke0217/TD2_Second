#include "FollowCamera.h"
void FollowCamera::Initialize()
{
	viewProjection_.Initialize();
}

void FollowCamera::Update()
{
	
	if (target_)
	{
		//Vector3 offset = { 0.0f,2.0f,-20.0f };

		Vector3 direction = forcusPoint_ - target_->GetWorldPos();

		Matrix4x4 rotateMatrix = DirectionToDirection({0,0,1.0f},Normalise(direction));

		//offset = TransformNormal(offset, rotateMatrix);

		//interTargert_ = Lerp(interTargert_, target_->GetWorldPosition(), cameraDelay_);

		viewProjection_.translation_ =  offset_;
		viewProjection_.translation_.x += target_->GetWorldPos().x;
	
		//手動アップデート
		viewProjection_.matView = Inverse(Multiply(Multiply(MakeScaleMatrix({1.0f,1.0f,1.0f}) , rotateMatrix),MakeTranslateMatrix(viewProjection_.translation_)) );
		viewProjection_.UpdateProjectionMatrix();
	}
}