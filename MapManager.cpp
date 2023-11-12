#include "MapManager.h"
#include "Globalvariables.h"
#include "Easing.h"
#include "GameController.h"
//#include "Input.h"
uint32_t MapManager::kBlockFloatForce = 4;
uint32_t MapManager::kBlocckFloatAnimationLength = 60;
uint32_t MapManager::kBlocckFloatAnimationDelay = 2;
uint32_t MapManager::kReverseFloatAnimationDelay = 2;
MapManager* MapManager::GetInstance()
{
	static MapManager instance;
	return &instance;
}

void MapManager::Initialize() {
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();
	const char* groupName = "Map";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "FloatForce", 4);
	globalVariables->AddItem(groupName, "FloatAnimationLength", 60);
	globalVariables->AddItem(groupName, "FloatAnimationDelay", 2);
	globalVariables->AddItem(groupName, "ReverseAnimationDelay", 2);

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
			if (map[y][x] == MapState::Wall) {
				//mapWidth_ = x+1;
				if (mapWidth_ < x) {
					mapWidth_ = x + 1;
				}
			}
			x++;
			
		}
		y++;
		/*if (mapWidth_ < x) {
			mapWidth_ = x;
		}*/
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
				worldTransform.scale_ = {2.0f,2.0f,2.0f};
				worldTransform.translation_.x = float(int(x) - int(mapWidth_/2))*2.0f*worldTransform.scale_.x;
				worldTransform.translation_.y = (float( mapHeight_) - float(y+1))*2.0f * worldTransform.scale_.y;
				worldTransform.UpdateMatrix();
				OBB obb;
				obb.size = { worldTransform.scale_.x,worldTransform.scale_.y,worldTransform.scale_.z };
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
	ApplyGlobalVariables();
	for (Map& object : floor_) {
		object.Update();
		if (GameController::GetInstance()->Reverse()) {
			object.Reverse();
			object.delay_ = kReverseFloatAnimationDelay;
		}
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
	isFrameCollision_ = false;
	if (moveFlag_ && !isTouch_) {
		//移動開始
		if (isCollision_) {
			Reverse();
			delay_ = kBlocckFloatAnimationDelay;
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

void MapManager::Map::Reverse() {
	moveFlag_ = false;
	from.translation_ = worldTransform.translation_;
	to.translation_ = worldTransform.translation_;
	to.translation_.y = float(kBlockFloatForce) * (((moveDirection_) + 1.0f) / 2.0f) + 0.0f;
	moveAnimationLength_ = kBlocckFloatAnimationLength;
	countUp_ = 0;
	isMove_ = true;
	isCollision_ = false;
	moveDirection_ *= -1.0f;
}

void MapManager::Map::Move() {
	float t = float(countUp_) / float(moveAnimationLength_);
	float easedT = EaseIn(t,delay_);
	worldTransform.translation_ = Lerp(easedT,from.translation_,to.translation_);
	if (countUp_ >= moveAnimationLength_) {
		countUp_ = 0;
		isMove_ = false;
	}
	countUp_++;
}

void MapManager::Map::OnCollision() {
	isCollision_ = true;
	//isFrameCollision_ = true;
}

void MapManager::Map::Touch() {
	if (!isMove_) {
		isTouch_ = true;
		moveFlag_ = true;
	}
}

void MapManager::ApplyGlobalVariables()
{
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();
	const char* groupName = "Map";
	kBlockFloatForce = globalVariables->GetIntValue(groupName, "FloatForce");
	kBlocckFloatAnimationLength = globalVariables->GetIntValue(groupName, "FloatAnimationLength");
	kBlocckFloatAnimationDelay = globalVariables->GetIntValue(groupName, "FloatAnimationDelay");
	kReverseFloatAnimationDelay = globalVariables->GetIntValue(groupName, "ReverseAnimationDelay");
}