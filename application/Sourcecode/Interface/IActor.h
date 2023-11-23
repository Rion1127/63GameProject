#pragma once
#include "IParticle.h"
#include "Object3d.h"
#include "Collision.h"
#include "Gravity.h"
#include "Timer.h"
#include "HitStop.h"

/**
 * @file IActor.h
 * @brief インターフェースクラス
 * @brief プレイヤーや敵・モブ等を作る際に継承するクラス
 */

enum class ActorType {
	Player,
	Enemy
};

class IActor : public HitStop
{
protected:
	ActorType type_;
	//モデル
	std::unique_ptr<Object3d> obj_;
	std::unique_ptr<Object3d> displayObj_;	//描画に使用しているオブジェ
	Sphere col_;
	Vector3 addVec_;
	Gravity gravity_;
	Vector3 knockVec_;
	Vector3 knockResist_;
	TimerFloat damageCoolTime_;
	Timer freezeTimer_;
	
	bool isFloorCollision_ = false;
	float knockDecreaseValue;
	bool isCanMove_ = true;
	//軸の角度
	float objAngleX_;
	float objAngleY_;
	float objAngleZ_;
	Quaternion axisX_;				//プレイヤーX軸回転
	Quaternion axisY_;				//プレイヤーY軸回転
	Quaternion axisZ_;				//プレイヤーZ軸回転
protected:
	void ObjUpdate();
public:
	IActor(ActorType type);
	Sphere GetCol() { return col_; }
	WorldTransform* GetWorldTransform() { return obj_->GetTransform(); }
	WorldTransform* GetDisplayWorldTransform() { return displayObj_->GetTransform(); }
	Vector3 GetAddVec() {
		return addVec_;
	}
	Gravity* GetGravity() { return &gravity_; }
	float GetObjAngleX() { return objAngleX_; }
	float GetObjAngleY() { return objAngleY_; }
	float GetObjAngleZ() { return objAngleZ_; }

	Quaternion GetAxisY() {
		return axisY_;
	}
public:
	void AddaddVec(const Vector3& vec) { addVec_ += vec; }
	void SetaddVec(const Vector3& vec) { addVec_ = vec; }
	void SetGravity(const Vector3& vec) { gravity_.SetGrabity(vec); }
	virtual void SetKnockVec(const Vector3& vec) { knockVec_ = vec; }
	void SetIsCanMove(bool flag) { isCanMove_ = flag; }
	void SetFreezeTime(uint32_t time) { freezeTimer_.SetLimitTime(time); freezeTimer_.Reset(); }
	void SetObjAngleX(float angle) { objAngleX_ = angle; }
	void SetObjAngleY(float angle) { objAngleY_ = angle; }
	void SetObjAngleZ(float angle) { objAngleZ_ = angle; }
	void SetAxisY(Quaternion q) { axisY_ = q; }
	void SetIsFloorCollision(bool flag) { isFloorCollision_ = flag; }
};

