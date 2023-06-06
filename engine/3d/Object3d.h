#pragma once
#include <wrl.h>
#include <map>
#include <string>
#include "Vertices.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include <unordered_map>
#include "LightGroup.h"
#include "Model.h"

using namespace Microsoft::WRL;
class Object3d
{
private:
	Model* model_ = nullptr;

	Vector3 pos;
	Vector3 rot;
	Vector3 scale;

	WorldTransform WT_;

public:
	Object3d();
	~Object3d();
	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();

public:
	//セッター
	void SetModel(Model* model) { model_ = model; }
	void SetPos(Vector3 pos) { this->pos = pos; }
	void SetScale(Vector3 scale) { this->scale = scale; }
	void SetRot(Vector3 rot) { this->rot = rot; }

public:
	//ゲッター
	WorldTransform GetTransform() { return WT_; }
};

