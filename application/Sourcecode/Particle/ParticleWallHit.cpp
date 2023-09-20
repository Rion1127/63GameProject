#include "ParticleWallHit.h"
#include "Easing.h"

ParticleWallHit::ParticleWallHit() :
	IParticle(),
	vertexCount(1)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("Circle");
	isBillBoard = false;
}

void ParticleWallHit::Add()
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
		wallHitParticles_.emplace_back();
		//追加した要素の参照
		auto& p = wallHitParticles_.back();

		p.basePos = emitter_->pos;
		p.end_frame = emitter_->time;
		p.rot = emitter_->addVec;
		p.scale = emitter_->scale;
		p.baseScale = emitter_->scale;
		p.color = { 255,255,255,0 };
	}
}

void ParticleWallHit::MoveUpdate()
{
	uint32_t index = 0;
	for (auto& p : wallHitParticles_)
	{
		p.frame++;

		float f = (float)p.frame / p.end_frame;

		p.color.a = (1 - f) * 255.f;

		p.scale = Easing::Circ::easeOut(f, p.baseScale, 5.f, 1.0f);

		particles_[index] = p;
	}
}
