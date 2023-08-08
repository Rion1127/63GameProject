#include "ParticleFire.h"
#include "RRandom.h"
#include "Easing.h"
#include "Util.h"

ParticleFire::ParticleFire() :
	IParticle("Particle"),
	vertexCount(64)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("Particle");
	isBillBoard = false;
	state_ = PipeLineState::Add;
}

void ParticleFire::Add(int32_t addNum, int32_t time, Vector3 pos, Vector3 addVec, float scale)
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

		p.position = vec;
		p.basePos = pos;
		p.end_frame = time;
		p.scale = scale;
		p.baseScale = scale;
		p.color = { 255,100,0,255 };
	}
}

void ParticleFire::MoveUpdate()
{
	for (auto& p : particles_)
	{
		p.frame++;

		float f = (float)p.frame / p.end_frame;

		p.position += { 0, 0.01f, 0 };

		/*if (f < 0.5f)p.color.a = f * 355.f;
		else p.color.a = (1 - f) * 355.f;*/

		p.scale -=0.01f;

		MoveTo({ 0,0,0 }, 0.001f, p.velocity);
		MoveTo({ 0,0,0 }, 0.003f, p.addRot);
	}
}
