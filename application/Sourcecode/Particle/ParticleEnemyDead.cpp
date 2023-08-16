#include "ParticleEnemyDead.h"
#include "ParticleManager.h"
#include "RRandom.h"
#include "Easing.h"
#include "Util.h"

#pragma region ���p�[�e�B�N��
ParticleEnemyDead::ParticleEnemyDead() :
	vertexCount(64), IParticle("Smoke")
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("Smoke");
	isBillBoard = true;
}

void ParticleEnemyDead::Add()
{
	transform_.position_ = emitter_->pos;
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

		Vector3 vec = {
			RRandom::RandF(-emitter_->addVec.x,emitter_->addVec.x),
			RRandom::RandF(-emitter_->addVec.y,emitter_->addVec.y),
			RRandom::RandF(-emitter_->addVec.z,emitter_->addVec.z)
		};
		Vector3 addrot = {
			RRandom::RandF(-emitter_->addVec.x,emitter_->addVec.x),
			RRandom::RandF(-emitter_->addVec.y,emitter_->addVec.y),
			RRandom::RandF(-emitter_->addVec.z,emitter_->addVec.z)
		};
		float scale_ = RRandom::RandF(0.3f, emitter_->scale);

		p.position = vec;
		p.basePos = emitter_->pos;
		p.end_frame = emitter_->time;
		p.scale = emitter_->scale;
		p.baseScale = emitter_->scale;
		p.color = { 5,5,35,0 };
	}
	std::shared_ptr<OneceEmitter> hitEmitter_ = std::make_shared<OneceEmitter>();
	hitEmitter_->particle = std::make_unique<ParticleHeart>();
	hitEmitter_->addNum = 1;
	hitEmitter_->time = 80;
	hitEmitter_->pos = emitter_->pos;
	hitEmitter_->addVec = { 0.f,0.05f, 0.0f };
	hitEmitter_->scale = 1.0f;
	ParticleManager::GetInstance()->
		AddParticle("Heart", hitEmitter_);
}

void ParticleEnemyDead::MoveUpdate()
{
	for (auto& p : particles_)
	{
		p.frame++;

		float f = (float)p.frame / p.end_frame;

		p.position += { 0,0.01f,0 };

		if(f < 0.5f)p.color.a = f * 355.f;
		else p.color.a = (1 - f) * 355.f;

		p.scale = Easing::Circ::easeIn(f, p.baseScale, -p.baseScale, 1.0f);

		MoveTo({ 0,0,0 }, 0.001f, p.velocity);
		MoveTo({ 0,0,0 }, 0.003f, p.addRot);
	}
}
#pragma endregion

#pragma region �n�[�g�p�[�e�B�N��
ParticleHeart::ParticleHeart() :
	IParticle("Particle_Depth_False"),
	vertexCount(1)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("Heart");
	isBillBoard = true;
}

void ParticleHeart::Add()
{
	transform_.position_ = emitter_->pos;
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

		p.veloAdd = emitter_->addVec;
		p.basePos = emitter_->pos;
		p.end_frame = emitter_->time;
		p.scale = emitter_->scale;
		p.baseScale = emitter_->scale;
		p.color = { 455,455,455,255 };
	}
}

void ParticleHeart::MoveUpdate()
{
	for (auto& p : particles_)
	{
		p.frame++;

		float f = (float)p.frame / p.end_frame;

		p.position += p.veloAdd;

		p.veloAdd += {0.f, 0.001f, 0.f};

		Vector3 col(p.color.r, p.color.g, p.color.b);

		MoveTo({ 255,255,255 }, 10.f, col);

		p.color = { col.x,col.y,col.z,255 };

		p.color.a = (1 - f) * 255.f;
	}
}
#pragma endregion
