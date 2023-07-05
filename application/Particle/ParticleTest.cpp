#include "ParticleTest.h"
#include "Easing.h"
#include "RRandom.h"
#include "Util.h"
ParticleTest::ParticleTest() :
	vertexCount(16)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("uv");
	////�ȉ��e�X�g�p
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
		//�w�肵���ő咸�_�������Ă��琶�����Ȃ�
		if (particles_.size() >= vertexCount)
		{
			return;
		}
		//���X�g�ɗv�f��ǉ�
		particles_.emplace_back();
		//�ǉ������v�f�̎Q��
		Particle& p = particles_.back();
		//�i�ފp�x�����߂�
		
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
