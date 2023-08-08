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

void ParticleEnemyDead::Add(int32_t addNum, int32_t time, Vector3 pos, Vector3 addVec, float scale)
{
	transform_.position_ = pos;
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

		Vector3 dist = pos - (pos + vec);

		p.position = vec;
		p.basePos = pos;
		p.end_frame = time;
		p.scale = scale;
		p.baseScale = scale;
		p.color = { 5,5,35,0 };
	}
	std::shared_ptr<Emitter> hitEmitter_ = std::make_shared<Emitter>();
	hitEmitter_->particle = std::make_unique<ParticleHeart>();
	hitEmitter_->addNum = 1;
	hitEmitter_->time = 80;
	hitEmitter_->pos = pos;
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
	vertexCount(10)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("Heart");
	isBillBoard = true;
}

void ParticleHeart::Add(int32_t addNum, int32_t time, Vector3 pos, Vector3 addVec, float scale)
{
	transform_.position_ = pos;
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

		p.veloAdd = addVec;
		p.basePos = pos;
		p.end_frame = time;
		p.scale = scale;
		p.baseScale = scale;
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
