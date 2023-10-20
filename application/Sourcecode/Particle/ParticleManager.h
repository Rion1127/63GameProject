#pragma once
#include "IParticle.h"
#include <unordered_map>
#include <list>
#include "HitStop.h"

/**
 * @file ParticleManager.h
 * @brief パーティクルを管理するクラス
 */

class ParticleManager : public HitStop
{
private:
	std::list<std::shared_ptr<IEmitter>> emitters_;
public:
	static ParticleManager* GetInstance();

	void Update();

	void Draw();

	void AddParticle(const std::string& name,const std::shared_ptr<IEmitter>& emitter);

	void AllClear() { emitters_.clear(); };

	void AllReset();
private:
	ParticleManager();
};

