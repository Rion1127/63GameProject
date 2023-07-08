#include "ParticleHitAttack.h"
#include "RRandom.h"
#include "Util.h"
ParticleHitAttack::ParticleHitAttack() :
	vertexCount(32)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("StarParticle");
}

void ParticleHitAttack::Add(int32_t addNum, int32_t time, Vector3 pos, Vector3 addVec, float scale)
{
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

		p.position = pos;
		p.basePos = pos;
		p.end_frame = time;
		p.velocity = vec;
		p.scale = scale;
		p.baseScale = scale;
		p.addRot = addrot;
		p.color = { 200,200,0,255 };
	}
}

void ParticleHitAttack::MoveUpdate()
{
	for (auto& p : particles_)
	{
		p.frame++;

		float f = (float)p.frame / p.end_frame;

		p.position += p.velocity;
		p.rot += p.addRot;

		MoveTo({ 0,0,0 }, 0.005f, p.velocity);
		MoveTo({ 0,0,0 }, 0.003f, p.addRot);
	}
}
