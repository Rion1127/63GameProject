#pragma once
#include "REngine.h"
#include "LightGroup.h"
#include "Model.h"

using namespace Microsoft::WRL;
class Object3d
{
private:
	std::unique_ptr<Model> model_ = nullptr;

	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

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
	void SetPos(const Vector3& pos) { pos_ = pos; }
	void SetScale(const Vector3& scale) { scale_ = scale; }
	void SetRot(const Vector3& rot) { rot_ = rot; }
	void SetAmbient(std::string name, Vector3 ambient) { model_->materials_.find(name)->second->SetAmbient(ambient); }
public:
	//�Q�b�^�[
	WorldTransform* GetTransform() { return &WT_; }
};

