#include "gameScene.h"
#include "MapManager.h"
#include "GameController.h"
#include "WaveManager.h"
#include <functional>
GameScene::~GameScene()
{
	enemys_.remove_if([](IEnemy* enemy) {

		delete enemy;
		return true;


		});
	bullets_.remove_if([](PlayerAimBullet* bullet) {
		delete bullet;
		return true;
		});
}

void GameScene::Initialize()
{
	blueMoon_ = BlueMoon::GetInstance();

	directXCommon_ = DirectXCommon::GetInstance();

	textureManager_ = Texturemanager::GetInstance();
	textureManager_->Initialize();
	enemyTex_ = textureManager_->Load("resource/black.png");
	viewProjection_.Initialize();

	GlovalVariables* globalVariables = GlovalVariables::GetInstance();
	const char* groupName = "GameSetting";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "fallingBorder", fallingBorder_);
	globalVariables->AddItem(groupName, "upperBorder", upperBorder_);
	globalVariables->AddItem(groupName, "horizonBorder", horizonBorder_);


	MapManager::GetInstance()->Initialize();
	//MapManager::GetInstance()->MapRead();
	MapManager::GetInstance()->SetJoyState(&joyState_);
	MapManager::GetInstance()->SetPreJoyState(&preJoyState_);
	
	player_.reset(new Player);
	player_->Initialize();
	player_->SetJoyState(&joyState_);
	player_->SetPreJoyState(&preJoyState_);
	GameController::GetInstance()->Initialize();

	followCamera_.reset(new FollowCamera);
	followCamera_->Initialize();
	//followCamera_->SetForcusPoint({0,1.0f,100.0f});
	followCamera_->SetTarget(&player_->GetWorldTransform());
	EnemymoveSpeed_ = 0.25f;
	enemyVelocity_ = { -1.0f,0.0f,0.0f };
	type = kBullet;
	enemyTransform = { {3.0f,3.0f,3.0f},{1.0f,1.0f,1.0f},{10.0f,10.0f,0.0f} };
	//EnemySpawn(player_->GetWorldTransform(),type);
	enemyPop_ = false;
	for (IEnemy* enemy : enemys_ ) {
		delete enemy;
	}
	enemys_.clear();
	waveNum_ = 0;
	WaveManager::GetInstance()->SetEnemyList(&enemys_);
	WaveManager::GetInstance()->SetWave(waveNum_);
	WaveManager::GetInstance()->SetGameScene(this);
	WaveManager::GetInstance()->SetPlayer(player_.get());
	WaveManager::GetInstance()->LoadFile();

	MapManager::GetInstance()->SetShakeCamera(std::bind(&FollowCamera::Shake, followCamera_.get()));
}

