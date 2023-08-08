#include "ParticleFire.h"
#include "RRandom.h"
#include "Easing.h"
#include "Util.h"

#pragma region 炎エフェクト
ParticleFire::ParticleFire() :
	IParticle("Fire"),
	vertexCount(128)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("Particle");
	isBillBoard = true;
	state_ = PipeLineState::Add;
}

void ParticleFire::Add(int32_t addNum, int32_t time, Vector3 pos, Vector3 addVec, float scale)
{
	//transform_.position_ = pos;
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

		Vector3 dist = pos - (pos + vec);

		p.position = pos + vec;
		p.basePos = pos;
		p.end_frame = time;
		p.scale = scale;
		p.baseScale = scale;
		p.addRot.z = RRandom::RandF(-0.01f, 0.01f);
		p.color = { 255,100,0,255 };
	}
}

void ParticleFire::MoveUpdate()
{
	for (auto& p : particles_)
	{
		p.frame++;

		float f = (float)p.frame / p.end_frame;

		p.position += { 0, 0.02f, 0 };

		if (f < 0.5f)p.color.a = f * 355.f;
		else p.color.a = (1 - f) * 355.f;

		p.scale -=0.01f;
		p.scale = Max(0.f, p.scale);

		p.rot.z += p.addRot.z;
	}
}
#pragma endregion

#pragma region 炎エフェクト
ParticleFireCircle::ParticleFireCircle() :
	IParticle("Fire"),
	vertexCount(2)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("FireCircle");
	isBillBoard = false;
	state_ = PipeLineState::Add;
}

void ParticleFireCircle::Add(int32_t addNum, int32_t time, Vector3 pos, Vector3 addVec, float scale)
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

		p.position = pos;
		p.position.y += 0.5f * (1 + i);
		p.basePos = pos;
		p.end_frame = time;
		p.scale = 0;
		p.baseScale = scale * (1.5f + i);
		p.addRot.y = RRandom::RandF(-0.03f, 0.03f);
		p.color = { 255,100,0,255 };
	}
}

void ParticleFireCircle::MoveUpdate()
{
	for (auto& p : particles_)
	{
		p.frame++;

		float f = (float)p.frame / p.end_frame;

		if (f > 0.7f)p.color.a -= 7.f;
		p.color.a = Max(0.f, p.color.a);

		f *= 3.f;
		f = Min(f, 1.f);
		p.scale = Easing::Circ::easeOut(0.f, p.baseScale,f);

		p.rot.x = Radian(90);
		p.rot.y += p.addRot.y;
	}
}
#pragma endregion


