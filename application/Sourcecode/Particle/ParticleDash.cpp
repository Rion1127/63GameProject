#include "ParticleDash.h"
#include "Easing.h"
#include "RRandom.h"
#include "Util.h"

ParticleDash::ParticleDash() :
	IParticle("Fire"),
	vertexCount(6)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("MoveSmoke");
	isBillBoard = true;
	state_ = PipeLineState::Alpha;

	speed_ = 0.1f;
}

void ParticleDash::Add()
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
		landingParticles_.emplace_back();
		//追加した要素の参照
		auto& baseP = particles_.back();
		auto& p = landingParticles_.back();

		p.position = emitter_->pos;
		p.basePos = emitter_->pos;
		Vector3 addvec = {
			RRandom::RandF(-1,1),
			0,
			RRandom::RandF(-1,1),
		};
		addvec = addvec.normalize();
		p.addVec = emitter_->addVec * speed_;
		p.end_frame = emitter_->time;
		p.scale = emitter_->scale;
		p.baseScale = emitter_->scale;
		p.addRot.z = RRandom::RandF(-0.3f, 0.3f);
		p.color = { 255,255,255,255 };

		baseP = p;
	}
}

void ParticleDash::MoveUpdate()
{
	for (int32_t i = 0; i < landingParticles_.size(); i++)
	{
		landingParticles_[i].rate = (float)landingParticles_[i].frame / (float)landingParticles_[i].end_frame;

		if (landingParticles_[i].frame >= landingParticles_[i].end_frame)
		{
			landingParticles_.erase(landingParticles_.begin() + i);
			vertices_.at(i).scale = 0;
			i = -1;
		}
	}

	uint32_t i = 0;
	for (auto& p : landingParticles_)
	{
		p.frame += 1.f * GameSpeed::GetGameSpeed();

		float f = (float)p.frame / p.end_frame;

		f;

		p.scale = Easing::Quint::easeIn(p.baseScale, 0.f, f);

		p.position += p.addVec;
		MoveTo({ 0,0,0 }, 0.003f, p.addVec);

		p.rot += p.addRot;
		MoveTo({ 0,0,0 }, 0.03f, p.addRot);

		particles_[i] = p;
		i++;
	}
}
