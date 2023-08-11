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
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	struct VertexPosNormalUv {
		Vector3 pos;		//xyz���W
		Vector3 normal;	//�@���x�N�g��
		Vector2 uv;		//uv���W
	};

	// ���O
	std::string name_;

	//���_�@���X���[�W���O�p�f�[�^
	bool smoothing_ = false;
	std::vector<std::unordered_map<unsigned short, std::vector<unsigned short>>> smoothData_;
public:
	std::vector<std::unique_ptr<Vertices>> vert_;
	// �}�e���A���R���e�i
	std::map<std::string, std::unique_ptr<Material>> materials_;
	//���C�g
	static std::shared_ptr<LightGroup> lightGroup_;
public:
	Model() {};
	~Model();

	static Model* GetInstance();

	//���f����ǂݍ���
	static Model* CreateOBJ(const std::string& modelname, bool smoothing = false);
	static std::unique_ptr<Model> CreateOBJ_uniptr(const std::string& modelname, bool smoothing = false);	

	void SetModel(const Model* model);

	static void SetLight(std::shared_ptr<LightGroup> lightGroup) { Model::lightGroup_ = lightGroup; }
private:
	//���f��������(CreateOBJ()�ɓ����Ă���)
	void ModelIni(const std::string& modelname, bool smoothing);
	//.OBJ�������ǂݍ���(ModelIni()�ɓ����Ă���)
	void LoadOBJ(const std::string& modelname);
	//.mtl����e�N�X�`����ǂݍ���
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	void LoadTexture();

	void AddMaterial(Material* material);

	void AddSmoothData(unsigned short indexPositon, unsigned short indexVertex,uint32_t dataindex);
	//���������ꂽ���_�@���̌v�Z
	void CalculateSmoothedVertexNormals();
public:
	void DrawOBJ(const WorldTransform& worldTransform);
	void DrawOBJ(const WorldTransform& worldTransform, uint32_t textureHandle);
public:
	std::string GetModelName() { return name_; }
};

