#include "ParticleTest.h"
#include "Easing.h"
ParticleTest::ParticleTest()
{
	Init(vertexCount);
}

void ParticleTest::Add(int life, Vector3 pos, float scale)
{
	//パーティクルをaddNum分追加
	int addNum = 6;
	//360度 / パーティクルの数
	int degree = 360 / addNum;

	for (int i = 0; i < 360; i += degree)
	{
		//指定した最大頂点数超えてたら生成しない
		if (particles.size() >= vertexCount)
		{
			return;
		}
		//リストに要素を追加
		particles.emplace_back();
		//追加した要素の参照
		Particle& p = particles.back();
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
	for (size_t i = 0; i < particles.size(); i++)
	{
		particles[i].frame++;

		float f = 0;

		f = (float)particles[i].frame / particles[i].end_frame;
		//イージングで描く方向に進む
		Vector2 pos = {
			Easing::Circ::easeInOut(particles[i].basePos.x,particles[i].basePos.x + particles[i].velocity.x,f),
			Easing::Circ::easeInOut(particles[i].basePos.y,particles[i].basePos.y + particles[i].velocity.y,f),
		};
		particles[i].position = {
			pos.x,
			pos.y,
			0
		};
		//スケール変化
		float scale = Easing::Back::easeInOut(particles[i].baseScale, 0.f, f);
		particles[i].scale = scale;
	}
}
