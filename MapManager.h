#pragma once
#include <stdint.h>
#include "WorldTransform.h"
#include "model.h"
#include <memory>
#include <vector>
#include "Mymath.h"
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
	};

	static MapManager* GetInstance();
	void Initialize();
	
	void Update();
	//CSVからマップ情報を格納する
	void MapRead();
	//マップ情報からWorldtransformをさくせいしてリストする
	void MapBuild();

	void Draw(const ViewProjection&);

	std::vector<Map>& GetMapObject() { return mapObject_; };

	//マップの読み込み最大幅
	static const uint32_t kMapWidth = 20;
	//マップの読み込み最大高さ
	static const uint32_t kMapHeight = 15;
private:
	MapManager() = default;
	~MapManager() = default;
	MapManager(const MapManager&) = delete;
	MapManager& operator=(const MapManager&) = delete;

	std::unique_ptr<Model> modelBlock_;

	std::vector<Map> mapObject_;

	MapState map[kMapHeight][kMapWidth];
	uint32_t mapWidth_;
	uint32_t mapHeight_;
};

