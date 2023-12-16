#pragma once
#include <stdint.h>
#include "MyMath.h"
#include "game/Player.h"
#include "game/Enemy/IEnemy.h"
#include "Sprite.h"
#include "TextureManager.h"
#include <Vector>
class GameScene;
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

	void LoadFile(const char filename[]);
	void LoadTutorialFile(const char filename[]);
	void LoadAllFiles();
	void SetWave(uint32_t waveNum) { waveNum_ = waveNum; currentFrame_ = 0; };
	size_t GetWave() { return waveNum_; };
	void SetBestWave(uint32_t waveNum) { bestWaveNum_ = waveNum; };
	size_t GetBestWave() { return bestWaveNum_; };
	size_t GetMaxWave() { return maxWaveNum_; };
	bool IsEnd() { return isEnd_; };
	void Initialize();
	void Update();
	void TutorialUpdate();
	void Draw();
	void DrawTutorial();
	void SetEnemyList(std::list<IEnemy*>* list) { enemyList_ = list; };
	void SetPlayer(Player* player) { player_ = player; };
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; };
	void ChangeNumAnimation();
	//void ChangeNumAnimation10();
	void GameClear() { isPastClear_ = true; };
	bool ISClear() {return isPastClear_;};
	bool isSpawn_;
	bool IsClearTutorial() { return isClearTutorialWave_; };
	void SetIsEnd(bool is) { isEnd_ = is; };
private:
	WaveManager() = default;
	~WaveManager() = default;
	WaveManager(const WaveManager&) = delete;
	WaveManager& operator=(const WaveManager&) = delete;

	std::vector<Wave> waves_;
	std::vector<Wave> tutorialWaves_;
	size_t waveNum_;
	size_t maxWaveNum_;
	std::list<IEnemy*> *enemyList_;
	int32_t currentFrame_;

	Player* player_;
	GameScene* gameScene_;

	std::unique_ptr<Sprite> lowNum0;
	std::unique_ptr<Sprite> lowNum1;
	std::unique_ptr<Sprite> highNum0;
	std::unique_ptr<Sprite> highNum1;

	uint32_t numberTextureHandle_[10];
	Vector3 left_ = {640.0f - 50.0f,360.0f,0};
	Vector3 right_ = { 640.0f + 50.0f,360.0f,0 };
	Vector3 numScale_;
	Vector3 centerPos0_;
	Vector3 topPos0_;
	Vector3 downPos0_;
	Vector3 centerPos1_;
	Vector3 topPos1_;
	Vector3 downPos1_;
	Vector3 s0;
	Vector3 s1;
	Vector3 s2;
	Vector3 s3;
	int num0=1;
	int num1=2;
	int num2=0;
	int num3=0;
	
	std::unique_ptr<Sprite> waveSprite_;
	uint32_t waveTextureHandle_;

	//Transform leftTransform_;
	//Transform RightTransform_;
	//表示用ウェーブ数
	size_t drawerWaveNum_;
	size_t bestWaveNum_=0;
	bool isChangeNum_;
	bool isChange10;
	int32_t changeAnimationLength_ = 30;
	int32_t changeAnimationCount_;
	int32_t waveInterval_ = 180;

	std::unique_ptr<Sprite> finalSprite_;
	uint32_t finalTextureHandle_;
	bool isFinal_;
	float finalScale_;
	float t_;

	uint32_t yokeroTextureHandle_;
	bool isFirst_;

	//終了検知
	bool isEnd_;
	bool isPastClear_ = false;
	float firstAlpha_;

	//チュートリアル用
	bool isClearTutorialWave_ = false;//チュートリアルの各ウェーブをクリアしたか

	std::unique_ptr<Sprite> tutorialSprite_;
	uint32_t tutorialTextureHandles_[3];
};

