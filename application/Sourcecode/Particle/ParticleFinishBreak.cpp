#include "ParticleFinishBreak.h"
#include "RRandom.h"
#include "Easing.h"
#include "Util.h"

ParticleFinishBreak::ParticleFinishBreak() :
	IParticle("Fire"),
	vertexCount(6)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("FireCircle");
	billBoard = BillBoard::None;
	state_ = PipeLineState::Alpha;

	speed_ = 0.1f;
}

void ParticleFinishBreak::Add()
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

		Vector2 moveVec = {
			emitter_->addVec.x,
			emitter_->addVec.z,
		};
		moveVec = moveVec.normalize();

		float angle = Vec2Angle(moveVec);

		p.position = emitter_->pos;
		p.basePos = emitter_->pos;
		p.end_frame = emitter_->time;
		p.scale = emitter_->scale;
		p.baseScale = emitter_->scale;
		p.color = { 255,255,255,255 };
		p.rot.y = Radian(-angle);

		baseP = p;
	}
}

void ParticleFinishBreak::MoveUpdate()
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
		p.frame++;

		float f = (float)p.frame / p.end_frame;

		p.color.a = (1 - f) * 255.f;

		p.scale = Easing::Circ::easeOut(0.f, p.baseScale,f);

		particles_[i] = p;
		i++;
	}
}
