#pragma once
#include "Texture.h"
#include "Vector3.h"
#include "Color.h"
#include "WorldTransform.h"



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
		//座標
		Vector3 position = {};
		Vector3 basePos = {};
		Vector3 endPos = {};
		Vector3* parentPos;
		//速度
		Vector3 velocity = {};
		Vector3 baseVelo = {};
		Vector3 veloAdd = {};

		Vector3 rot = {};
		Vector3 addRot = {};
		//現在フレーム
		size_t frame = 0;
		//終了フレーム
		size_t end_frame = 0;
		// ローカルスケール
		float scale = 1.0f;			//スケール
		float maxScale = 1.f;
		float baseScale = 0;
		//色(RGBA)
		Color color = {};
		Color originColor = {};	//色の初期値
		Color endColor = {};	//色の最終値

		int particleType = 0;
		Vector2 ancorPoint_ = { 0,0 };
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

	size_t maxParticleNum_;
	//パーティクルデータ
	std::vector<Particle> particles_;
public:
	IParticle() {};
	~IParticle();

	void Update();

	void Draw();
private:
	//データ転送
	void TransferBuff();
	//end_frameを超えたら削除
	void DeleteUpdate();
protected:
	void Init(size_t vertexCount);
	//継承先のパーティクルの処理
	//動き更新
	virtual void MoveUpdate() = 0;
};


