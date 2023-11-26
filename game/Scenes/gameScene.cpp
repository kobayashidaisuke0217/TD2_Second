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
	sceneNum = 1;
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

	globalVariables->AddItem(groupName, "transitionAnimationLength", int32_t(transitionAnimationLength_));
	globalVariables->AddItem(groupName, "transitionAnimationDelay", int32_t(transitionAnimationDelay_));

	const char* groupName2 = "Title";
	globalVariables->AddItem(groupName2, "lineScale", lineScale_);
	globalVariables->AddItem(groupName2, "linePosition", linePosition_);
	globalVariables->AddItem(groupName2, "startScale", lineScale_);
	globalVariables->AddItem(groupName2, "startPosition", linePosition_);


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
	bullets_.clear();
	waveNum_ = 0;
	WaveManager::GetInstance()->SetEnemyList(&enemys_);
	WaveManager::GetInstance()->SetWave(waveNum_);
	WaveManager::GetInstance()->SetGameScene(this);
	WaveManager::GetInstance()->SetPlayer(player_.get());
	WaveManager::GetInstance()->LoadAllFiles();

	MapManager::GetInstance()->SetShakeCamera(std::bind(&FollowCamera::Shake, followCamera_.get()));
	transitionSprite_.reset(new Sprite);
	transitionSprite_->Initialize({0.0f,0.0f,0.0f,0.0f}, {1280.0f,720.0f,0.0f,0.0f});
	blackTextureHandle_ = textureManager_->Load("resource/startLine.png");
	
	titleLine_.reset(new Plane);
	titleLine_->Initialize();
	worldTransformLine_.Initialize();
	
	titleChar_.reset(new Plane);
	titleChar_->Initialize();
	worldTransformStart_.Initialize();



	titleTransform_.scale = {0.5f,0.5f,0.5f};
	titleTransform_.rotate = {0,0,0};
	titleTransform_.translate = {640,360,0};

	titleSprite_.reset(new Sprite);
	titleSprite_->Initialize({-960,-540,0,0}, { 960,540,0,0 });

	backGroundSprite_.reset(new Sprite);
	backGroundSprite_->Initialize({ 0,0,0,0 }, {1280,720,0,0});
	titleTextureHandle_ = textureManager_->Load("resource/title.png");
	backTextureHandle_ = textureManager_->Load("resource/back.png");
	startTextureHandle_ = textureManager_->Load("resource/gameStart.png");

	globalVariables->AddItem(groupName2, "TitleScale", titleTransform_.scale);
	globalVariables->AddItem(groupName2, "TitleTransform", titleTransform_.translate);

	moveSprite_.reset(new Sprite);
	moveSprite_->Initialize({ -800.0f,-200.0f,0,0 }, { 800.0f,200.0f,0,0 });
	jumpSprite_.reset(new Sprite);
	jumpSprite_->Initialize({ -800.0f,-200.0f,0,0 }, { 800.0f,200.0f,0,0 });
	reverseSprite_.reset(new Sprite);
	reverseSprite_->Initialize({ -800.0f,-200.0f,0,0 }, { 800.0f,200.0f,0,0 });

	moveTextureHandle_ = textureManager_->Load("Resource/UI/moveUI.png");
	jumpTextureHandle_ = textureManager_->Load("Resource/UI/jumpUI.png");
	reverseTextureHandle_ = textureManager_->Load("Resource/UI/reversUI.png");

	const char* groupName3 = "UI";
	globalVariables->AddItem(groupName3, "moveScale", move_.scale);
	globalVariables->AddItem(groupName3, "movePosition", move_.translate);
	globalVariables->AddItem(groupName3, "jumpScale", jump_.scale);
	globalVariables->AddItem(groupName3, "jumpPosition", jump_.translate);
	globalVariables->AddItem(groupName3, "reverseScale", reverse_.scale);
	globalVariables->AddItem(groupName3, "reversePosition", reverse_.translate);


	isInGame_ = false;
	isTitle_ = true;
	isStartGame_ = false;
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
		if (ImGui::Selectable("Aimheight", type == kAimBulletHeight)) {
			type = kAimBulletHeight;
		}
		if (ImGui::Selectable("AimWidth", type == kAimBulletWidth)) {
			type = kAimBulletWidth;
		}
		if (ImGui::Selectable("Tire", type == kTire)) {
			type = kTire;
		}
		if (ImGui::Selectable("Beam", type == kRaser)) {
				type = kRaser;
		}
		if (ImGui::Selectable("AimBall", type == kAimBound)) {
			type = kAimBound;
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
	
	if (isTitle_) {
		Title();
	}
	else if (isInGame_) {
		InGame();
	}
	else {
		player_->Update();
		ReStartAnimation();
	}
	if (isRunAnimation_) {
		TransitionAnimation();
	}

	followCamera_->Update();

	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();
	Matrix4x4 toCameraRotate = Inverse(viewProjection_.matView);
	toCameraRotate.m[3][0] = 0;
	toCameraRotate.m[3][1] = 0;
	toCameraRotate.m[3][2] = 0;

	worldTransformStart_.matWorld_ = Multiply(Multiply( MakeScaleMatrix(worldTransformStart_.scale_) , toCameraRotate),MakeTranslateMatrix(worldTransformStart_.translation_));
	worldTransformStart_.TransferMatrix();
}


