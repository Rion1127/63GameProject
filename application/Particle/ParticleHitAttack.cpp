#include "ParticleHitAttack.h"
#include "RRandom.h"
#include "Util.h"
ParticleHitAttack::ParticleHitAttack() :
	vertexCount(16)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("uv");
}

void ParticleHitAttack::Add(size_t addNum, size_t time, Vector3 pos, Vector3 addVec, float scale)
{
	for (int i = 0; i < addNum; i++)
	{
		//�w�肵���ő咸�_�������Ă��琶�����Ȃ�
		if (particles_.size() >= vertexCount)
		{
			return;
		}
		//���X�g�ɗv�f��ǉ�
		particles_.emplace_back();
		//�ǉ������v�f�̎Q��
		Particle& p = particles_.back();
		
		Vector3 vec = {
			RRandom::RandF(-addVec.x,addVec.x),
			RRandom::RandF(-addVec.y,addVec.y),
			RRandom::RandF(-addVec.z,addVec.z)
		};
		Vector3 addrot = {
			RRandom::RandF(-addVec.x,addVec.x),
			RRandom::RandF(-addVec.y,addVec.y),
			RRandom::RandF(-addVec.z,addVec.z)
		};
		float scale_ = RRandom::RandF(0.3f, scale);

		p.position = pos;
		p.basePos = pos;
		p.end_frame = time;
		p.velocity = vec;
		p.scale = scale;
		p.baseScale = scale;
		p.addRot = addrot;
	}
}

void ParticleHitAttack::MoveUpdate()
{
	for (auto& p : particles_)
	{
		p.frame++;

		float f = (float)p.frame / p.end_frame;

		p.position += p.velocity;
		p.rot += p.addRot;

		MoveTo({ 0,0,0 }, 0.005f, p.velocity);
		MoveTo({ 0,0,0 }, 0.003f, p.addRot);
	}
}