void GameScene::Update()
{
	ApplyGlobalVariables();
	//preJoyState_ = joyState_;
	//Input::GetInstance()->GetJoystickState(0, joyState_);
	GameController::GetInstance()->Update();
	if (Input::GetInstance()->PressKey(DIK_1)) {
		//textureManager_->Initialize();
		MapManager::GetInstance()->MapRead();
		player_->Initialize();
		enemyPop_ = false;
	}
	if (Input::GetInstance()->PressKey(DIK_2)) {
		MapManager::GetInstance()->WaveRead(waveNum_);
		WaveManager::GetInstance()->SetWave(waveNum_);
		//MapManager::GetInstance()->Clear();
	}

	ImGui::Begin("wave");
	ImGui::DragInt("wave", &waveNum_,1,0,30);
	ImGui::End();

	ImGui::Begin("testcamera");
	ImGui::DragFloat3("rotate", &viewProjection_.rotation_.x, 0.01f);
	ImGui::DragFloat3("translate", &viewProjection_.translation_.x, 0.01f);

	ImGui::End();
	ImGui::Begin("EnemyPopData");
	if (ImGui::BeginCombo("EnemyType", "Types")) {
		if (ImGui::Selectable("Bullet", type == kBullet)) {
			type = kBullet;
		}
		if (ImGui::Selectable("Bound", type == kBound)) {
			type = kBound;
		}
		if (ImGui::Selectable("Reflect", type == kReflect)) {
			type = kReflect;
		}
		if (ImGui::Selectable("StageDown", type == kStageDown)) {
			type = kStageDown;
		}
		if (ImGui::Selectable("StageUp", type == kStageUp)) {
			type = kStageUp;
		}
		if (ImGui::Selectable("Aimheight", type == kAimbulletheight)) {
			type = kAimbulletheight;
		}
		ImGui::EndCombo();

	}
	ImGui::DragFloat("speed", &EnemymoveSpeed_, 0.05f);
	ImGui::DragFloat3("velocity", &enemyVelocity_.x, 0.05f);
	ImGui::DragFloat3("translate", &enemyTransform.translate.x, 0.05f);
	ImGui::DragFloat3("scale", &enemyTransform.scale.x, 0.05f);
	ImGui::Checkbox("POP", &enemyPop_);
	ImGui::End();
	if (Input::GetInstance()->PushKey(DIK_E) || enemyPop_) {
		EnemySpawn(player_->GetWorldTransform(), type);
		enemyPop_ = false;
	}
	WaveManager::GetInstance()->Update();
	//viewProjection_.UpdateMatrix();
	//viewProjection_.TransferMatrix();

	ImGui::Begin("Scene");

	ImGui::InputInt("blendCount", &directXCommon_->count);
	ImGui::InputInt("SceneNum", &sceneNum);
	if (sceneNum > 1) {
		sceneNum = 1;
	}
	ImGui::End();
	player_->Update();
	if (player_->GetWorldTransform().GetWorldPos().y < fallingBorder_) {
		ReStartWave();
	}
	for (PlayerAimBullet* bullet : bullets_) {

		bullet->Update();
	}
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
		if (std::abs(enemy->GetWorldTransform().GetWorldPos().x) > horizonBorder_ || 
			enemy->GetWorldTransform().GetWorldPos().y > upperBorder_ || 
			enemy->GetWorldTransform().GetWorldPos().y < fallingBorder_) {
			enemy->Deth();
		}
	}
	MapManager::GetInstance()->Update();
	std::vector<std::shared_ptr<MapManager::Map>>& floors = MapManager::GetInstance()->GetFloor();
	for (std::shared_ptr<MapManager::Map> object : floors) {
		if (IsCollision(player_->GetOBB(), object->obb)) {
			object->isFrameCollision_ = player_->OnCollisionFloorVertical(object->obb);
			object->OnCollision();
		}
		if (IsCollision(player_->GetFloatTrigger(), object->obb)) {
			object->Touch();
		}
	}
	for (std::shared_ptr<MapManager::Map> object : floors) {
		if (IsCollision(player_->GetOBB(), object->obb) && (object->isFrameCollision_ == false)) {
			player_->OnCollisionFloorHorizon(object->obb);
		}
	}

	std::vector<std::shared_ptr<MapManager::Map>>& walls = MapManager::GetInstance()->GetWall();
	for (std::shared_ptr<MapManager::Map> object : walls) {
		if (IsCollision(player_->GetOBB(), object->obb)) {
			player_->OnCollisionWall(object->obb);
		}
	}
	for (IEnemy* enemy : enemys_) {
		if (IsCollision(enemy->GetOBB(),player_->GetOBB())) {
			//Initialize();
			ReStartWave();
			return;
		}
		for (std::shared_ptr<MapManager::Map> object : floors) {
			if (IsCollision(enemy->GetOBB(), object->obb) && !enemy->GetIsHit()) {
				
					if (enemy->GetType() == kStageUp) {
						if (object->worldTransform.translation_.y == 0.0f) {
							object->OnCollision();
							object->Touch();
						}
					}
					if (enemy->GetType() == kStageDown) {
						if (object->worldTransform.translation_.y == 4.0f) {
							object->OnCollision();
							object->Touch();
						}
					}
				
				enemy->SetPartener(kflore);
				enemy->isCollision(object->obb);
				
			}
		}
		if (enemy->GetType() == kReflect) {
			for (std::shared_ptr<MapManager::Map> object : walls) {
				if (IsCollision(enemy->GetOBB(), object->obb)) {
					enemy->SetPartener(kwall);
					enemy->isCollision(object->obb);

				}
			}
		}

	}
	followCamera_->Update();

	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();
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
	for (PlayerAimBullet* bullet : bullets_) {

		bullet->Draw(viewProjection_);
	}
	blueMoon_->PariclePreDraw();

	blueMoon_->ModelPreDrawWireFrame();


}

