#pragma once
#include "IEnemy.h"
#include <optional>
class PlayerAimBallEnemy:public IEnemy
{
	public:
		PlayerAimBallEnemy();
		~PlayerAimBallEnemy();
		virtual void Initialize(const Transform& transform, const Vector3& velocity, float moveSpeed, uint32_t texture) override;
		virtual void Update() override;
		virtual void Draw(const ViewProjection& viewProjection) override;
		virtual void isCollision(OBB pertner) override;
		virtual bool GetIsAlive()override { return isAlive_; }
		virtual OBB& GetOBB()override { return obb_; };



		virtual bool GetIsHit()override { return ishit_; }
		virtual void SetPartener(collisionPartner partner)override { collisionpartner_ = partner; }
private:
	enum class Behavior {
		kstandBy,
		kAtack,
		kLeave,
	};
	
		int standBycount;
		int BehaviorChangeCount;


	std::optional<Behavior> behaviorRequest_;
	Behavior behavior_ = Behavior::kstandBy;
	void BehaviorStandbyUpdate();
	void BehaviorAtackUpdate();
	void BehaviorLeaveUpdate();
	void BehaviorStandbyInitialize();
	void BehaviorLeaveInitialize();
	void BehaviorAtackInitialize();
};
