#pragma once
#include "mInput.h"
#include "Vector3.h"
#include <memory>
#include "Texture.h"
#include "PlayerAttack.h"
#include "PlayerInfo.h"
#include "Gravity.h"
#include "AttackManager.h"
#include "IActor.h"


class Player :
	public IActor
{
private:
	Vector3 addVec_;
	Vector3 rot_;
	Vector3 scale_;

	bool isFloorCollision_ = false;
	Gravity gravity_;
	//�x�N�g��
	Vector3 move_;
	Vector3 frontVec_;
	
	//�ړ�����
	Vector2 MaxMinX_;
	Vector2 MaxMinY_;

	// --����-- //
	float inputAngle_;// -> ���͂���Ă�������̊p�x

	float moveSpeed_;

	bool isJump_;
	int jumpTime_;

	AttackManager attack_;
	PlayerInfo info_;
	PlayerState state_;
private:
	Controller* controller_ = nullptr;
public:
	Player();
	void PreUpdate();
	void PostUpdate();

	void ColPosUpdate();
private:
	//�ړ�
	void InputVecUpdate();
	//�d��
	void GravityUpdate();
	//�W�����v
	void JumpUpdate();
	//�X�e�[�^�X�X�V
	void StateUpdate();
public:
	void Draw();

	void DrawImGui();

	void floorColision();

	bool GetIsCanMove();
public:
	void SetPos(Vector3 pos) { obj_->GetTransform()->SetPosition(pos); }
	void SetRot(Vector3 rot) { obj_->GetTransform()->SetRotation(rot); }
	void Setscale(Vector3 scale) { obj_->GetTransform()->SetScale(scale); }

	void AddPos(Vector3 pos) { addVec_ += pos; }
	void SetAddPos(Vector3 pos) { addVec_ = pos; }
	void SetLockOnEnemy(IEnemy* enemy) { attack_.SetLockOnEnemy(enemy); }
public:
	AttackManager* GetAttackManager() { return &attack_; }
	Vector3 GetFrontVec() { return frontVec_; }
	PlayerInfo* GetPlayerInfo() { return &info_; }
};

