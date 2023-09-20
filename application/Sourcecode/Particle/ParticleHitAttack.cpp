#include "ParticleHitAttack.h"
#include "RRandom.h"
#include "Util.h"
#include "Easing.h"
ParticleHitAttack::ParticleHitAttack() :
	IParticle("Particle_Depth_False"),
	vertexCount(32)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("StarParticle");
	isBillBoard = true;
}

void ParticleHitAttack::Add()
{
	transform_.position_ = emitter_->pos;
	for (int i = 0; i < emitter_->addNum; i++)
	{
		//指定した最大頂点数超えてたら生成しない
		if (particles_.size() >= vertexCount)
		{
			return;
		}
		//リストに要素を追加
		particles_.emplace_back();
		hitAttackParticles_.emplace_back();
		//追加した要素の参照
		auto& p = hitAttackParticles_.back();

		Vector3 vec = {
			RRandom::RandF(-emitter_->addVec.x,emitter_->addVec.x),
			RRandom::RandF(0,emitter_->addVec.y * 2.f),
			RRandom::RandF(-emitter_->addVec.z,emitter_->addVec.z)
		};
		Vector3 addvec = {
			0,
			RRandom::RandF(-emitter_->addVec.y / 20.f,-emitter_->addVec.y / 70.f),
			0
		};
		Vector3 addrot = {
			0,
			0,
			0.1f + RRandom::RandF(-emitter_->addVec.z,emitter_->addVec.z)
		};
		float scale_ = RRandom::RandF(0.3f, emitter_->scale);

		p.basePos = emitter_->pos;
		p.end_frame = emitter_->time;
		p.velocity = vec;
		p.addvelocity = addvec;
		p.scale = emitter_->scale;
		p.baseScale = emitter_->scale;
		p.addRot = addrot;
		p.color = { 255,175,60,255 };
	}
}

void ParticleHitAttack::MoveUpdate()
{
	uint32_t index = 0;
	for (auto& p : hitAttackParticles_)
	{
		p.frame++;

		float f = (float)p.frame / p.end_frame;

		p.velocity += p.addvelocity;

		p.position += p.velocity;
		p.rot += p.addRot;

		p.scale = Easing::Quint::easeIn(p.baseScale,0.f, f);


		MoveTo({ 0,0,0 }, 0.001f, p.velocity);
		MoveTo({ 0,0,0 }, 0.003f, p.addRot);

		particles_[index] = p;
	}
}
