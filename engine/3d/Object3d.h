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
	bool isVisible_ = true;
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
	void SetPos(const Vector3& pos) { WT_.position_ = pos; }
	void SetScale(const Vector3& scale) { WT_.scale_ = scale; }
	void SetRot(const Vector3& rot) { WT_.rotation_ = rot; }
	void SetIsVisible(bool flag) { isVisible_ = flag; }
public:
	//ゲッター
	WorldTransform* GetTransform() { return &WT_; }
	Vector3 GetPos() { return WT_.position_; }
	Vector3 GetScale() { return WT_.scale_; }
	Vector3 GetRot() { return WT_.rotation_; }
	bool GetIsVisible() { return isVisible_; }
	Model* GetModel() { return model_.get(); }
};

