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

	OBB& GetOBB() { return obb_; };
	OBB& GetFloatTrigger() { return obbFloatTrigger_; };

	void SetJoyState(XINPUT_STATE* joystate) { joyState_ = joystate; };
	void SetPreJoyState(XINPUT_STATE* joystate) { preJoyState_ = joystate; };

	void ApplyGlobalVariables();
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

	//パラメータ群
	Vector3 gravity_;
	Vector3 jumpAccerelation_;
	float moveSpeed_;
	float jumpDampingX_;
};