void GameScene::Title() {
	
	player_->Update();
	if (player_->GetWorldTransform().GetWorldPos().y < fallingBorder_) {
		followCamera_->Shake();
		ReStart();
	}

	if (player_->GetWorldTransform().GetWorldPos().y > linePosition_.y) {
		isStartGame_ = true;
		isInGame_ = true;
		isTitle_ = false;
		//WaveManager::GetInstance()->SetWave(0);
	}

	worldTransformLine_.translation_ = linePosition_;
	worldTransformLine_.scale_ = lineScale_;
	worldTransformLine_.UpdateMatrix();
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
	
	if (isRunAnimation_) {
		resetT_ = frameCount_ / float(transitionAnimationLength_);
		resetT_ = std::powf(resetT_ * 2.0f - 1.0f, 2) * -1.0f + 1.0f;

		if (frameCount_ >= transitionAnimationLength_) {
			isRunAnimation_ = false;
		}
		frameCount_++;
	}
}

void GameScene::InGame() {
	WaveManager::GetInstance()->Update();
	//viewProjection_.UpdateMatrix();
	//viewProjection_.TransferMatrix();

	ImGui::Begin("Scene");

	
	ImGui::InputInt("SceneNum", &sceneNum);
	if (sceneNum > 1) {
		sceneNum = 1;
	}
	ImGui::End();
	player_->Update();
	if (player_->GetWorldTransform().GetWorldPos().y < fallingBorder_) {
		followCamera_->Shake();
		ReStart();
	}
	for (PlayerAimBullet* bullet : bullets_) {

		bullet->Update();
	}
	bullets_.remove_if([](PlayerAimBullet* bullet) {
		if (!bullet->GetIsAlive()) {
			delete bullet;
			return true;
		}
		return false;
		});
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
		for (PlayerAimBullet* bullet : bullets_) {
			if (IsCollision(bullet->GetOBB(), object->obb)) {
 				bullet->isCollision();
			}
			if (IsCollision(bullet->GetOBB(), player_->GetOBB())) {
				Initialize();
				return;
			}
			
		}

	}

	std::vector<std::shared_ptr<MapManager::Map>>& walls = MapManager::GetInstance()->GetWall();
	for (std::shared_ptr<MapManager::Map> object : walls) {
		if (IsCollision(player_->GetOBB(), object->obb)) {
			player_->OnCollisionWall(object->obb);
		}
	}
	for (IEnemy* enemy : enemys_) {
		if (IsCollision(enemy->GetOBB(), player_->GetOBB())) {
			//Initialize();
			ReStart();
			followCamera_->Shake();
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
	if (isRunAnimation_) {
		resetT_ = frameCount_ / float(transitionAnimationLength_);
		resetT_ = std::powf(resetT_ * 2.0f - 1.0f, 2) * -1.0f + 1.0f;

		if (frameCount_ >= transitionAnimationLength_) {
			isRunAnimation_ = false;
		}
		frameCount_++;
	}
}

void GameScene::Draw()
{
	//2D描画準備
	blueMoon_->SpritePreDraw();
	DrawBackGround();
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
	if (!isStartGame_) {
		titleLine_->Draw(worldTransformLine_, viewProjection_, { 1.0f,1.0f ,1.0f ,1.0f }, blackTextureHandle_);
		titleChar_->Draw(worldTransformStart_, viewProjection_,{1.0f,1.0f,1.0f,1.0f},startTextureHandle_);
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
	transitionAnimationLength_ = globalVariables->GetIntValue(groupName, "transitionAnimationLength");
	transitionAnimationDelay_ = globalVariables->GetIntValue(groupName, "transitionAnimationDelay");


	const char* groupName2 = "Title";
	lineScale_ = globalVariables->GetVector3Value(groupName2, "lineScale");
	linePosition_ = globalVariables->GetVector3Value(groupName2, "linePosition");

	titleTransform_.scale = globalVariables->GetVector3Value(groupName2, "TitleScale");
	titleTransform_.translate = globalVariables->GetVector3Value(groupName2, "TitleTransform");
	worldTransformStart_.scale_ = globalVariables->GetVector3Value(groupName2, "startScale");
	worldTransformStart_.translation_ = globalVariables->GetVector3Value(groupName2, "startPosition");

	const char* groupName3 = "UI";
	move_.scale = globalVariables->GetVector3Value(groupName3, "moveScale");
	move_.translate = globalVariables->GetVector3Value(groupName3, "movePosition");
	jump_.scale = globalVariables->GetVector3Value(groupName3, "jumpScale");
	jump_.translate = globalVariables->GetVector3Value(groupName3, "jumpPosition");
	reverse_.scale = globalVariables->GetVector3Value(groupName3, "reverseScale");
	reverse_.translate = globalVariables->GetVector3Value(groupName3, "reversePosition");

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
		enemy = new TireEnemy();
		//{ 0.3f, -1.0f, 0.0f }
		enemy->Initialize(enemyTransform, enemyVelocity_, EnemymoveSpeed_, enemyTex_);

		enemys_.push_back(enemy);
		break;
	case kSpear:
		break;
	case kRaser:
		enemy = new BeamEnemy();
		//{ 0.3f, -1.0f, 0.0f }
		enemy->Initialize(enemyTransform, enemyVelocity_, EnemymoveSpeed_, enemyTex_);

		enemys_.push_back(enemy);
		break;
	case kAimBulletWidth:
		enemy = new AImBulletWidthEnemy();
		//{ 0.3f, -1.0f, 0.0f }
		enemy->Initialize(enemyTransform, enemyVelocity_, EnemymoveSpeed_, enemyTex_);
		enemy->SetPlayer(player_.get());
		enemy->SetGameScene(this);
		enemys_.push_back(enemy);
		break;
	case kAimBulletHeight:
		enemy = new AimBulletEnemy();
		//{ 0.3f, -1.0f, 0.0f }
		enemy->Initialize(enemyTransform, enemyVelocity_, EnemymoveSpeed_, enemyTex_);
		enemy->SetPlayer(player_.get());
		enemy->SetGameScene(this);
		enemys_.push_back(enemy);
		break;
	case kAimBound:
		enemy = new PlayerAimBallEnemy();
		//{ 0.3f, -1.0f, 0.0f }
		enemy->Initialize(enemyTransform, enemyVelocity_, EnemymoveSpeed_, enemyTex_);
		enemy->SetPlayer(player_.get());
		enemy->SetGameScene(this);
		enemys_.push_back(enemy);
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
	Transform uv = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f} };

	moveSprite_->Draw(move_, uv, {1.0f,1.0f,1.0f,1.0f},moveTextureHandle_);
	jumpSprite_->Draw(jump_, uv, { 1.0f,1.0f,1.0f,1.0f }, jumpTextureHandle_);
	reverseSprite_->Draw(reverse_, uv, { 1.0f,1.0f,1.0f,1.0f }, reverseTextureHandle_);
	if (isTitle_){
		titleSprite_->Draw(titleTransform_, uv, {1.0f,1.0f,1.0f,1.0f},titleTextureHandle_);
	}
	if (isRunAnimation_) {
		Transform pos = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{transitionSpritePosition_.x,transitionSpritePosition_.y,transitionSpritePosition_.z} };
		transitionSprite_->Draw(pos, uv,{ 0.0f,0.0f,0.0f,1.0f } ,blackTextureHandle_);
	}
}

