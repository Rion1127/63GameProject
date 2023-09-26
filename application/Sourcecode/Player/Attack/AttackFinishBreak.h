#pragma once
#include "IAttack.h"
class AttackFinishBreak :
    public IAttack
{
private:
	float colRadius_;
	float splineTime_;
	int32_t hitNum_;
	//�����蔻�肪��]����X�s�[�h
	float rotateSpeed_;
	bool isParticleAdd_;
	//�G�~�b�^�[
	std::array<std::shared_ptr<OneceEmitter>,3> emitter_;
public:
	AttackFinishBreak(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;

	void SplineUpdate();
};

