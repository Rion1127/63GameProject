#pragma once
#include "mInput.h"
#include "WorldTransform.h"
#include "Object3d.h"
#include "Vector3.h"
#include <memory>
#include "Texture.h"
#include "Collision.h"
//#include "PlayerAttack.h"
class Player
{
public:
	Player();
	void Update();

	void ColPosUpdate();
private:
	//移動
	void InputVecUpdate();
	void InputAngleUpdate();
	//重力
	void GravityUpdate();
	//ジャンプ
	void JumpUpdate();
	bool isJump_;
	int jumpTime;
public:
	void Draw();

	void DrawImGui();

	WorldTransform* GetWorldTransform() { return &worldTransform_; }
private:
	Controller* controller_ = nullptr;
	//モデル
	std::unique_ptr<Object3d> model_;
	WorldTransform worldTransform_;
public:

	void floorColision();
	Sphere GetColPos() { return colPos_; }
private:
	Sphere colPos_;
	bool isFloorCollision = false;
	float gravity_;

	//ベクトル
	Vector3 move;
	Vector2 moveVec2;
	Vector3 resultVec;
	Vector2 resultVec2;
	Vector3 sideVec;
	Vector3 upVec = { 0,1,0 };
	//移動制限
	Vector2 MaxMinX;
	Vector2 MaxMinY;

	// --入力-- //
	Vector2 inputVec_;// -> 入力されているベクトル
	float inputAngle_;// -> 入力されている方向の角度
	bool isLeftStick_;// -> 左スティックが倒されているか
	bool isOldLeftStick_;// -> 1フレーム前の左スティックが倒されているか

	float moveSpeed_;

	//PlayerAttack attack_;

};

