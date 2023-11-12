#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
class FollowCamera
{
public:
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	inline void SetForcusPoint(const Vector3 forcusPoint) { forcusPoint_ = forcusPoint; };
	inline void SetTarget(WorldTransform* target) { target_ = target; };
	inline const ViewProjection& GetViewProjection() { return viewProjection_; };
	//void Reset();
	//void ApplyGlobalVariables();
private:
	// ビュープロジェクション
	ViewProjection viewProjection_;

	Vector3 forcusPoint_;
	WorldTransform* target_ = nullptr;
	//Vector3 interTargert_;
	
	Vector3 offset_ = {0,2.0f,-20.0f};
	
	//補間の強さ
	//float cameraDelay_;
};

