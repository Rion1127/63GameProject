#include "ParticleFire.h"
#include "RRandom.h"
#include "Easing.h"
#include "Util.h"

#pragma region 炎エフェクト
ParticleFire::ParticleFire() :
	IParticle("Fire"),
	vertexCount(192)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("Particle");
	isBillBoard = true;
	state_ = PipeLineState::Add;
}

void ParticleFire::Add()
{
	//transform_.position_ = pos;
	for (int i = 0; i < emitter_->addNum; i++)
	{
		//指定した最大頂点数超えてたら生成しない
		if (particles_.size() >= vertexCount)
		{
			return;
		}
		//リストに要素を追加
		particles_.emplace_back();
		fireParticles_.emplace_back();
		//追加した要素の参照
		auto& baseP = particles_.back();
		auto& p = fireParticles_.back();

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
		
		p.position = emitter_->pos + vec;
		p.basePos = emitter_->pos;
		p.end_frame = emitter_->time;
		p.scale = emitter_->scale;
		p.baseScale = emitter_->scale;
		p.addRot.z = RRandom::RandF(-0.01f, 0.01f);
		p.color = { 255,100,0,255 };

		baseP = p;
	}
}

void ParticleFire::MoveUpdate()
{
	for (int32_t i = 0; i < fireParticles_.size(); i++)
	{
		fireParticles_[i].rate = (float)fireParticles_[i].frame / (float)fireParticles_[i].end_frame;

		if (fireParticles_[i].frame >= fireParticles_[i].end_frame)
		{
			fireParticles_.erase(fireParticles_.begin() + i);
			vertices_.at(i).scale = 0;
			i = -1;
		}
	}

	uint32_t index = 0;
	for (auto& p : fireParticles_)
	{
		p.frame++;

		float f = (float)p.frame / p.end_frame;

		p.position += { 0, 0.02f, 0 };

		if (f < 0.5f)p.color.a = f * 355.f;
		else p.color.a = (1 - f) * 355.f;

		p.scale -= 0.01f;
		p.scale = Max(0.f, p.scale);

		p.rot.z += p.addRot.z;

		particles_[index] = p;
		index++;
	}
}
#pragma endregion

#pragma region 炎円エフェクト
ParticleFireCircle::ParticleFireCircle() :
	IParticle("Fire"),
	vertexCount(2)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("FireCircle");
	isBillBoard = false;
	state_ = PipeLineState::Add;
}

void ParticleFireCircle::Add()
{
	parentPos_ = emitter_->parentPos;
	for (int i = 0; i < emitter_->addNum; i++)
	{
		//指定した最大頂点数超えてたら生成しない
		if (particles_.size() >= vertexCount)
		{
			return;
		}
		//リストに要素を追加
		particles_.emplace_back();
		fireCircleParticles_.emplace_back();
		//追加した要素の参照
		auto& baseP = particles_.back();
		auto& p = fireCircleParticles_.back();

		p.position = emitter_->pos;
		p.position.y += 0.5f * (1 + i);
		p.basePos.y = p.position.y;
		p.end_frame = emitter_->time;
		p.scale = 0;
		p.baseScale = emitter_->scale * (1.5f + i);
		p.addRot.y = RRandom::RandF(-0.03f, 0.03f);
		p.color = { 255,100,0,255 };

		baseP = p;
	}
}

void ParticleFireCircle::MoveUpdate()
{
	for (int32_t i = 0; i < fireCircleParticles_.size(); i++)
	{
		fireCircleParticles_[i].rate = (float)fireCircleParticles_[i].frame / (float)fireCircleParticles_[i].end_frame;

		if (fireCircleParticles_[i].frame >= fireCircleParticles_[i].end_frame)
		{
			fireCircleParticles_.erase(fireCircleParticles_.begin() + i);
			vertices_.at(i).scale = 0;
			i = -1;
		}
	}

	uint32_t index = 0;
	for (auto& p : fireCircleParticles_)
	{
		p.position = emitter_->pos;
		p.position.y = p.basePos.y;
		p.frame++;

		float f = (float)p.frame / p.end_frame;

		if (f > 0.7f)p.color.a -= 7.f;
		p.color.a = Max(0.f, p.color.a);

		f *= 3.f;
		f = Min(f, 1.f);
		p.scale = Easing::Circ::easeOut(0.f, p.baseScale, f);

		p.rot.x = Radian(90);
		p.rot.y += p.addRot.y;

		particles_[index] = p;
		index++;
	}
}
#pragma endregion


