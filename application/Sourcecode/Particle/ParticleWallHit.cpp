#include "ParticleWallHit.h"
#include "Easing.h"

ParticleWallHit::ParticleWallHit() : 
	IParticle(),
	vertexCount(3)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("Circle");
	isBillBoard = false;
	tiemr_.SetLimitTime(20);
	addMaxNum_ = 3;
}

void ParticleWallHit::Add(int32_t addNum, int32_t time, Vector3 pos, Vector3 addVec, float scale)
{
	tiemr_.AddTime(1);
	if (tiemr_.GetIsEnd()) {
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

			p.position = pos;
			p.basePos = pos;
			p.end_frame = time;
			p.rot = addVec;
			p.scale = scale;
			p.baseScale = scale;
			p.color = { 255,255,255,0 };
		}
		//�R��Z�b�g�Ńp�[�e�B�N�����o��悤�ɂ���
		addNum_++;
		if (addNum_ == addMaxNum_) {
			tiemr_.SetLimitTime(80);
			addNum_ = 0;
		}
		else {
			tiemr_.SetLimitTime(20);
		}
		tiemr_.Reset();
	}
}

void ParticleWallHit::MoveUpdate()
{
	for (auto& p : particles_)
	{
		p.frame++;

		float f = (float)p.frame / p.end_frame;

		p.color.a = (1 - f) * 255.f;

		p.scale = Easing::Circ::easeOut(f, p.baseScale, 5.f, 1.0f);

	}
}
