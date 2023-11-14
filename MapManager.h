#pragma once
#include <stdint.h>
#include "WorldTransform.h"
#include "model.h"
#include <memory>
#include <vector>
#include "Mymath.h"
#include "Input.h"
class MapManager
{
public:
	enum class MapState {
		None,
		Block,
		Wall,
	};
	struct Map {
		MapState mapState;
		OBB obb;
		WorldTransform worldTransform;
		bool isCollision_;
		bool isTouch_;
		bool moveFlag_;
		bool isMove_;
		uint32_t countUp_;
		WorldTransform from;
		WorldTransform to;
		uint32_t moveAnimationLength_;
		void Update();
		void Move();
		void OnCollision() ;
		void Touch();
		float moveDirection_;
		void Reverse();
		uint32_t delay_;
		bool isFrameCollision_;
		uint32_t id;
	};

	static MapManager* GetInstance();
	void Initialize();
	
	void Update();
	//CSVからマップ情報を格納する
	void MapRead();
	//マップ情報からWorldtransformをさくせいしてリストする
	void MapBuild();

	//指定ウェーブの床の位置を読み取って移動させる
	void WaveRead(uint32_t wave);

	void Draw(const ViewProjection&);

	std::vector<Map>& GetMapObject() { return mapObject_; };
	std::vector<Map>& GetFloor() { return floor_; };
	std::vector<Map>& GetWall() { return wall_; };

	void ApplyGlobalVariables();

	void SetJoyState(XINPUT_STATE* joystate) { joyState_ = joystate; };
	void SetPreJoyState(XINPUT_STATE* joystate) { preJoyState_ = joystate; };


	//マップの読み込み最大幅
	static const uint32_t kMapWidth = 30;
	//マップの読み込み最大高さ
	static const uint32_t kMapHeight = 20;

	//ブロックの浮き上がりの高さ
	static uint32_t kBlockFloatForce;
	static uint32_t kBlocckFloatAnimationLength;
	static uint32_t kBlocckFloatAnimationDelay;
	static uint32_t kReverseFloatAnimationDelay;
private:
	MapManager() = default;
	~MapManager() = default;
	MapManager(const MapManager&) = delete;
	MapManager& operator=(const MapManager&) = delete;

	XINPUT_STATE* joyState_;
	XINPUT_STATE* preJoyState_;

	std::unique_ptr<Model> modelBlock_;

	std::vector<Map> mapObject_;

	std::vector<Map> floor_;
	std::vector<Map> wall_;


	MapState map[kMapHeight][kMapWidth];
	uint32_t mapWidth_;
	uint32_t mapHeight_;
};

