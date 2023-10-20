#include "ParticleHitAttack.h"
#include "RRandom.h"
#include "Util.h"
#include "Easing.h"
#include "ParticleManager.h"

/**
 * @file ParticleHitAttack.cpp
 * @brief 攻撃がヒットしたときのパーティクルクラス
 */

#pragma region ParticleHitAttack
ParticleHitAttack::ParticleHitAttack() :
	IParticle("Particle_Depth_False"),
	vertexCount(3)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("StarParticle");
	isBillBoard = true;
}

void ParticleHitAttack::Add()
{
	transform_.position_ = emitter_->pos;
	color_ = { 255,175,60,255 };
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
		auto& baseP = particles_.back();
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
		
		p.basePos = emitter_->pos;
		p.end_frame = emitter_->time;
		p.velocity = vec;
		p.addvelocity = addvec;
		p.scale = emitter_->scale;
		p.baseScale = emitter_->scale;
		p.addRot = addrot;
		p.color = color_;
		p.timer_.SetLimitTime(RRandom::RandF(5, 10));

		baseP = p;
	}

	std::shared_ptr<OneceEmitter> hitCirckeEmitter_ = std::make_shared<OneceEmitter>();
	hitCirckeEmitter_->particle = std::make_unique<ParticleHitCircle>();
	hitCirckeEmitter_->addNum = 1;
	hitCirckeEmitter_->time = 15;
	hitCirckeEmitter_->pos = emitter_->pos;
	hitCirckeEmitter_->scale = 0.0f;
	ParticleManager::GetInstance()->
		AddParticle("HitCircle", hitCirckeEmitter_);

	std::shared_ptr<OneceEmitter> hitTriangleEmitter_ = std::make_shared<OneceEmitter>();
	hitTriangleEmitter_->particle = std::make_unique<ParticleHitTriangle>();
	hitTriangleEmitter_->addNum = 10;
	hitTriangleEmitter_->time = 60;
	hitTriangleEmitter_->addVec = { 0.1f,0.1f, 0.1f };
	hitTriangleEmitter_->pos = emitter_->pos;
	hitTriangleEmitter_->scale = 0.2f;
	ParticleManager::GetInstance()->
		AddParticle("HitCircle", hitTriangleEmitter_);
}

void ParticleHitAttack::MoveUpdate()
{
	for (int32_t i = 0; i < hitAttackParticles_.size(); i++)
	{
		hitAttackParticles_[i].rate = (float)hitAttackParticles_[i].frame / (float)hitAttackParticles_[i].end_frame;

		if (hitAttackParticles_[i].frame >= hitAttackParticles_[i].end_frame)
		{
			hitAttackParticles_.erase(hitAttackParticles_.begin() + i);
			vertices_.at(i).scale = 0;
			i = -1;
		}
	}

	uint32_t index = 0;
	for (auto& p : hitAttackParticles_)
	{
		p.frame += 1.f * GameSpeed::GetGameSpeed();
		p.timer_.AddTime(1.f * GameSpeed::GetGameSpeed());

		float f = (float)p.frame / p.end_frame;

		p.velocity += p.addvelocity * GameSpeed::GetGameSpeed();

		p.position += p.velocity;
		p.rot += p.addRot * GameSpeed::GetGameSpeed();

		p.scale = Easing::Quint::easeIn(p.baseScale, 0.f, f);

		MoveTo({ 0,0,0 }, 0.001f, p.velocity);
		MoveTo({ 0,0,0 }, 0.003f, p.addRot);

		if (p.timer_.GetIsEnd()) {
			p.timer_.Reset();

			Color color = color_ * 2.f;

			p.color = (p.color.a == color_.a) ? color : color_;
		}


		particles_[index] = p;

		index++;
	}
}
#pragma endregion

#pragma region ParticleHitCircle

ParticleHitCircle::ParticleHitCircle() :
	IParticle("Particle_Depth_False"),
	vertexCount(1)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("HitCircle");
	isBillBoard = true;
}

void ParticleHitCircle::Add()
{
	transform_.position_ = emitter_->pos;

	endScale_ = 4.f;

	for (int i = 0; i < emitter_->addNum; i++)
	{
		//指定した最大頂点数超えてたら生成しない
		if (particles_.size() >= vertexCount)
		{
			return;
		}
		//リストに要素を追加
		particles_.emplace_back();
		hitCircleParticles_.emplace_back();
		//追加した要素の参照
		auto& baseP = particles_.back();
		auto& p = hitCircleParticles_.back();

		Vector3 addrot = {
			0,
			0,
			0.2f
		};

		//p.position = emitter_->pos;
		p.end_frame = emitter_->time;
		p.scale = emitter_->scale;
		p.baseScale = emitter_->scale;
		p.endScale = endScale_;
		p.addRot = addrot;
		p.color = { 255,255,255,255 };

		baseP = p;
	}
}

