#pragma once
#include "Texture.h"
#include "Vector3.h"
#include "Color.h"
#include "WorldTransform.h"

struct VertexParticle
{
	Vector3 pos;
	float scale;
	Vector3 rot;
	Vector2 ancorPoint = { 0.0f,0.0f };
	Color color;
};

class IParticle
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	IParticle();
	~IParticle();
	void Init(int vertexCount);

	inline void SetTexture(const Texture& texture) { this->texture = texture; }

	void Update();

	void Draw();

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
		int frame = 0;
		//終了フレーム
		int end_frame = 0;
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
private:
	//データ転送
	void TransferBuff();
	//end_frameを超えたら削除
	void DeleteUpdate();

	//void SetTextureRect(const Vec2 leftTopPos, const Vec2 rightDownPos);

public:
	//継承先のパーティクルの処理
	//動き更新
	virtual void MoveUpdate() = 0;
public:

	std::vector<VertexParticle> vertices_;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	ComPtr<ID3D12Resource> vertBuff_;

	WorldTransform transform;

	Vector3 pos;
	Vector3 scale;
	Vector3 rot;
	Texture texture;

	std::vector<Particle> particles;
};


