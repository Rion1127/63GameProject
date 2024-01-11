#include "ParticleGroundStump.h"
#include "Easing.h"
#include "RRandom.h"
#include "ParticleManager.h"
#include "mSound.h"
#include "SoundVolume.h"
#pragma region ParticleGroundStump
ParticleGroundStump::ParticleGroundStump() :
	IParticle("Fire"),
	vertexCount(6)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("GroundCrack");
	billBoard = BillBoard::None;
	state_ = PipeLineState::Alpha;
}

void ParticleGroundStump::Add()
{
	float pitch = RRandom::RandF(0.7f, 1.3f);
	SoundManager::Play("GroundStumpSE", false, SoundVolume::GetValumeSE(), pitch);
	for (int i = 0; i < emitter_->addNum; i++)
	{
		//指定した最大頂点数超えてたら生成しない
		if (particles_.size() >= vertexCount)
		{
			return;
		}
		//リストに要素を追加
		particles_.emplace_back();
		landingParticles_.emplace_back();
		//追加した要素の参照
		auto& baseP = particles_.back();
		auto& p = landingParticles_.back();

		float randRotY = RRandom::RandF(0.f, Radian(360));

		p.position = emitter_->pos;
		p.addVec = emitter_->addVec;
		p.end_frame = emitter_->time;
		p.rot = { Radian(90),randRotY,0 };
		p.scale = emitter_->scale;
		p.color = { 255,255,255,255 };

		baseP = p;
	}

	std::shared_ptr<OneceEmitter> otherEmitter;
	otherEmitter = std::make_shared<OneceEmitter>();
	otherEmitter->particle = std::make_unique<ParticleStumpSmoke>();
	otherEmitter->addNum = 10;
	otherEmitter->time = emitter_->time;
	otherEmitter->pos = emitter_->pos;
	otherEmitter->scale = 3.5f;
	ParticleManager::GetInstance()->
		AddParticle("StumpSmoke", otherEmitter);
}

void ParticleGroundStump::MoveUpdate()
{
	for (int32_t i = 0; i < landingParticles_.size(); i++)
	{
		landingParticles_[i].rate = (float)landingParticles_[i].frame / (float)landingParticles_[i].end_frame;

		if (landingParticles_[i].frame >= landingParticles_[i].end_frame)
		{
			landingParticles_.erase(landingParticles_.begin() + i);
			vertices_.at(i).scale = 0;
			i = -1;
		}
	}

	uint32_t i = 0;
	for (auto& p : landingParticles_)
	{
		p.frame += 1.f * GameSpeed::GetGameSpeed();

		float f = (float)p.frame / p.end_frame;

		p.color.a = Easing::Quint::easeIn(255.f,0.f,f);
		p.rot += p.addRot;
		
		particles_[i] = p;
		i++;
	}
}
#pragma endregion

#pragma region ParticleStumpSmoke
ParticleStumpSmoke::ParticleStumpSmoke() :
	IParticle("Smoke"),
	vertexCount(10)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("Smoke");
	billBoard = BillBoard::BillBoard;
	state_ = PipeLineState::Alpha;
}

void ParticleStumpSmoke::Add()
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
		landingParticles_.emplace_back();
		//追加した要素の参照
		auto& baseP = particles_.back();
		auto& p = landingParticles_.back();

		float length = RRandom::RandF(0.5f, 5.f);
		Vector3 randPos = {
			RRandom::RandF(-1.f,1.f),
			0,
			RRandom::RandF(-1.f,1.f),
		};
		Vector3 randAddVec = {
			0,
			RRandom::RandF(0.005f,0.04f),
			0,
		};
		randPos = randPos.normalize() * length;
		p.position = randPos;
		p.addVec = randAddVec;
		p.end_frame = emitter_->time;
		p.scale = emitter_->scale;
		p.color = { 100,100,100,150 };

		baseP = p;
	}
}

void ParticleStumpSmoke::MoveUpdate()
{
	for (int32_t i = 0; i < landingParticles_.size(); i++)
	{
		landingParticles_[i].rate = (float)landingParticles_[i].frame / (float)landingParticles_[i].end_frame;

		if (landingParticles_[i].frame >= landingParticles_[i].end_frame)
		{
			landingParticles_.erase(landingParticles_.begin() + i);
			vertices_.at(i).scale = 0;
			i = -1;
		}
	}

	uint32_t i = 0;
	for (auto& p : landingParticles_)
	{
		p.frame += 1.f * GameSpeed::GetGameSpeed();

		float f = (float)p.frame / p.end_frame;
		p.position += p.addVec;
		p.color.a = Easing::Quint::easeIn(150.f, 0.f, f);
		
		particles_[i] = p;
		i++;
	}
}
#pragma endregion

ParticleDebri::ParticleDebri(Object3d* obj, int32_t addNum, Vector3 pos, float activeTime)
{
	timer_.SetLimitTime(activeTime);
	for (int32_t i = 0; i < addNum; i++)
	{
		debris_.emplace_back();
		auto& obj_ = debris_.back();
		obj_.obj = obj;

		Vector3 randPos = { (float)RRandom::Rand(-3,3),0,(float)RRandom::Rand(-3,3) };
		Vector3 randaddVec = { RRandom::RandF(-0.2f,0.2f),0,RRandom::RandF(-0.2f,0.2f) };
		obj_.addVec = randaddVec;
		obj_.pos = pos + randPos;
		obj_.WT.position_ = obj_.pos;
		obj_.WT.scale_ = { RRandom::RandF(0.3f,0.6f),RRandom::RandF(0.3f,0.6f),RRandom::RandF(0.3f,0.6f) };

		obj_.addRot = {
			RRandom::RandF(-0.1f,0.1f),
			RRandom::RandF(-0.1f,0.1f),
			RRandom::RandF(-0.1f,0.1f),
		};
		obj_.gravity.SetAddValue({ 0,-0.01f,0.0f });
		obj_.gravity.SetGrabity({ 0,RRandom::RandF(0.2f,0.3f),0.0f });
	}
}

void ParticleDebri::Update()
{
	timer_.AddTime(GameSpeed::GetGameSpeed());
	for (auto& debri : debris_)
	{
		debri.gravity.Update();

		Vector3 rot = debri.WT.rotation_;
		rot += debri.addRot;
		Vector3 pos = debri.WT.position_;
		MoveTo({ 0,0,0 }, 0.005f, debri.addVec);
		pos += debri.addVec + debri.gravity.GetGravityValue() * GameSpeed::GetGameSpeed();

		debri.WT.position_ = pos;
		debri.WT.rotation_ = rot;
		debri.WT.Update();
	}
}

void ParticleDebri::Draw()
{
	for (auto& debri : debris_)
	{
		debri.obj->Draw(debri.WT);
	}
}
