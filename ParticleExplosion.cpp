#include "ParticleExplosion.h"
#include "RRandom.h"
#include "Easing.h"
#include "ParticleExplosion.h"

ParticleExplosion::ParticleExplosion() : 
	IParticle("Fire"),
	vertexCount(1)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("Explosion_01");
	isBillBoard = true;
	state_ = PipeLineState::Alpha;

	endRate_ = 0.8f;
}

void ParticleExplosion::Add()
{
	for (int i = 0; i < emitter_->addNum; i++)
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

		p.position = emitter_->pos;
		p.basePos = emitter_->pos;
		p.end_frame = emitter_->time;
		p.scale = emitter_->scale;
		p.baseScale = emitter_->scale;
		p.addRot.z = RRandom::RandF(-0.01f, 0.01f);
		p.color = { 255,255,255,255 };
	}
}

void ParticleExplosion::MoveUpdate()
{
	for (auto& p : particles_)
	{
		p.frame++;

		float f = (float)p.frame / p.end_frame;
		p.position = emitter_->pos;

		if (f >= endRate_) {
			float rate = (1.f - f) / (1.f - endRate_);
			rate = 1.f - rate;
			p.color.a = Easing::Sine::easeIn(rate,255,-255,1.0f);
		}
	}
}
