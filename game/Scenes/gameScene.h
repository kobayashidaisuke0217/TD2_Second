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
#include "game/Player.h"
#include <list>
#pragma region エネミー
#include"game/Enemy/IEnemy.h"
#include"game/Enemy/BoundEnemy.h"
#include"game/Enemy/ReflectEnemy.h"
#include"game/Enemy/BulletEnemy.h"
#include"game/Enemy/StageChangeEnemy.h"
#include"game/Enemy/AimBulletEnemy.h"
#include"game/Enemy/PlayerAimBullet.h"
#pragma endregion
#include "Input.h"
#include "FollowCamera.h"
class GameScene :public Iscene
{
public:
	~GameScene();
	void Initialize()override;
	void Update()override;

	void Draw()override;
	void Finalize()override;
	void AddEnemyBullet(PlayerAimBullet* enemyBullet);
	void ReStartWave();
	void ReStart();
	void InGame();
	void Title();

	void ReStartAnimation();
	void TransitionAnimation();
	void DrawBackGround();
private:

	BlueMoon* blueMoon_;
	DirectXCommon* directXCommon_;
	Sprite* sprite_;
	Texturemanager* textureManager_;

	int blendCount_;
	std::unique_ptr<FollowCamera> followCamera_;
	ViewProjection viewProjection_;
	void Draw2D();
	void Draw3D();
	void ApplyGlobalVariables();

	XINPUT_STATE joyState_;
	XINPUT_STATE preJoyState_;

	std::unique_ptr<Player> player_;
	//std::list<Enemy*> enemys_;
	std::list <IEnemy*>enemys_;
	float EnemymoveSpeed_;
	Vector3 enemyVelocity_;
	uint32_t enemyTex_;
	EnemyType type;
	Transform enemyTransform;
	bool enemyPop_;
	int32_t waveNum_;
	std::list<PlayerAimBullet*> bullets_;
	//設定群
	float fallingBorder_ = -10.0f;
	float upperBorder_ = 100.0f;
	float horizonBorder_=100.0f;

	//リスポーン用
	std::unique_ptr<Sprite> transitionSprite_;
	bool isInGame_;
	//遷移アニメーションの補間用
	float resetT_;
	//遷移アニメーションの長さ
	uint32_t transitionAnimationLength_ = 30;
	//遷移アニメーションの開始までの長さ
	uint32_t transitionAnimationDelay_ = 60;
	uint32_t frameCount_;
	bool isRunAnimation_;
	Vector4 transitionStartPosition_ = {0.0f,-720.0f,0.0f,0.0f};
	Vector4 transitionEndPosition_ = {0.0f,0.0f,0.0f,0.0f};
	Vector4 transitionSpritePosition_;
	uint32_t blackTextureHandle_;
	bool isTitle_;
	bool isStartGame_;
	std::unique_ptr<Plane> titleLine_;
	std::unique_ptr<Plane> titleChar_;
	WorldTransform worldTransformLine_;
	WorldTransform worldTransformStart_;


	Vector3 lineScale_ = {1.0f,1.0f,1.0f};
	Vector3 linePosition_ = {0,0,0};

	std::unique_ptr<Sprite> titleSprite_;

	Transform titleTransform_;
	uint32_t titleTextureHandle_;
	uint32_t backTextureHandle_;
	uint32_t startTextureHandle_;

	std::unique_ptr<Sprite> backGroundSprite_;

private:
	void EnemySpawn(const WorldTransform& worldTransform, EnemyType type);
};

