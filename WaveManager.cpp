#include "WaveManager.h"
#include "MapManager.h"
#include"game/Enemy/IEnemy.h"
#include"game/Enemy/BoundEnemy.h"
#include"game/Enemy/ReflectEnemy.h"
#include"game/Enemy/BulletEnemy.h"
#include"game/Enemy/StageChangeEnemy.h"
#include"game/Enemy/AimBulletEnemy.h"
#include"game/Enemy/PlayerAimBullet.h"
#include "game/Scenes/gameScene.h"

#include <fstream>
#include <sstream>
WaveManager* WaveManager::GetInstance() {
	static WaveManager instance;
	return &instance;
}

void WaveManager::LoadAllFiles() {
	maxWaveNum_ = 0;
	waves_.clear();
	waves_.shrink_to_fit();
	//LoadFile("Resource/Wave/waveData.wave");
	LoadFile("Resource/Wave/protoWave.wave");
}

void WaveManager::LoadFile(const char filename[]) {
	std::string line;
	std::ifstream file(filename);
	//assert(file.is_open());
	if (!file.is_open()) {
		return;
	}
	EnemyData* newEnemyData = nullptr;
	Wave* newWave = nullptr;
	while (std::getline(file, line)) {
		std::string identifilter;
		std::istringstream s(line);
		s >> identifilter;
		if (identifilter == "wave") {
			newWave = &waves_.emplace_back();
			maxWaveNum_++;
		}
		else if (identifilter == "enemy") {
			if (newWave) {
				newEnemyData = &newWave->enemyDatas.emplace_back();
			}
		}
		else if (identifilter == "position") {
			Vector3 position;
			s >> position.x >> position.y >> position.z;
			if (newEnemyData) {
				//マップからトランスフォーム値に変換
				newEnemyData->translate.x = float(position.x) * 2.0f * 2.0f;
				newEnemyData->translate.y = float(position.y) * 2.0f * 2.0f;
				newEnemyData->translate.z = float(position.z) * 2.0f * 2.0f;
			}
		}
		else if (identifilter == "velocity") {
			if (newEnemyData) {
				s >> newEnemyData->velocity.x >> newEnemyData->velocity.y >> newEnemyData->velocity.z;
			}
		}
		else if (identifilter == "speed") {
			if (newEnemyData) {
				s >> newEnemyData->speed;
			}
		}
		else if (identifilter == "type") {
			int32_t type;
			s >> type;
			if (newEnemyData) {
				newEnemyData->type = EnemyType(type);
			}
		}
		else if (identifilter == "frame") {
			if (newEnemyData) {
				s >> newEnemyData->frame;
			}
		}
		else if (identifilter == "length") {
			if (newWave) {
				s >> newWave->length;
			}
		}
	}

}

void WaveManager::Initialize() {
	lowNum0.reset(new Sprite);
	lowNum0->Initialize({ -500.0f,-500.0f,0,0 }, { 500.0f,500.0f,0,0 });
	lowNum1.reset(new Sprite);
	lowNum1->Initialize({ -500.0f,-500.0f,0,0 }, { 500.0f,500.0f,0,0 });
	highNum0.reset(new Sprite);
	highNum0->Initialize({ -500.0f,-500.0f,0,0 }, { 500.0f,500.0f,0,0 });
	highNum1.reset(new Sprite);
	highNum1->Initialize({ -500.0f,-500.0f,0,0 }, { 500.0f,500.0f,0,0 });
	numberTextureHandle_[0] = Texturemanager::GetInstance()->Load("Resource/nums/0.png");
	numberTextureHandle_[1] = Texturemanager::GetInstance()->Load("Resource/nums/1.png");
	numberTextureHandle_[2] = Texturemanager::GetInstance()->Load("Resource/nums/2.png");
	numberTextureHandle_[3] = Texturemanager::GetInstance()->Load("Resource/nums/3.png");
	numberTextureHandle_[4] = Texturemanager::GetInstance()->Load("Resource/nums/4.png");
	numberTextureHandle_[5] = Texturemanager::GetInstance()->Load("Resource/nums/5.png");
	numberTextureHandle_[6] = Texturemanager::GetInstance()->Load("Resource/nums/6.png");
	numberTextureHandle_[7] = Texturemanager::GetInstance()->Load("Resource/nums/7.png");
	numberTextureHandle_[8] = Texturemanager::GetInstance()->Load("Resource/nums/8.png");
	numberTextureHandle_[9] = Texturemanager::GetInstance()->Load("Resource/nums/9.png");

	drawerWaveNum_ = 0;
	centerPos0_ = right_;
	topPos0_ = right_;
	topPos0_.y -= 500;
	downPos0_ = right_;
	downPos0_.y += 500;
	centerPos1_ = left_;
	topPos1_ = left_;
	topPos1_.y -= 500;
	downPos1_ = left_;
	downPos1_.y += 500;
	s0 = centerPos0_;
	s1 = topPos0_;
	s2 = centerPos1_;
	s3 = topPos1_;
	num0 = 1;
	num1 = 2;
	num2 = 0;
	num3 = 0;

	isChangeNum_ = false;
	isChange10 = false;
}

