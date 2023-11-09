#include "gameScene.h"
#include "MapManager.h"

GameScene::~GameScene()
{
	
}

void GameScene::Initialize()
{
	blueMoon_ = BlueMoon::GetInstance();

	directXCommon_ = DirectXCommon::GetInstance();

	textureManager_ = Texturemanager::GetInstance();
	textureManager_->Initialize();
	viewProjection_.Initialize();
	
	MapManager::GetInstance()->Initialize();
	MapManager::GetInstance()->MapRead();
	MapManager::GetInstance()->SetJoyState(&joyState_);
	MapManager::GetInstance()->SetPreJoyState(&preJoyState_);

	player_.reset(new Player);
	player_->Initialize();
	player_->SetJoyState(&joyState_);
	player_->SetPreJoyState(&preJoyState_);
}

void GameScene::Update()
{
	preJoyState_ = joyState_;
	Input::GetInstance()->GetJoystickState(0, joyState_);
	if (Input::GetInstance()->PressKey(DIK_1)) {
		textureManager_->Initialize();
		MapManager::GetInstance()->MapRead();
		player_->Initialize();
	}
	ImGui::Begin("testcamera");
	ImGui::DragFloat3("rotate", &viewProjection_.rotation_.x,0.01f);
	ImGui::DragFloat3("translate", &viewProjection_.translation_.x,0.01f);
	
	ImGui::End();

	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	ImGui::Begin("Scene");
	
	ImGui::InputInt("blendCount", &blendCount_);
	ImGui::InputInt("SceneNum", &sceneNum);
	if (sceneNum > 1) {
		sceneNum = 1;
	}
	ImGui::End();
	player_->Update();
	/*std::vector<MapManager::Map>& mapObjects = MapManager::GetInstance()->GetMapObject();
	for (MapManager::Map & object : mapObjects) {
		if (IsCollision(player_->GetOBB(), object.obb)) {
			player_->OnCollision(object.obb);
		}
	}*/
	MapManager::GetInstance()->Update();
	std::vector<MapManager::Map>& floors = MapManager::GetInstance()->GetFloor();
	for (MapManager::Map& object : floors) {
		if (IsCollision(player_->GetOBB(), object.obb)) {
			player_->OnCollisionFloorVertical(object.obb);
			object.OnCollision();
		}
		if (IsCollision(player_->GetFloatTrigger(), object.obb)) {
			object.Touch();
		}
	}
	for (MapManager::Map& object : floors) {
		if (IsCollision(player_->GetOBB(), object.obb)) {
			player_->OnCollisionFloorHorizon(object.obb);
		}
	}

	std::vector<MapManager::Map>& walls = MapManager::GetInstance()->GetWall();
	for (MapManager::Map& object : walls) {
		if (IsCollision(player_->GetOBB(), object.obb)) {
			player_->OnCollisionWall(object.obb);
		}
	}
}


void GameScene::Draw()
{
	
	//3D描画準備
	blueMoon_->ModelPreDraw();
	Draw3D();
	//2D描画準備
	blueMoon_->SpritePreDraw();
	Draw2D();
}

void GameScene::Draw3D()
{
	blueMoon_->ModelPreDraw();
	
	MapManager::GetInstance()->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	blueMoon_->PariclePreDraw();
	
	blueMoon_->ModelPreDrawWireFrame();
	
	
}

void GameScene::ApplyGlobalVariables()
{
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();

	const char* groupName = "Player";

}

void GameScene::Draw2D() {
	blueMoon_->SetBlendMode(blendCount_);
	

}
void GameScene::Finalize()
{
	
}

