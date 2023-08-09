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

void ParticleWallHit::Add(int32_t addNum, int32_t time,
	Vector3 pos, Vector3 addVec,
	float scale, Vector3* parentPos)
{
	transform_.position_ = pos;
	for (int i = 0; i < addNum; i++)
	{
		//指定した最大頂点数超えてたら生成しない
		if (particles_.size() >= vertexCount)
		{
			return;
		}
		//リストに要素を追加
		particles_.emplace_back();
		//追加した要素の参照
		Particle& p = particles_.back();

		p.basePos = pos;
		p.end_frame = time;
		p.rot = addVec;
		p.scale = scale;
		p.baseScale = scale;
		p.color = { 255,255,255,0 };
	}
}

void ParticleWallHit::MoveUpdate()
{
	for (auto& p : particles_)
	{
		p.frame++;

		float f = (float)p.frame / p.end_frame;

		p.color.a = (1 - f) * 255.f;

		p.scale = Easing::Circ::easeOut(f, p.baseScale, 5.f, 1.0f);

	}
}
