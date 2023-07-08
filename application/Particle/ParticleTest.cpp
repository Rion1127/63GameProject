#include "ParticleTest.h"
#include "Easing.h"
#include "RRandom.h"
#include "Util.h"
ParticleTest::ParticleTest() :
	vertexCount(16)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("StarParticle");
}

void ParticleTest::Add(int32_t addNum, int32_t time, Vector3 pos, Vector3 addVec, float scale)
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

		p.position = pos;
		p.basePos = pos;
		p.end_frame = time;
		p.velocity = vec;
		p.scale = scale;
		p.baseScale = scale;
	}
}

void ParticleTest::MoveUpdate()
{
	for (auto& p : particles_)
	{
		p.frame++;

		float f = (float)p.frame / p.end_frame;

		p.position += p.velocity;
		
		MoveTo({ 0,0,0 }, 0.01f, p.velocity);
	}
}
