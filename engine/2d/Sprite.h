#pragma once
#include <wrl.h>
#include <vector>
#include "math.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include <imgui.h>
#include <DirectXMath.h>
class Sprite
{
public:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static void StaticIni();
	void Ini(std::string guiname = "");
	//座標を代入する
	void SetPos(float x, float y) {
		pos_.x = x;
		pos_.y = y;
	};
	void SetPos(Vector2 pos);
	//回転させる
	void SetRot(float rot);
	//スケール設定
	void SetScale(float x, float y) {
		Scale_.x = x;
		Scale_.y = y;
	};
	void SetScale(Vector2 scale);
	//画像の中心を決める
	void SetAnchor(float x, float y) {
		anchorPoint_.x = x;
		anchorPoint_.y = y;
	};
	void SetAnchor(Vector2 pos) {
		anchorPoint_ = pos;
	};
	//色変更
	void ChangeColor(float x, float y, float z, float w) {
		color_ = {x, y, z, w};
	};
	void ChangeColor(DirectX::XMFLOAT4 color) {
		color_ = color;
	};
	//左右反転
	void SetFlipX(bool flip) { isFlipX_ = flip; }
	//上下反転
	void SetFlipY(bool flip) { isFlipY_ = flip; }
	//表示フラグ
	void SetInvisivle(bool invisivle) { isInvisible_ = invisivle; }
	//画像左上変更
	void SetTex_LeftTop(Vector2 pos) { textureLeftTop_ = pos; }
	//画像UV座標変更
	void SetTex_Size(Vector2 pos) { textureSize = pos; }

	Vector2 GetScale() { return Scale_; }
	Vector2 GetPos() { return pos_; }

	void SetImGui(bool flag) { isImguiDisplay = flag; }
	void DrawImGui();

	static void PreDraw();
	//画像サイズ自動取得(描画座標は中心)
	void Draw(UINT descriptorSize);
	//画像の頂点データを自分で指定
	void Draw(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY, UINT descriptorSize);
	/// <summary>
	/// ブレンド設定
	/// </summary>
	/// <param name="BLEND_ALPHA">アルファブレンド</param>
	/// <param name="BLEND_SUB">減算合成</param>
	/// <param name="BLEND_NEGA">色反転合成</param>
	/// <param name="BLEND_NORMAL">半透明合成</param>
	static void SetBlend(int blend);

	static void AddAllNum() { allNum++; }
private:
	static int allNum;
	int spriteNum = 0;

	static DirectXCommon* directX_;
	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	enum VertNum {
		LB,	//左下
		LT,	//左上
		RB,	//右下
		RT	//右上
	};

#pragma region 頂点データ
	//頂点データ
	std::vector<Vertex> vertices;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// 頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff = nullptr;

	Vertex* vertMap = nullptr;
#pragma endregion
#pragma region インデックスデータ
	//インデックスデータ
	std::vector<uint16_t> indices;
	// インデックスバッファの生成
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
#pragma endregion

	ComPtr<ID3D12GraphicsCommandList> commandList_;
	//定数バッファ用データ構造体
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;
	};
	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	struct ConstBufferDataTransform {
		XMMATRIX mat;
	};
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;
	XMMATRIX matProjection;

	// ワールド行列
	DirectX::XMMATRIX matWorld_{};
	//色
	DirectX::XMFLOAT4 color_;
	//回転
	float rot_;
	//座標
	Vector2 pos_;
	//スケール
	Vector2 Scale_;
	//アンカーポイント
	Vector2 anchorPoint_;
	//左右フリップ
	bool isFlipX_ = false;
	//上下フリップ
	bool isFlipY_ = false;
	//非表示フラグ
	bool isInvisible_ = false;
	//テクスチャ左上座標
	Vector2 textureLeftTop_ = { 0.0f,0.0f };
	//テクスチャ切り出しサイズ
	Vector2 textureSize = { 0.f,0.f };
	//imgui表示
	bool isImguiDisplay = false;

#pragma region
	std::string guiName_;
	int clicked = 0;
	const char* gui;

	std::string name;
#pragma endregion
};

