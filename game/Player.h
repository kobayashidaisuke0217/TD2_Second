#pragma once
#include "MapManager.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"
#include "Mymath.h"
#include <memory>

class Player {
public:

	enum class Phase {
		Idle,
		Move,
		Break,
	};

	void Initialize();

	
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void OnCollision(OBB& partner);
	bool OnCollisionFloorVertical(OBB& partner);
	void OnCollisionFloorHorizon(OBB& partner);
	void OnCollisionWall(OBB& partner);
	void OnCollisionEnemy();

	OBB& GetOBB() { return obb_; };
	OBB& GetFloatTrigger() { return obbFloatTrigger_; };

	void SetJoyState(XINPUT_STATE* joystate) { joyState_ = joystate; };
	void SetPreJoyState(XINPUT_STATE* joystate) { preJoyState_ = joystate; };

	void ApplyGlobalVariables();
	WorldTransform& GetWorldTransform() { return worldTransform_; };
	//死んだときに位置をリセットする用
	void Reset();
private:
	Input* input_ = nullptr;
	XINPUT_STATE* joyState_;
	XINPUT_STATE* preJoyState_;
	std::unique_ptr<Model> model_;
	
	WorldTransform worldTransform_;

	OBB obb_;
	OBB obbFloatTrigger_;

	float direction_;
	Vector3 velocity_;
	Vector3 acceleration_;
	//Vector3 gravity_;
	Vector3 prePosition_;

	bool isCollision_;
	bool isCollisionFloor_;
	bool isCollisionWall_;

	bool jumpAble_;

	//長押し入力が途切れるまで
	bool isJumpReception_;
	int32_t jumpReceptionRest_;

	//パラメータ群
	Vector3 gravity_;
	Vector3 jumpAccerelation_;
	float moveSpeed_;
	float jumpDampingX_;
	int32_t jumpReceptionLength_;
};
