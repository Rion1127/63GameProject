#pragma once
#include <wrl.h>
#include <map>
#include <string>
#include <memory>
#include <unordered_map>
#include "Vertices.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include "LightGroup.h"
#include "Texture.h"
#include "myMath.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Material.h"
#include "WorldTransform.h"

class Model
{
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	struct VertexPosNormalUv {
		Vector3 pos;		//xyz座標
		Vector3 normal;	//法線ベクトル
		Vector2 uv;		//uv座標
	};

	// 名前
	std::string name_;

	//頂点法線スムージング用データ
	bool smoothing_ = false;
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData_;
public:
	std::vector<std::unique_ptr<Vertices>> vert_;
	std::vector<Texture> texture_;
	// マテリアルコンテナ
	std::map<std::string, std::unique_ptr<Material>> materials_;
	//ライト
	static std::shared_ptr<LightGroup> lightGroup_;
public:
	~Model();

	static Model* GetInstance();

	static void Ini();
	//モデルを読み込む
	static Model* CreateOBJ(const std::string& modelname, bool smoothing = false);
	static std::unique_ptr<Model> CreateOBJ_uniptr(const std::string& modelname, bool smoothing = false);
	

	void SetModel(const Model* model);

	static void SetLight(std::shared_ptr<LightGroup> lightGroup) { Model::lightGroup_ = lightGroup; }
private:
	//モデル初期化(CreateOBJ()に入っている)
	void ModelIni(const std::string& modelname, bool smoothing);
	//.OBJから情報を読み込む(ModelIni()に入っている)
	void LoadOBJ(const std::string& modelname);
	//.mtlからテクスチャを読み込む
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	void LoadTexture();

	void AddMaterial(Material* material);

	void AddSmoothData(unsigned short indexPositon, unsigned short indexVertex);
	//平滑化された頂点法線の計算
	void CalculateSmoothedVertexNormals();
public:
	void DrawOBJ(const WorldTransform& worldTransform);
	void DrawOBJ(const WorldTransform& worldTransform, uint32_t textureHandle);
};

