#include "MapManager.h"

MapManager* MapManager::GetInstance()
{
	static MapManager instance;
	return &instance;
}

void MapManager::Initialize() {
	
		MapRead();
		modelBlock_.reset(Model::CreateModelFromObj("Resource/cube", "cube.obj"));
	
}

void MapManager::MapRead()
{
	char readString[256];
	char* ptr;
	char* context = nullptr;
	FILE* fp = NULL;
	fopen_s(&fp, "Resource/Map/mapSample.csv", "rt");
	if (fp == NULL) {
		return;
	}
	mapWidth_ = 1;
	mapHeight_ = 1;
	uint32_t x = 0, y = 0;
	while (fgets(readString, sizeof(readString) / sizeof(char), fp) != NULL && y < kMapHeight) {
		ptr = strtok_s(readString, ",", &context);
		map[y][0] = MapState(atoi(ptr));
		
		x = 1;
		while (ptr != NULL && x < kMapWidth) {
			ptr = strtok_s(NULL, ",", &context);
			if (ptr != NULL) {
				map[y][x] = MapState(atoi(ptr));
			}
			
			x++;
		}
		y++;
		if (mapWidth_ < x) {
			mapWidth_ = x;
		}
		mapHeight_ = y;
	}
	fclose(fp);	
	MapBuild();
}

void MapManager::MapBuild() {
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{0,0,0});
	mapObject_.clear();
	floor_.clear();
	wall_.clear();
	for (uint32_t y = 0; y < mapHeight_;y++) {
		for (uint32_t x = 0; x < mapWidth_;x++) {
			if (map[y][x] != MapState::None) {
				WorldTransform worldTransform;
				worldTransform.Initialize();
				worldTransform.translation_.x = float(x);
				worldTransform.translation_.y = float( mapHeight_) - float(y);
				worldTransform.UpdateMatrix();
				OBB obb;
				obb.size = { worldTransform.scale_.x/2.0f,worldTransform.scale_.y / 2.0f,worldTransform.scale_.z / 2.0f };
				GetOrientations(rotateMatrix, obb.orientation);
				obb.center = worldTransform.translation_;
				Map object{ map[y][x] ,obb,worldTransform };
				object.moveDirection_ = 1.0f;
				mapObject_.push_back(object);
				if (map[y][x] == MapState::Block) {
					floor_.push_back(object);
				}
				else if (map[y][x] == MapState::Wall) {
					wall_.push_back(object);
				}
			}
		}
	}
}

void MapManager::Update() {
	for (Map& object : floor_) {
		object.Update();
	}
}

void MapManager::Draw(const ViewProjection& viewProjection) {
	/*for (Map& object : mapObject_) {
		modelBlock_->Draw(object.worldTransform,viewProjection);
	}*/
	for (Map& object : floor_) {
		modelBlock_->Draw(object.worldTransform, viewProjection);
	}
	for (Map& object : wall_) {
		modelBlock_->Draw(object.worldTransform,viewProjection);
	}
}

void MapManager::Map::Update() {
	if (moveFlag_ && !isTouch_) {
		//移動開始
		if (!isCollision_) {
			moveFlag_ = false;
			from.translation_ = worldTransform.translation_;
			to.translation_ = worldTransform.translation_;
			to.translation_.y += float(kBlockFloatForce) * moveDirection_;
			moveAnimationLength_ = kBlocckFloatAnimationLength;
			countUp_ = 0;
			isMove_ = true;
			isCollision_ = false;
			moveDirection_ *= -1.0f;
		}
		else {
			/*moveFlag_ = false;
			from.translation_ = worldTransform.translation_;
			to.translation_ = worldTransform.translation_;
			to.translation_.y -= float(kBlockFloatForce);
			moveAnimationLength_ = kBlocckFloatAnimationLength;
			countUp_ = 0;
			isMove_ = true;
			isCollision_ = false;*/
			//isMove_ = true;
			isCollision_ = false;
		}
		moveFlag_ = false;
	}
	//isCollision_ = false;
	isTouch_ = false;
	if (isMove_) {
		Move();
	}
	obb.center = worldTransform.translation_;
	worldTransform.UpdateMatrix();
}

void MapManager::Map::Move() {
	float t = float(countUp_) / float(moveAnimationLength_);
	worldTransform.translation_ = Lerp(t,from.translation_,to.translation_);
	if (countUp_ >= moveAnimationLength_) {
		countUp_ = 0;
		isMove_ = false;
	}
	countUp_++;
}

void MapManager::Map::OnCollision() {
	isCollision_ = true;
}

void MapManager::Map::Touch() {
	if (!isMove_) {
		isTouch_ = true;
		moveFlag_ = true;
	}
}