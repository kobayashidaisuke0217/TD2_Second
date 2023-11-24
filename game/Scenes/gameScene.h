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

	std::unique_ptr<Sprite> transitionSprite_;
	bool isInGame_;
private:
	void EnemySpawn(const WorldTransform& worldTransform, EnemyType type);
};

