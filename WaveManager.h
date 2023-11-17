#pragma once
#include <stdint.h>
#include "MyMath.h"
#include "game/Enemy/IEnemy.h"
#include <Vector>
class WaveManager
{
public:

	struct EnemyData
	{
		Vector3 translate;
		Vector3 velocity;
		float speed;
		EnemyType type;
		int32_t frame;
	};

	struct Wave
	{
		std::list<EnemyData> enemyDatas;
		int32_t length;
	};

	static WaveManager* GetInstance();

	void LoadFile();
	void SetWave(uint32_t waveNum) { waveNum_ = waveNum; currentFrame_ = 0; };

	void Update();

	void SetEnemyList(std::list<IEnemy*>* list) { enemyList_ = list; };

private:
	WaveManager() = default;
	~WaveManager() = default;
	WaveManager(const WaveManager&) = delete;
	WaveManager& operator=(const WaveManager&) = delete;

	std::vector<Wave> waves_;
	size_t waveNum_;

	std::list<IEnemy*> *enemyList_;
	int32_t currentFrame_;
};