void WaveManager::Update() {
	for (EnemyData& enemy : waves_[size_t(waveNum_)].enemyDatas) {
		if (currentFrame_ == enemy.frame) {
			//Enemyの生成処理	
			IEnemy* newEnemy;

			Transform transform;
			transform.scale = { 1.0f,1.0f,1.0f };
			transform.rotate = { 0,0,0 };
			transform.translate = enemy.translate;

			switch (enemy.type)
			{
			case kBullet://0
				newEnemy = new BulletEnemy();
				newEnemy->SetStartCount(gameScene_->BulletStartCount);
				transform.scale = { 1.5, 2, 2 };
				newEnemy->Initialize(transform, enemy.velocity, enemy.speed, 2, gameScene_->bulletEnemyModel_.get());

				enemyList_->push_back(newEnemy);
				break;
			case kReflect://1
				newEnemy = new ReflectEnemy();
				transform.scale = { 2, 2, 2 };
				newEnemy->Initialize(transform, enemy.velocity, enemy.speed, 2, gameScene_->ballEnemyModel_.get());
				enemyList_->push_back(newEnemy);
				break;
			case kBound://2
				newEnemy = new BoundEnemy();
				transform.scale = { 3, 3, 3 };
				newEnemy->Initialize(transform, enemy.velocity, enemy.speed, 2, gameScene_->ballEnemyModel_.get());
				enemyList_->push_back(newEnemy); 
				break;
			case kTire://3
				newEnemy = new TireEnemy();
				//{ 0.3f, -1.0f, 0.0f }
				transform.scale = { 1.8f, 1.8f, 1.8f };
				newEnemy->Initialize(transform, enemy.velocity, enemy.speed, 2, gameScene_->wheelEnemyModel_.get());

				enemyList_->push_back(newEnemy);
				break;
			case kSpear://4
				break;
			case kRaser://5
				newEnemy = new BeamEnemy();
				//{ 0.3f, -1.0f, 0.0f }
				newEnemy->Initialize(transform, enemy.velocity, enemy.speed, 2, gameScene_->reverceEnemyModel_.get());
				newEnemy->SetStartCount(gameScene_->BulletStartCount);
				enemyList_->push_back(newEnemy);
				break;
			case kAimBulletWidth://6
				newEnemy = new AImBulletWidthEnemy();
				//{ 0.3f, -1.0f, 0.0f }
				transform.scale = { 2, 2, 2 };
				newEnemy->Initialize(transform, enemy.velocity, enemy.speed, 2, gameScene_->bulletEnemyModel_.get());
				newEnemy->SetPlayer(player_);
				newEnemy->SetGameScene(gameScene_);
				enemyList_->push_back(newEnemy);
				break;
			case kAimBulletHeight://7
				newEnemy = new AimBulletEnemy();
				//{ 0.3f, -1.0f, 0.0f }
				//enemy->Initialize(enemyTransform, enemyVelocity_, EnemymoveSpeed_, enemyTex_);
				transform.scale = { 2, 2, 2 };
				newEnemy->SetPlayer(player_);
				newEnemy->SetGameScene(gameScene_);
				newEnemy->Initialize(transform, enemy.velocity, enemy.speed, 2, gameScene_->bulletEnemyModel_.get());
				enemyList_->push_back(newEnemy);
				break;
			case kAimBound://8
				newEnemy = new PlayerAimBallEnemy();
				//{ 0.3f, -1.0f, 0.0f }
				transform.scale = { 3, 3, 3 };
				newEnemy->Initialize(transform, enemy.velocity, enemy.speed, 2, gameScene_->ballEnemyModel_.get());
				newEnemy->SetPlayer(player_);
				newEnemy->SetGameScene(gameScene_);
				enemyList_->push_back(newEnemy);
				break;
			case kStageUp://9
				newEnemy = new StageChangeEnemy();
				//{ 0.3f, -1.0f, 0.0f }
				//enemy->Initialize(enemyTransform, enemyVelocity_, EnemymoveSpeed_, enemyTex_);
				newEnemy->SetType(kStageUp);
				newEnemy->Initialize(transform, enemy.velocity, enemy.speed, 2, gameScene_->reverceEnemyModel_.get());
				enemyList_->push_back(newEnemy);
				break;
			case kStageDown://10
				newEnemy = new StageChangeEnemy();
				//enemy->Initialize(enemyTransform, enemyVelocity_, EnemymoveSpeed_, enemyTex_);
				newEnemy->SetType(kStageDown);
				newEnemy->Initialize(transform, enemy.velocity, enemy.speed, 2, gameScene_->reverceEnemyModel_.get());
				enemyList_->push_back(newEnemy);
				break;
			case kHoming://11
				break;
			default://else
				newEnemy = new ReflectEnemy();
				//enemy->Initialize(enemyTransform, enemyVelocity_, EnemymoveSpeed_, enemyTex_);

				newEnemy->Initialize(transform, enemy.velocity, enemy.speed, 2, gameScene_->ballEnemyModel_.get());
				enemyList_->push_back(newEnemy);
				break;
			}

		}
	}
	currentFrame_++;
	if (currentFrame_ >= waves_[size_t(waveNum_)].length + waveInterval_) {
		if (waves_.size() - 1 > waveNum_) {
			waveNum_++;
		}
		currentFrame_ = 0;
	}
	if (waveNum_ > drawerWaveNum_ && !isChangeNum_) {
		num0 = drawerWaveNum_ % 10 + 1;
		num1 = num0 + 1;
		if (num1 >= 10) {
			num1 = 0;
			num3 += 1;
			isChange10 = true;
		}
		isChangeNum_ = true;
		changeAnimationCount_ = 0;
		drawerWaveNum_++;
	}
	if (isChangeNum_) {
		ChangeNumAnimation();
	}
}

