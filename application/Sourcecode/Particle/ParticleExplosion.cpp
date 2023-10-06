#include "ParticleExplosion.h"
#include "RRandom.h"
#include "Easing.h"
#include "ParticleExplosion.h"
#include "ParticleManager.h"

#pragma region ParticleExplosion
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
	rgbColor_ = 700;
	for (int i = 0; i < emitter_->addNum; i++)
	{
		//指定した最大頂点数超えてたら生成しない
		if (particles_.size() >= vertexCount)
		{
			return;
		}
		//リストに要素を追加
		particles_.emplace_back();
		explosionParticles_.emplace_back();
		//追加した要素の参照
		auto& baseP = particles_.back();
		auto& p = explosionParticles_.back();

		p.position = emitter_->pos;
		p.basePos = emitter_->pos;
		p.end_frame = emitter_->time;
		p.scale = emitter_->scale;
		p.baseScale = emitter_->scale;
		p.addRot.z = RRandom::RandF(-0.01f, 0.01f);
		p.color = { rgbColor_,rgbColor_,rgbColor_,255 };

		baseP = p;
	}

	otherEmitter_ = std::make_shared<OneceEmitter>();
	otherEmitter_->particle = std::make_unique<ParticleSmallExplosion>();
	otherEmitter_->addNum = 2;
	otherEmitter_->time = emitter_->time;
	otherEmitter_->pos = emitter_->pos;
	otherEmitter_->addVec = Vector3(0, 0, 0);
	otherEmitter_->scale = 0.5f;
	ParticleManager::GetInstance()->
		AddParticle("SmallExplosion", otherEmitter_);
}

void ParticleExplosion::MoveUpdate()
{
	for (int32_t i = 0; i < explosionParticles_.size(); i++)
	{
		explosionParticles_[i].rate = (float)explosionParticles_[i].frame / (float)explosionParticles_[i].end_frame;

		if (explosionParticles_[i].frame >= explosionParticles_[i].end_frame)
		{
			explosionParticles_.erase(explosionParticles_.begin() + i);
			vertices_.at(i).scale = 0;
			i = -1;
		}
	}

	uint32_t index = 0;
	for (auto& p : explosionParticles_)
	{
		p.frame += 1.f * GameSpeed::GetGameSpeed();

		float f = (float)p.frame / p.end_frame;
		p.position = emitter_->pos;

		p.color.r = rgbColor_;
		p.color.g = rgbColor_;
		p.color.b = rgbColor_;

		rgbColor_ -= 20.f * GameSpeed::GetGameSpeed();

		rgbColor_ = Max(rgbColor_, 255.f);


		if (f >= endRate_) {
			float rate = (1.f - f) / (1.f - endRate_);
			rate = 1.f - rate;
			p.color.a = Easing::Sine::easeIn(rate, 255, -255, 1.0f);
		}

		particles_[index] = p;
		index++;
	}
}
#pragma endregion

#pragma region ParticleSmallExplosion
ParticleSmallExplosion::ParticleSmallExplosion() :
	IParticle("Fire"),
	vertexCount(2)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("Explosion_02");
	isBillBoard = true;
	state_ = PipeLineState::Alpha;

	endRate_ = 0.8f;
}
void ParticleSmallExplosion::Add()
{
	for (int i = 0; i < emitter_->addNum; i++)
	{
		//指定した最大頂点数超えてたら生成しない
		if (particles_.size() >= vertexCount)
		{
			return;
		}
		//リストに要素を追加
		particles_.emplace_back();
		smallExplosionParticles_.emplace_back();
		//追加した要素の参照
		auto& baseP = particles_.back();
		auto& p = smallExplosionParticles_.back();

		p.position = emitter_->pos;
		p.basePos = emitter_->pos;
		p.end_frame = emitter_->time;
		p.scale = emitter_->scale;
		p.addRot.z = RRandom::RandF(-0.01f, 0.01f);
		p.color = { 255,255,255,255 };

		baseP = p;
	}
	randOffset_ = RRandom::RandF(0.f, 120.f);
}

void ParticleSmallExplosion::MoveUpdate()
{
	for (int32_t i = 0; i < smallExplosionParticles_.size(); i++)
	{
		smallExplosionParticles_[i].rate = (float)smallExplosionParticles_[i].frame / (float)smallExplosionParticles_[i].end_frame;

		if (smallExplosionParticles_[i].frame >= smallExplosionParticles_[i].end_frame)
		{
			smallExplosionParticles_.erase(smallExplosionParticles_.begin() + i);
			vertices_.at(i).scale = 0;
			i = -1;
		}
	}

	uint32_t i = 0;
	for (auto& p : smallExplosionParticles_)
	{
		p.frame += 1.f * GameSpeed::GetGameSpeed();

		float f = (float)p.frame / p.end_frame;

		float theta = randOffset_ + (p.frame / 5.f) + (i * Radian(180.f));
		Vector3 offsetPos = {
			cosf(theta),
			sinf(theta),
			sinf(theta),
		};

		p.position = emitter_->pos + offsetPos;

		if (f >= endRate_) {
			float rate = (1.f - f) / (1.f - endRate_);
			rate = 1.f - rate;
			p.color.a = Easing::Sine::easeIn(rate, 255, -255, 1.0f);
		}

		particles_[i] = p;
		i++;
	}
}

#pragma endregion