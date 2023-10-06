#pragma once
#include "IAttack.h"
class AttackFinishBreak :
	public IAttack
{
private:
	float colRadius_;
	float splineTime_;
	int32_t hitNum_;
	//当たり判定が回転するスピード
	float rotateSpeed_;
	bool isParticleAdd_;
	//エミッター
	std::array<std::shared_ptr<OneceEmitter>, 3> emitter_;
public:
	AttackFinishBreak(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;

	void SplineUpdate();
};

