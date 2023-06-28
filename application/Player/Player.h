#pragma once
#include "mInput.h"
#include "Object3d.h"
#include "Vector3.h"
#include <memory>
#include "Texture.h"
#include "Collision.h"
#include "PlayerAttack.h"
class Player
{
private:
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;


	Sphere colPos_;
	bool isFloorCollision = false;
	float gravity_;

	//�x�N�g��
	Vector3 move;
	Vector2 moveVec2;
	Vector3 resultVec;
	Vector2 resultVec2;
	Vector3 sideVec;
	Vector3 upVec = { 0,1,0 };
	//�ړ�����
	Vector2 MaxMinX;
	Vector2 MaxMinY;

	// --����-- //
	Vector2 inputVec_;// -> ���͂���Ă���x�N�g��
	float inputAngle_;// -> ���͂���Ă�������̊p�x
	bool isLeftStick_;// -> ���X�e�B�b�N���|����Ă��邩
	bool isOldLeftStick_;// -> 1�t���[���O�̍��X�e�B�b�N���|����Ă��邩

	float moveSpeed_;

	bool isJump_;
	int jumpTime;
private:
	Controller* controller_ = nullptr;
	//���f��
	std::unique_ptr<Object3d> model_;
public:
	Player();
	void Update();

	void ColPosUpdate();
private:
	//�ړ�
	void InputVecUpdate();
	void InputAngleUpdate();
	//�d��
	void GravityUpdate();
	//�W�����v
	void JumpUpdate();
public:
	void Draw();

	void DrawImGui();

public:

	void floorColision();
	Sphere GetColPos() { return colPos_; }
	WorldTransform* GetWorldTransform() { return model_->GetTransform(); }
	void SetPos(Vector3 pos) { pos_ = pos; }
	void SetRot(Vector3 rot) { rot_ = rot; }
	void Setscale(Vector3 scale) { scale_ = scale; }
};