void WaveManager::Draw() {
	Transform uv = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f} };
	Transform transform = { { 0.1f,0.1f,0.1f },{0,0,0},{0,0,0} };
	transform.translate = s0;
	lowNum0->Draw(transform, uv, { 1.0f,1.0f,1.0f,1.0f }, numberTextureHandle_[num0]);
	transform.translate = s1;
	highNum0->Draw(transform, uv, { 1.0f,1.0f,1.0f,1.0f }, numberTextureHandle_[num1]);
	transform.translate = s2;
	lowNum1->Draw(transform, uv, { 1.0f,1.0f,1.0f,1.0f }, numberTextureHandle_[num2]);
	transform.translate = s3;
	highNum1->Draw(transform, uv, { 1.0f,1.0f,1.0f,1.0f }, numberTextureHandle_[num3]);

}

void WaveManager::ChangeNumAnimation() {
	float t = float(changeAnimationCount_) / float(changeAnimationLength_);
	s0 = Lerp(t, centerPos0_, downPos0_);
	s1 = Lerp(t, topPos0_, centerPos0_);
	if (isChange10) {
		s2 = Lerp(t, centerPos1_, downPos1_);
		s3 = Lerp(t, topPos1_, centerPos1_);
	}
	if (changeAnimationCount_ == changeAnimationLength_) {
		s0 = centerPos0_;
		s1 = topPos0_;
		num0 = num1;
		if (isChange10) {
			s2 = centerPos1_;
			s3 = topPos1_;
			num2 = num3;
		}
		isChangeNum_ = false;
		isChange10 = false;
	}
	changeAnimationCount_++;
}
