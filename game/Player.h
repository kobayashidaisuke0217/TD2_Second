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
	OBB& GetOBB() { return obb_; };
private:
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	
	WorldTransform worldTransform_;

	OBB obb_;

	float direction_;
	Vector3 velocity_;
	Vector3 acceleration_;
	Vector3 gravity_;
};
