#pragma once
#include "BlueMoon.h"
#include "MyMath.h"
#include "triangle.h"
#include "Iscene.h"
#include "Sprite.h"
#include "Sphere.h"
#include "model.h"
#include "worldTransform.h"
#include "ViewProjection.h"
#include "Texturemanager.h"
#include "GlobalVariables.h"
#include "Particle.h"
#include "Plane.h"
//#include "game/Player.h"
#include <list>
class ResultScene :public Iscene
{
public:
	enum Phase
	{
		FROMGAME,
		RESULT,
		TOTITLE,
	};
	~ResultScene();
	void Initialize()override;
	void Update()override;
	void Draw() override;
	void Finalize()override;
	void TransitionAnimation();

	void FromGame();
	void Result();
	void ToTitle();

	void Draw2D();
	void Draw3D();
	void DrawBackGround();
	void ApplyGlobalVariables();
private:

	BlueMoon* blueMoon_;
	DirectXCommon* directXCommon_;
	Texturemanager* textureManager_;

	int blendCount_;

	Phase phase_;

	ViewProjection viewProjection_;


	std::unique_ptr<Sprite> transitionSprite_;
	bool isInGame_;
	//遷移アニメーションの補間用
	float resetT_;
	//遷移アニメーションの長さ
	uint32_t transitionAnimationLength_ = 15;
	//遷移アニメーションの開始までの長さ
	uint32_t transitionAnimationDelay_ = 0;
	uint32_t frameCount_;
	bool isRunAnimation_;
	Vector4 transitionStartPosition_ = { 0.0f,-720.0f,0.0f,0.0f };
	Vector4 transitionEndPosition_ = { 0.0f,0.0f,0.0f,0.0f };
	Vector4 transitionSpritePosition_;
	uint32_t blackTextureHandle_;

	uint32_t backTextureHandle_;
	std::unique_ptr<Sprite> backGroundSprite_;

	std::unique_ptr<Plane> plane_;

	//到達wave表示
	WorldTransform worldTransformReach_;
	WorldTransform worldTransformReachChar_;
	WorldTransform worldTransformReachWave_;
	WorldTransform worldTransformReach10_;
	WorldTransform worldTransformReach1_;

	//最高到達wave表示
	WorldTransform worldTransformBest_;
	WorldTransform worldTransformBestChar_;
	WorldTransform worldTransformBestWave_;
	WorldTransform worldTransformBest10_;
	WorldTransform worldTransformBest1_;

	uint32_t numberTextureHandle_[10];
	uint32_t reachTextureHandle_;
	uint32_t bestTextureHandle_;
	uint32_t waveTextureHandle_;

	int drawerWaveNum_;
	size_t bestWaveNum_;
	uint32_t numChangeLength_ = 5;

	std::unique_ptr<Sprite> returnTitleSprite_;
	Transform titleTransform_;
	uint32_t returnTextureHandle_;
	float alpha_;
	float theta_;
};
