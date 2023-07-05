#include "ParticleTest.h"
#include "Easing.h"
ParticleTest::ParticleTest() :
	vertexCount(6)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("uv");
	//以下テスト用
	particles_.resize(vertexCount);
	for (size_t i = 0; i < particles_.size(); i++)
	{
		particles_[i].end_frame = 30 * (1 * i);

		particles_[i].position = {
			0 + i * -5.f,
			4,
			0
		};
		particles_[i].scale = 1.f * i;
	}
}

void ParticleTest::Add(size_t addNum, size_t life, Vector3 pos, float scale)
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
		float rad = i * 3.1415f / 180.f;
		Vector2 vec = { sinf(rad),cosf(rad) };
		vec = vec.normalize();

		p.position = pos;
		p.basePos = pos;
		p.end_frame = life;
		p.velocity = {
			vec.x * (scale * 2.f),
			vec.y * (scale * 2.f),
			0
		};
		p.scale = scale;
		p.baseScale = scale;
	}
}

void ParticleTest::MoveUpdate()
{

	for (size_t i = 0; i < particles_.size(); i++)
	{
		//particles_[i].frame++;
		float f = 0;

		f = (float)particles_[i].frame / particles_[i].end_frame;
		
		
		particles_[i].rot += {
			0.01f,
			-0.02f,
			0.005f
		};
	}
}
