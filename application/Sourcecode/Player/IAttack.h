#pragma once
#include <vector>
#include "Object3d.h"
#include "Collision.h"
#include "mInput.h"
#include "PlayerInfo.h"
#include "IActor.h"
#include "Spline.h"

class IAttack
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
	std::vector<std::unique_ptr<AttackCol>> attackCol_;
	AttackInfo attackInfo_;
	IActor* selfActor_;
	IActor* lockOnActor_;
	Vector3 attackVec_;
	Spline spline_;
public:
	IAttack(IActor* selfActor,int32_t colNum = 1, int32_t maxTime = 20, int32_t damage = 10,int32_t damageCoolTime = 25);
	virtual ~IAttack(){};
	void Update();
	void DrawCol();
	Vector3 CalculateFrontVec();
	void CalculateRotToLockOnActor(Vector3 frontVec);
public:
	void SetNowTime(int32_t time) { attackInfo_.nowTime = time; }
	void SetLockOnActor(IActor* lockOnActor) { lockOnActor_ = lockOnActor; };
public:
	AttackInfo GetInfo() { return attackInfo_; }
	std::vector<std::unique_ptr<AttackCol>>* GetAttackCol() { return &attackCol_; }
	IActor* GetSelfActor() { return selfActor_; }
public:
	virtual void Init() = 0;
protected:
	virtual void MoveUpdate() = 0;
};

