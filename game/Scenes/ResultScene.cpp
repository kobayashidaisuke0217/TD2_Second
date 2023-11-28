#include "ResultScene.h"
#include "MapManager.h"

ResultScene::~ResultScene() {

}

void ResultScene::Finalize() {

}

void ResultScene::Initialize() {
	sceneNum = 2;
	blueMoon_ = BlueMoon::GetInstance();

	directXCommon_ = DirectXCommon::GetInstance();

	textureManager_ = Texturemanager::GetInstance();

	transitionSprite_.reset(new Sprite);
	transitionSprite_->Initialize({ 0.0f,0.0f,0.0f,0.0f }, { 1280.0f,720.0f,0.0f,0.0f });
	blackTextureHandle_ = textureManager_->Load("resource/startLine.png");

	backGroundSprite_.reset(new Sprite);
	backGroundSprite_->Initialize({ 0,0,0,0 }, { 1280,720,0,0 });
	backTextureHandle_ = textureManager_->Load("resource/back.png");

	viewProjection_.Initialize();

	GlovalVariables* globalVariables = GlovalVariables::GetInstance();
	const char* groupName = "ResultSetting";
	globalVariables->AddItem(groupName, "cameraRotate", viewProjection_.rotation_);
	globalVariables->AddItem(groupName, "cameraTranslate", viewProjection_.translation_);
#ifdef _DEBUG
	MapManager::GetInstance()->Initialize();

#endif // _DEBUG


	MapManager::GetInstance()->WaveRead(0);
}

void ResultScene::ApplyGlobalVariables()
{
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();
	const char* groupName = "ResultSetting";
	viewProjection_.rotation_ = globalVariables->GetVector3Value(groupName, "cameraRotate");
	viewProjection_.translation_ = globalVariables->GetVector3Value(groupName, "cameraTranslate");


}

void ResultScene::Update() {
	ApplyGlobalVariables();
	switch (phase_)
	{
	case ResultScene::FROMGAME:
		FromGame();
		break;
	case ResultScene::RESULT:
		Result();
		break;
	case ResultScene::TOTITLE:
		ToTitle();
		break;
	default:
		break;
	}
	if (isRunAnimation_) {
		TransitionAnimation();
	}
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();
}

void ResultScene::FromGame() {
	if (!isRunAnimation_) {
		isRunAnimation_ = true;
		frameCount_ = 0;
	}
	if (isRunAnimation_) {
		resetT_ = frameCount_ / float(transitionAnimationLength_);
		//resetT_ = std::powf(resetT_ * 2.0f - 1.0f, 2) * -1.0f + 1.0f;
		resetT_ = 1.0f - resetT_;

		
		if (frameCount_ >= transitionAnimationLength_) {
			isRunAnimation_ = false;
			phase_ = RESULT;
		}
	}
	frameCount_++;
}

void ResultScene::Result() {

}

void ResultScene::ToTitle() {
	if (!isRunAnimation_) {
		isRunAnimation_ = true;
		frameCount_ = 0;
	}
	if (isRunAnimation_) {
		resetT_ = frameCount_ / float(transitionAnimationLength_);
		

		if (frameCount_ >= transitionAnimationLength_) {
			isRunAnimation_ = false;
			//sceneNum = 1;
		}
	}
	frameCount_++;
}

void ResultScene::Draw()
{
	//2D描画準備
	blueMoon_->SpritePreDraw();
	DrawBackGround();
	//3D描画準備
	blueMoon_->ModelPreDraw();
	Draw3D();
	//2D描画準備
	blueMoon_->SpritePreDraw();
	Draw2D();
}

void ResultScene::Draw3D() {
	blueMoon_->ModelPreDraw();

	MapManager::GetInstance()->Draw(viewProjection_);
}

void ResultScene::Draw2D() {
	blueMoon_->SetBlendMode(blendCount_);
	Transform uv = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f} };

	if (isRunAnimation_) {
		Transform pos = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{transitionSpritePosition_.x,transitionSpritePosition_.y,transitionSpritePosition_.z} };
		transitionSprite_->Draw(pos, uv, { 0.0f,0.0f,0.0f,1.0f }, blackTextureHandle_);
	}
}

void ResultScene::DrawBackGround() {
	blueMoon_->SetBlendMode(blendCount_);
	Transform uv = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f} };
	Transform pos = { {1.0f,1.0f,0.0f},{0.0f,0.0f,0.0f},{0,0,0} };
	backGroundSprite_->Draw(pos, uv, { 1.0f,1.0f,1.0f,1.0f }, backTextureHandle_);
	
}

void ResultScene::TransitionAnimation() {

	transitionSpritePosition_.x = (1.0f - resetT_) * transitionStartPosition_.x + resetT_ * transitionEndPosition_.x;
	transitionSpritePosition_.y = (1.0f - resetT_) * transitionStartPosition_.y + resetT_ * transitionEndPosition_.y;
	transitionSpritePosition_.z = (1.0f - resetT_) * transitionStartPosition_.z + resetT_ * transitionEndPosition_.z;
}
