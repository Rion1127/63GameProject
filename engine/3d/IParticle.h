#pragma once
#include "Texture.h"
#include "Vector3.h"
#include "Color.h"
#include "WorldTransform.h"
#include "Timer.h"
#include "PipelineManager.h"
#include "GameSpeed.h"

/**
 * @file IParticle.h
 * @brief インターフェースクラス
 * @brief パーティクルクラスに継承するクラス
 */

struct IEmitter;

class IParticle
{
protected:
	struct VertexParticle
	{
		Vector3 pos;
		float scale;
		Vector3 rot;
		Vector2 ancorPoint = { 0.0f,0.0f };
		Color color;
	};
	struct Particle
	{
		Vector3 position = {};	//座標
		float scale = 1.0f;		//スケール
		Vector3 rot;
		Vector2 ancorPoint_ = { 0,0 };
		Color color = {};

		float frame = 0;		//現在フレーム
		float end_frame = 0;	//終了フレーム
		float rate = 0;
	};
protected:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//頂点データ
	std::vector<VertexParticle> vertices_;

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	ComPtr<ID3D12Resource> vertBuff_;

	Texture texture;

	WorldTransform transform_;

	int32_t maxParticleNum_;
	//パーティクルデータ
	std::vector<Particle> particles_;

	bool isBillBoard;
	std::string shaderName_;
	PipeLineState state_;
	IEmitter* emitter_;
public:
	IParticle(std::string shaderName = "Particle") {
		shaderName_ = shaderName;
		isBillBoard = false;
		state_ = PipeLineState::Alpha;
	};
	virtual ~IParticle();

	void Update();
	//データ転送
	void TransferBuff();

	void Draw();

	virtual void Add() = 0;
	void SetEmitter(IEmitter* emitter) { emitter_ = emitter; }
public:
	std::string GetShaderName() { return shaderName_; }
	size_t GetParticleNum() { return particles_.size(); }
	PipeLineState GetPipelineState() { return state_; }
private:
	//end_frameを超えたら削除
	void DeleteUpdate();
protected:
	void Init(int32_t vertexCount);
	//継承先のパーティクルの処理
	//動き更新
	virtual void MoveUpdate() = 0;
};

struct IEmitter {
	std::unique_ptr<IParticle> particle;
	int32_t addNum;	//同時発生数
	float time;	//パーティクル生存時間
	Vector3 pos;	//エミッター中心座標
	Vector3* parentPos = nullptr;	//エミッター中心座標
	Vector3 addVec;	//移動方向ベクトル
	float scale;
	bool isActive = true;
	Timer popCoolTime_;
};

struct OneceEmitter : public IEmitter {
	OneceEmitter() { isActive = false; }
};
//フラグがfalseになるまで出続ける
struct ContinuousEmitter : public IEmitter {

};