void ParticleHitCircle::MoveUpdate()
{
	for (int32_t i = 0; i < hitCircleParticles_.size(); i++)
	{
		hitCircleParticles_[i].rate = (float)hitCircleParticles_[i].frame / (float)hitCircleParticles_[i].end_frame;

		if (hitCircleParticles_[i].frame >= hitCircleParticles_[i].end_frame)
		{
			hitCircleParticles_.erase(hitCircleParticles_.begin() + i);
			vertices_.at(i).scale = 0;
			i = -1;
		}
	}

	uint32_t index = 0;
	for (auto& p : hitCircleParticles_)
	{
		p.frame += 1.f * GameSpeed::GetGameSpeed();
		float f = (float)p.frame / p.end_frame;

		p.rot += p.addRot * GameSpeed::GetGameSpeed();

		p.scale = Easing::Quint::easeOut(p.baseScale, p.endScale, f);
		p.color.a = Easing::Quint::easeIn(255, 0, f);

		MoveTo({ 0,0,0 }, 0.008f * GameSpeed::GetGameSpeed(), p.addRot);

		particles_[index] = p;
		index++;
	}
}
#pragma endregion

#pragma region ParticleHitTriangle
ParticleHitTriangle::ParticleHitTriangle() :
	IParticle("Particle_Depth_False"),
	vertexCount(10)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("Triangle");
	isBillBoard = false;
}

void ParticleHitTriangle::Add()
{
	endScale_ = 0.f;
	transform_.position_ = emitter_->pos;
	color_ = { 255,175,60,255 };
	for (int i = 0; i < emitter_->addNum; i++)
	{
		//指定した最大頂点数超えてたら生成しない
		if (particles_.size() >= vertexCount)
		{
			return;
		}
		//リストに要素を追加
		particles_.emplace_back();
		hitTriangleParticles_.emplace_back();
		//追加した要素の参照
		auto& baseP = particles_.back();
		auto& p = hitTriangleParticles_.back();

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
			RRandom::RandF(-emitter_->addVec.x,emitter_->addVec.x),
			RRandom::RandF(-emitter_->addVec.y,emitter_->addVec.y),
			RRandom::RandF(-emitter_->addVec.z,emitter_->addVec.z)
		};
		float scale = RRandom::RandF(emitter_->scale / 2.f, emitter_->scale);

		p.basePos = emitter_->pos;
		p.end_frame = emitter_->time;
		p.velocity = vec;
		p.addvelocity = addvec;
		p.scale = scale;
		p.baseScale = scale;
		p.addRot = addrot * 3.f;
		p.color = color_;
		p.timer_.SetLimitTime(RRandom::RandF(5, 10));

		baseP = p;
	}
}

void ParticleHitTriangle::MoveUpdate()
{
	for (int32_t i = 0; i < hitTriangleParticles_.size(); i++)
	{
		hitTriangleParticles_[i].rate = (float)hitTriangleParticles_[i].frame / (float)hitTriangleParticles_[i].end_frame;

		if (hitTriangleParticles_[i].frame >= hitTriangleParticles_[i].end_frame)
		{
			hitTriangleParticles_.erase(hitTriangleParticles_.begin() + i);
			vertices_.at(i).scale = 0;
			i = -1;
		}
	}

	uint32_t index = 0;
	for (auto& p : hitTriangleParticles_)
	{
		p.frame += 1 * GameSpeed::GetGameSpeed();
		p.timer_.AddTime(1.f * GameSpeed::GetGameSpeed());

		float f = (float)p.frame / p.end_frame;

		p.velocity += p.addvelocity * GameSpeed::GetGameSpeed();

		p.position += p.velocity * GameSpeed::GetGameSpeed();
		p.rot += p.addRot * GameSpeed::GetGameSpeed();

		p.scale = Easing::Quint::easeIn(p.baseScale, 0.f, f);

		MoveTo({ 0,0,0 }, 0.003f * GameSpeed::GetGameSpeed(), p.addRot);

		if (p.timer_.GetIsEnd()) {
			p.timer_.Reset();

			Color color = color_ * 2.f;

			p.color = (p.color.a == color_.a) ? color : color_;
		}


		particles_[index] = p;

		index++;
	}
}
#pragma endregion