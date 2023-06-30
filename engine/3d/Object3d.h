#pragma once
#include "REngine.h"
#include "LightGroup.h"
#include "Model.h"

using namespace Microsoft::WRL;
class Object3d
{
private:
	std::unique_ptr<Model> model_ = nullptr;
public:
	WorldTransform WT_;
public:
	Object3d();
	~Object3d();
	//������
	void Init();
	//�X�V
	void Update();
	//�`��
	void Draw();

public:
	//�Z�b�^�[
	void SetModel(std::unique_ptr<Model> model) { model_ = std::move(model); }
	void SetAmbient(std::string name, Vector3 ambient) { model_->materials_.find(name)->second->SetAmbient(ambient); }
public:
	//�Q�b�^�[
	WorldTransform* GetTransform() { return &WT_; }
};

