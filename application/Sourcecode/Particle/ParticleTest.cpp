#include "ParticleTest.h"
#include "Easing.h"
#include "RRandom.h"
#include "Util.h"
#include <imgui.h>
ParticleTest::ParticleTest() :
	vertexCount(16)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("StarParticle");
	isBillBoard = true;
}

void ParticleTest::Add()
{
	transform_.position_ = emitter_->pos;
	for (int i = 0; i < emitter_->addNum; i++)
	{
		//指定した最大頂点数超えてたら生成しない
		if (particles_.size() >= vertexCount)
		{
			return;
		}
		//リストに要素を追加
		particles_.emplace_back();
		testParticles_.emplace_back();
		//追加した要素の参照
		auto& p = testParticles_.back();

		Vector3 vec = {
			RRandom::RandF(-emitter_->addVec.x,emitter_->addVec.x),
			RRandom::RandF(-emitter_->addVec.y,emitter_->addVec.y),
			0
		};

		p.position = vec;
		p.basePos = emitter_->pos;
		p.end_frame = emitter_->time;
		p.scale = emitter_->scale;
		p.baseScale = emitter_->scale;
	}
}

void ParticleTest::MoveUpdate()
{
	for (auto& p : particles_)
	{
		p.frame++;

		//float f = (float)p.frame / p.end_frame;

		p.rot.z += 0.01f;
	}
}