void GameScene::ApplyGlobalVariables()
{
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();
	const char* groupName = "GameSetting";
	fallingBorder_ = globalVariables->GetFloatValue(groupName, "fallingBorder");
	upperBorder_ = globalVariables->GetFloatValue(groupName, "upperBorder");
	horizonBorder_ = globalVariables->GetFloatValue(groupName, "horizonBorder");
}

void GameScene::EnemySpawn(const WorldTransform& worldTransform, EnemyType type)
{
	IEnemy* enemy;

	switch (type)
	{
	case kBullet:
		enemy = new BulletEnemy();
		enemy->Initialize(enemyTransform, enemyVelocity_, EnemymoveSpeed_, enemyTex_);

		enemys_.push_back(enemy);
		break;
	case kReflect:
		enemy = new ReflectEnemy();
		enemy->Initialize(enemyTransform, enemyVelocity_, EnemymoveSpeed_, enemyTex_);

		enemys_.push_back(enemy);
		break;
	case kBound:
		enemy = new BoundEnemy();
		//{ 0.3f, -1.0f, 0.0f }
		enemy->Initialize(enemyTransform, enemyVelocity_, EnemymoveSpeed_, enemyTex_);

		enemys_.push_back(enemy);
		break;
	case kTire:
		break;
	case kSpear:
		break;
	case kRaser:
		break;
	case kAimBulletWidth:
		break;
	case kAimbulletheight:
		enemy = new AimBulletEnemy();
		//{ 0.3f, -1.0f, 0.0f }
		enemy->Initialize(enemyTransform, enemyVelocity_, EnemymoveSpeed_, enemyTex_);
		enemy->SetPlayer(player_.get());
		enemy->SetGameScene(this);
		enemys_.push_back(enemy);
		break;
	case kAimBound:
		break;
	case kStageUp:
		enemy = new StageChangeEnemy();
		//{ 0.3f, -1.0f, 0.0f }
		enemy->Initialize(enemyTransform, enemyVelocity_, EnemymoveSpeed_, enemyTex_);
		enemy->SetType(kStageUp);
		enemys_.push_back(enemy);
		break;
	case kStageDown:
		enemy = new StageChangeEnemy();
		enemy->Initialize(enemyTransform, enemyVelocity_, EnemymoveSpeed_, enemyTex_);
		enemy->SetType(kStageDown);
		enemys_.push_back(enemy);
		break;
	case kHoming:
		break;
	default:
		enemy = new ReflectEnemy();
		enemy->Initialize(enemyTransform, enemyVelocity_, EnemymoveSpeed_, enemyTex_);

		enemys_.push_back(enemy);
		break;
	}

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

void GameScene::AddEnemyBullet(PlayerAimBullet* enemyBullet)
{
	bullets_.push_back(enemyBullet);
}

void GameScene::ReStartWave()
{
	for (IEnemy* enemy : enemys_) {
		delete enemy;
	}
	enemys_.clear();
	size_t num = WaveManager::GetInstance()->GetWave();
	MapManager::GetInstance()->WaveRead(uint32_t(num));
	WaveManager::GetInstance()->SetWave(uint32_t(num));
	player_->Reset();
}
