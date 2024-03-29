#pragma once
#include "Object3d.h"
#include "IAttack.h"
#include "Timer.h"
#include "SwordTrail.h"

/**
 * @file Sword.h
 * @brief 剣の動きを管理するクラス
 */

class Sword
{
public:
	enum class SwordState {
		Idle,
		Attack,
		Guard,
		MagicFire,
		Debug
	};
private:
	std::unique_ptr<Object3d> obj_;
	Object3d* playerObj_;
	SwordState state_;
	Vector3 localPos_;
	//Idle注にふわふわ浮くようにするタイマー
	TimerFloat floatingTimer_;

	Vector3 goalPos_;
	Vector3 nowPos_;

	float rot_;
	float distaice_;
	float floatRange_;
	float rotSpeed_;

	Vector3 trailHeadPos_;
	Vector3 trailtailPos_;

	std::vector<std::unique_ptr<Object3d>> tailObj_;
	SwordTrail::PosBuffer tailPos_;
	std::unique_ptr<SwordTrail> trail_;
public:
	Sword();
	void Update(const Vector3& swordPos = {0,0,0});
	void EditorUpdate(const Vector3& swordPos = { 0,0,0 });
	void Draw();
	void DrawImGui();
	void Reset();
private:
	void CalculateTrailPos();
public:
	void SetPos(const Vector3& pos) {
		obj_->SetPos(pos);
	}
	void SetParent(Object3d* parent) { playerObj_ = parent; }
	void SetState(const SwordState& state) {
		if (state_ != SwordState::Debug) {
			state_ = state;
		}
	}
public:
	Vector3 GetNowPos() { return nowPos_; }
};

