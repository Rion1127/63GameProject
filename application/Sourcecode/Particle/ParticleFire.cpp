#include "ParticleFire.h"
#include "RRandom.h"
#include "Easing.h"
#include "Util.h"

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
		//�w�肵���ő咸�_�������Ă��琶�����Ȃ�
		if (particles_.size() >= vertexCount)
		{
			return;
		}
		//���X�g�ɗv�f��ǉ�
		particles_.emplace_back();
		//�ǉ������v�f�̎Q��
		Particle& p = particles_.back();

		Vector3 vec = {
			RRandom::RandF(-addVec.x,addVec.x),
			RRandom::RandF(-addVec.y,addVec.y),
			0
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

		MoveTo({ 0,0,0 }, 0.001f, p.velocity);
		MoveTo({ 0,0,0 }, 0.003f, p.addRot);
	}
}