void GameScene::DrawBackGround() {
	blueMoon_->SetBlendMode(blendCount_);
	Transform uv = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f} };
	Transform pos = { {1.0f,1.0f,0.0f},{0.0f,0.0f,0.0f},{0,0,0} };
	backGroundSprite_->Draw(pos, uv, { 1.0f,1.0f,1.0f,1.0f }, backTextureHandle_);
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

void GameScene::ReStart()
{
	for (IEnemy* enemy : enemys_) {
		delete enemy;
	}
	enemys_.clear();
	player_->DethAnimation();
	isInGame_ = false;
	isTitle_ = false;
	frameCount_ = 0;
	isRunAnimation_ = false;
}

void GameScene::ReStartAnimation() {
	if (!isRunAnimation_ && frameCount_ >= transitionAnimationDelay_){
		isRunAnimation_ = true;
		frameCount_ = 0;
	}
	if (isRunAnimation_) {
		resetT_ = frameCount_ / float(transitionAnimationLength_);
		resetT_ = std::powf(resetT_*2.0f-1.0f, 2)*-1.0f + 1.0f;
		
		if (frameCount_ == transitionAnimationLength_/2) {
			ReStartWave();
			if (isStartGame_) {
				isInGame_ = true;
			}
			else {
				isTitle_ = true;
			}
		}

		if (frameCount_ >= transitionAnimationLength_) {
			isRunAnimation_ = false;
			isInGame_ = true;
		}
	}
	frameCount_++;
}

void GameScene::TransitionAnimation() {

	transitionSpritePosition_.x = (1.0f - resetT_) * transitionStartPosition_.x + resetT_ * transitionEndPosition_.x;
	transitionSpritePosition_.y = (1.0f - resetT_) * transitionStartPosition_.y + resetT_ * transitionEndPosition_.y;
	transitionSpritePosition_.z = (1.0f - resetT_) * transitionStartPosition_.z + resetT_ * transitionEndPosition_.z;
}