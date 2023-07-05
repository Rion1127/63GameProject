#pragma once
#include "REngine.h"
#include "LightGroup.h"
#include "Model.h"

class Object3d
{
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	std::unique_ptr<Model> model_ = nullptr;
public:
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
	void SetModel(std::unique_ptr<Model> model) { model_ = std::move(model); }
	void SetAmbient(std::string name, Vector3 ambient) { model_->materials_.find(name)->second->SetAmbient(ambient); }
public:
	//ゲッター
	WorldTransform* GetTransform() { return &WT_; }
};

