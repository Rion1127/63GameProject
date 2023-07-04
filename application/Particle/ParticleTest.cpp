#include "ParticleTest.h"
#include "Easing.h"
ParticleTest::ParticleTest()
{
	Init(vertexCount);
}

void ParticleTest::Add(int life, Vector3 pos, float scale)
{
	//�p�[�e�B�N����addNum���ǉ�
	int addNum = 6;
	//360�x / �p�[�e�B�N���̐�
	int degree = 360 / addNum;

	for (int i = 0; i < 360; i += degree)
	{
		//�w�肵���ő咸�_�������Ă��琶�����Ȃ�
		if (particles.size() >= vertexCount)
		{
			return;
		}
		//���X�g�ɗv�f��ǉ�
		particles.emplace_back();
		//�ǉ������v�f�̎Q��
		Particle& p = particles.back();
		//�i�ފp�x�����߂�
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
		//�C�[�W���O�ŕ`�������ɐi��
		Vector2 pos = {
			Easing::Circ::easeInOut(particles[i].basePos.x,particles[i].basePos.x + particles[i].velocity.x,f),
			Easing::Circ::easeInOut(particles[i].basePos.y,particles[i].basePos.y + particles[i].velocity.y,f),
		};
		particles[i].position = {
			pos.x,
			pos.y,
			0
		};
		//�X�P�[���ω�
		float scale = Easing::Back::easeInOut(particles[i].baseScale, 0.f, f);
		particles[i].scale = scale;
	}
}
