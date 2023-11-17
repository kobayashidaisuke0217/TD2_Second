#include "WaveManager.h"
#include "MapManager.h"
#include "game/Enemy/IEnemy.h"
#include"game/Enemy/BulletEnemy.h"
#include <fstream>
#include <sstream>
WaveManager* WaveManager::GetInstance() {
	static WaveManager instance;
	return &instance;
}

void WaveManager::LoadFile() {
	std::string line;
	std::ifstream file("Resource/Wave/waveData.wave");
	assert(file.is_open());
	EnemyData* newEnemyData = nullptr;
	Wave* newWave = nullptr;
	waves_.clear();
	waves_.shrink_to_fit();
	while (std::getline(file, line)) {
		std::string identifilter;
		std::istringstream s(line);
		s >> identifilter;
		if (identifilter == "wave") {
			newWave=&waves_.emplace_back();
		}
		else if (identifilter == "enemy") {
			 newEnemyData = &newWave->enemyDatas.emplace_back();
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

void WaveManager::Update() {
	for (EnemyData& enemy : waves_[size_t(waveNum_)].enemyDatas) {
		if (currentFrame_ == enemy.frame) {
			//Enemyの生成処理	
			IEnemy* newEnemy;

			switch (enemy.type)
			{
			case kBullet:
				newEnemy = new BulletEnemy();
				Transform transform;
				transform.scale = { 1.0f,1.0f,1.0f };
				transform.rotate = {0,0,0};
				transform.translate = enemy.translate;
				WorldTransform w;
				newEnemy->Initialize(transform, enemy.velocity,enemy.speed, 2);

				enemyList_->push_back(newEnemy);
				break;
			}
		}
	}
	currentFrame_++;
	if (currentFrame_ >= waves_[size_t(waveNum_)].length) {
		if (waves_.size() -1> waveNum_) {
			waveNum_++;
		}
		currentFrame_ = 0;
	}
}