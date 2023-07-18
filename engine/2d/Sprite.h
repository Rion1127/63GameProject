#pragma once
#include <wrl.h>
#include <vector>
#include "math.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include <imgui.h>
#include "Vector2.h"
#include "Texture.h"
#include "Color.h"
#include "myMath.h"
class Sprite
{
private:
	struct Vertex {
		Vector3 pos;
		Vector2 uv;
	};
	enum VertNum {
		LB,	//左下
		LT,	//左上
		RB,	//右下
		RT	//右上
	};
	struct ConstBufferDataMaterial {
		Color color;
	};
	struct ConstBufferDataTransform {
		Matrix4 mat;
	};
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static uint32_t SAllNum;
	uint32_t spriteNum_ = 0;

	//頂点データ
	std::vector<Vertex> vertices_;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	// 頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	Vertex* vertMap_ = nullptr;

	//インデックスデータ
	std::vector<uint16_t> indices_;
	// インデックスバッファの生成
	ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	//定数バッファ用データ構造体
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;

	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;
	ConstBufferDataTransform* constMapTransform_ = nullptr;
	Matrix4 matProjection_;
	
	Matrix4 matWorld_{};// ワールド行列
	
	Color color_;//色
	
	float rot_;				//回転
	Vector2 pos_;			//座標
	Vector2 Scale_;			//スケール
	Vector2 anchorPoint_;	//アンカーポイント
	bool isFlipX_ = false;	//左右フリップ
	bool isFlipY_ = false;	//上下フリップ
	bool isInvisible_ = false;					//非表示フラグ
	Vector2 textureLeftTop_ = { 0.0f,0.0f };	//テクスチャ左上座標
	Vector2 textureSize_ = { 0.f,0.f };			//テクスチャ切り出しサイズ
	bool isImguiDisplay_ = false;				//imgui表示フラグ
	uint32_t descriptorSize_ = 0;
	Texture texture_;		//テクスチャ

	//imgui
	std::string guiName_;
	uint32_t clicked_ = 0;
	const char* gui_;

	std::string name_;
public:
	void Update();

	void DrawImGui();

	//画像サイズ自動取得(描画座標は中心)
	void Draw();
	//画像の頂点データを自分で指定
	void Draw(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY, UINT descriptorSize);
	/// <summary>
	/// ブレンド設定
	/// </summary>
	/// <param name="BLEND_ALPHA">アルファブレンド</param>
	/// <param name="BLEND_SUB">減算合成</param>
	/// <param name="BLEND_NEGA">色反転合成</param>
	/// <param name="BLEND_NORMAL">半透明合成</param>
	static void SetBlend(uint32_t blend);

	static void AddAllNum() { SAllNum++; }
public:
	//セッター
	void Ini(const std::string& guiname = "");
	//座標を代入する
	void SetPos(const Vector2& pos) { pos_ = pos; }
	//回転させる
	void SetRot(float rot) { rot_ = rot; }
	//スケール設定
	void SetScale(const Vector2& scale) { Scale_ = scale; }
	//画像の中心を決める
	void SetAnchor(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }
	//色変更
	void SetColor(const Color& color) { color_ = color; }
	//左右反転
	void SetFlipX(bool flip) { isFlipX_ = flip; }
	//上下反転
	void SetFlipY(bool flip) { isFlipY_ = flip; }
	//表示フラグ
	void SetInvisivle(bool invisivle) { isInvisible_ = invisivle; }
	//画像左上変更
	void SetTex_LeftTop(const Vector2& pos) { textureLeftTop_ = pos; }
	//画像UV座標変更
	void SetTex_Size(const Vector2& pos) { textureSize_ = pos; }
	//テクスチャ
	void SetTexture(Texture* texture) { texture_ = *texture; }
	void SetImGui(bool flag) { isImguiDisplay_ = flag; }
public:
	//ゲッター
	Vector2 GetScale() { return Scale_; }
	Vector2 GetPos() { return pos_; }
	Color GetColor() { return color_; }
private:
	void TransferVertex();
};

