#pragma once
#include <vector>
#include "Object3d.h"
#include "Collision.h"
#include "mInput.h"
#include "PlayerInfo.h"
#include "IActor.h"
#include "Spline.h"

class IBullet
{
private:
	struct AttackCol {
		Object3d colObj_;
		Sphere col_;
		int32_t damage = 10;
		//�G�̍U�������蔻��L���܂ł̎���
		int32_t damageCoolTime = 20;
		//�m�b�N�o�b�N�̋���
		Vector3 knockPower = { 0.5f,0.5f,0.5f };
		//������̃m�b�N�o�b�N
		float knockVecY = 0.5f;
	};
	struct AttackInfo {
		int32_t maxTime;	//�U���̃t���[����
		int32_t nowTime;
	};
protected:
	std::unique_ptr<AttackCol> attackCol_;
	AttackInfo attackInfo_;
	IActor* selfActor_;
	IActor* lockOnActor_;
	Vector3 attackVec_;
	Spline spline_;

	Timer aliveTimer_;
	bool isDead_;
public:
	IBullet(IActor* selfActor, int32_t colNum = 1, int32_t maxTime = 20, int32_t damage = 10, int32_t damageCoolTime = 25);
	virtual ~IBullet() {};
	void Update();
	void DrawCol();
	Vector3 CalculateFrontVec();
	void CalculateRotToLockOnActor(Vector3 frontVec);
public:
	void SetNowTime(int32_t time) { attackInfo_.nowTime = time; }
	void SetLockOnActor(IActor* lockOnActor) { lockOnActor_ = lockOnActor; };
	void SetIsDead(bool flag) { isDead_ = flag; }
public:
	AttackInfo GetInfo() { return attackInfo_; }
	std::unique_ptr<AttackCol>* GetAttackCol() { return &attackCol_; }
	IActor* GetSelfActor() { return selfActor_; }
	bool GetIsDead() { return isDead_; }
public:
	virtual void Init() = 0;
protected:
	virtual void MoveUpdate() = 0;
};
