#pragma once
#include "IParticle.h"
#include <unordered_map>
#include <list>
#include "HitStop.h"
#include "ParticleObjDataPool.h"
#include "ParticleGroundStump.h"
/**
 * @file ParticleManager.h
 * @brief パーティクルを管理するクラス
 */

class ParticleManager : public HitStop
{
private:
	std::list<std::shared_ptr<IEmitter>> emitters_;
	ParticleObjDataPool objDataPool_;
	std::vector<std::unique_ptr<ParticleDebri>> particleDebri_;
public:
	static ParticleManager* GetInstance();
	//更新
	void Update();
	//描画
	void Draw();
	//パーティクル追加
	void AddParticle(const std::string& name,const std::shared_ptr<IEmitter>& emitter);
	//全てのパーティクルを削除する
	void AllClear() { emitters_.clear(); };
	//全てのパーティクルを無効にする
	void AllReset();
private:
	ParticleManager();
	//キーによってパーティクルを初期化する
	void InitParticle(const std::string& name);
};

