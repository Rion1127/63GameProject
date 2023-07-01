#pragma once
#include "mInput.h"
#include "Object3d.h"
#include "Vector3.h"
#include <memory>
#include "Texture.h"
#include "Collision.h"
#include "PlayerAttack.h"
#include "PlayerInfo.h"
#include "Gravity.h"
#include "AttackManager.h"



class Player
{
private:
	Vector3 addVec_;
	Vector3 rot_;
	Vector3 scale_;

	Sphere colPos_;
	bool isFloorCollision_ = false;
	Gravity gravity_;
	//ベクトル
	Vector3 move_;
	Vector3 frontVec_;
	
	//移動制限
	Vector2 MaxMinX_;
	Vector2 MaxMinY_;

	// --入力-- //
	float inputAngle_;// -> 入力されている方向の角度

	float moveSpeed_;

	bool isJump_;
	int jumpTime_;

	AttackManager attack_;
	PlayerInfo info_;
	PlayerState state_;
private:
	Controller* controller_ = nullptr;
	//モデル
	std::unique_ptr<Object3d> model_;
public:
	Player();
	void PreUpdate();
	void PostUpdate();

	void ColPosUpdate();
private:
	//移動
	void InputVecUpdate();
	//重力
	void GravityUpdate();
	//ジャンプ
	void JumpUpdate();
	//ステータス更新
	void StateUpdate();
public:
	void Draw();

	void DrawImGui();

	void floorColision();
public:
	void SetPos(Vector3 pos) { model_->GetTransform()->SetPosition(pos); }
	void SetRot(Vector3 rot) { model_->GetTransform()->SetRotation(rot); }
	void Setscale(Vector3 scale) { model_->GetTransform()->SetScale(scale); }

	void AddPos(Vector3 pos) { addVec_ += pos; }
	void SetAddPos(Vector3 pos) { addVec_ = pos; }
public:
	Sphere GetCol() { return colPos_; }
	WorldTransform* GetWorldTransform() { return model_->GetTransform(); }
	Vector3 GetFrontVec() { return frontVec_; }
	PlayerInfo* GetPlayerInfo() { return &info_; }
};

