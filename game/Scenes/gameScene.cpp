#include "gameScene.h"
#include "MapManager.h"

GameScene::~GameScene()
{
	enemys_.remove_if([](IEnemy* enemy) {

		delete enemy;
		return true;


		});
}

void GameScene::Initialize()
{
	blueMoon_ = BlueMoon::GetInstance();

	directXCommon_ = DirectXCommon::GetInstance();

	textureManager_ = Texturemanager::GetInstance();
	textureManager_->Initialize();
	enemyTex_=textureManager_->Load("resource/black.png");
	viewProjection_.Initialize();
	
	MapManager::GetInstance()->Initialize();
	MapManager::GetInstance()->MapRead();
	player_.reset(new Player);
	player_->Initialize();
	EnemyVelocity_ = 0.25f;
	EnemySpawn(player_->GetWorldTransform(),kBound);
}

void GameScene::Update()
{
	if (Input::GetInstance()->PressKey(DIK_1)) {
		textureManager_->Initialize();
		MapManager::GetInstance()->MapRead();
		player_->Initialize();
	}
	ImGui::Begin("testcamera");
	ImGui::DragFloat3("rotate", &viewProjection_.rotation_.x,0.01f);
	ImGui::DragFloat3("translate", &viewProjection_.translation_.x,0.01f);
	
	ImGui::End();
	if (Input::GetInstance()->PushKey(DIK_E)) {
		EnemySpawn(player_->GetWorldTransform(),kBound);
	}
	if (Input::GetInstance()->PushKey(DIK_S)) {
		EnemySpawn(player_->GetWorldTransform(), kBound);
	}
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
	enemys_.remove_if([](IEnemy* enemy) {
		if (!enemy->GetIsAlive()) {
			delete enemy;
			return true;
		}
		return false;
		});

	/*std::vector<MapManager::Map>& mapObjects = MapManager::GetInstance()->GetMapObject();
	for (MapManager::Map & object : mapObjects) {
		if (IsCollision(player_->GetOBB(), object.obb)) {
			player_->OnCollision(object.obb);
		}
	}*/
	for (IEnemy* enemy : enemys_) {
		enemy->Update();
	}
	MapManager::GetInstance()->Update();
	std::vector<MapManager::Map>& floors = MapManager::GetInstance()->GetFloor();
	for (MapManager::Map& object : floors) {
		if (IsCollision(player_->GetOBB(), object.obb)) {
			player_->OnCollisionFloor(object.obb);
			object.OnCollision();
		}
		if (IsCollision(player_->GetFloatTrigger(), object.obb)) {
			object.Touch();
		}
	}

	std::vector<MapManager::Map>& walls = MapManager::GetInstance()->GetWall();
	for (MapManager::Map& object : walls) {
		if (IsCollision(player_->GetOBB(), object.obb)) {
			player_->OnCollisionWall(object.obb);
		}
	}
	for (IEnemy* enemy : enemys_) {
		for (MapManager::Map& object : floors) {
			if (IsCollision(enemy->GetOBB(), object.obb)&&!enemy->GetIsHit()) {
				
				enemy->SetPartener(kflore);
				enemy->isCollision(object.obb);
				
			}
		}
		if (enemy->GetType() == kReflect) {
			for (MapManager::Map& object : walls) {
				if (IsCollision(enemy->GetOBB(), object.obb)) {
					enemy->SetPartener(kwall);
						enemy->isCollision(object.obb);
						
				}
			}
		}
	
	}
	
	ImGui::Begin("velo");
	ImGui::DragFloat("veloc", &EnemyVelocity_, 0.05f);
	ImGui::End();
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
	for (IEnemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}
	blueMoon_->PariclePreDraw();
	
	blueMoon_->ModelPreDrawWireFrame();
	
	
}

void GameScene::ApplyGlobalVariables()
{
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();

	const char* groupName = "Player";

}

void GameScene::EnemySpawn(const WorldTransform& worldTransform, EnemyType type)
{
	IEnemy* enemy;
	Transform trans = { {1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{10.0f,10.0f,0.0f} };
	if (type == kBound) {
		enemy =new BoundEnemy() ;
	}
	else {
		enemy = new BoundEnemy();
	}
	enemy->Initialize(trans,{2.0f,0.0f,0.0f},0.25f,enemyTex_,player_->GetWorldTransform());

	enemys_.push_back(enemy);
}

void GameScene::Draw2D() {
	blueMoon_->SetBlendMode(blendCount_);
	

}
void GameScene::Finalize()
{
	enemys_.remove_if([](IEnemy* enemy) {
		
			delete enemy;
			return true;
		
		
		});
}

