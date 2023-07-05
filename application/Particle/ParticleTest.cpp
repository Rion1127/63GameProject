#include "ParticleTest.h"
#include "Easing.h"
#include "RRandom.h"
#include "Util.h"
ParticleTest::ParticleTest() :
	vertexCount(16)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("uv");
	////以下テスト用
	//particles_.resize(vertexCount);
	//for (size_t i = 0; i < particles_.size(); i++)
	//{
	//	particles_[i].end_frame = 30 * (1 * i);

	//	particles_[i].position = {
	//		0 + i * -5.f,
	//		4,
	//		0
	//	};
	//	particles_[i].scale = 1.f * i;
	//}
}

void ParticleTest::Add(size_t addNum, size_t time, Vector3 pos, Vector3 addVec, float scale)
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
		//進む角度を求める
		
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
		/*particles_[i].rot += {
			0.01f,
			-0.02f,
			0.005f
		};*/
	}
}
