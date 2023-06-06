#pragma once
#include <wrl.h>
#include <map>
#include <string>
#include "Vertices.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include <unordered_map>
#include "LightGroup.h"

class Model
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	~Model();

	static Model* GetInstance();

	static void Ini();
	//���f����ǂݍ���
	static Model* CreateOBJ(const std::string& modelname, bool smoothing = false);
	static std::unique_ptr<Model> CreateOBJ_uniptr(const std::string& modelname, bool smoothing = false);
	/// <summary>
	/// �u�����h�ݒ�
	/// </summary>
	/// <param name="BLEND_ALPHA">�A���t�@�u�����h</param>
	/// <param name="BLEND_SUB">���Z����</param>
	/// <param name="BLEND_NEGA">�F���]����</param>
	/// <param name="BLEND_NORMAL">����������</param>
	static void SetBlend(int blend);

	void SetModel(const Model* model);

	static void SetLight(LightGroup* lightGroup) { Model::lightGroup = lightGroup; }
private:
	//���f��������(CreateOBJ()�ɓ����Ă���)
	void ModelIni(const std::string& modelname, bool smoothing);
	//.OBJ�������ǂݍ���(ModelIni()�ɓ����Ă���)
	void LoadOBJ(const std::string& modelname);
	//.mtl����e�N�X�`����ǂݍ���
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	void LoadTexture();

	void AddMaterial(Material* material);

public:
	static void PreDraw();

	//�I�u�W�F�N�g�̐F��ς���
	void ObjChangeColor(float x, float y, float z, float w);
	void ObjChangeColor(XMFLOAT4 color_);

	void DrawOBJ(WorldTransform* worldTransform);
	void DrawOBJ(WorldTransform* worldTransform, uint32_t textureHandle);

private:
	struct VertexPosNormalUv {
		XMFLOAT3 pos;		//xyz���W
		XMFLOAT3 normal;	//�@���x�N�g��
		XMFLOAT2 uv;		//uv���W
	};

	// ���O
	std::string name_;

	//���_�@���X���[�W���O�p�f�[�^
	bool smoothing = false;
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
	void AddSmoothData(unsigned short indexPositon, unsigned short indexVertex);
	//���������ꂽ���_�@���̌v�Z
	void CalculateSmoothedVertexNormals();
public:
	std::vector<Vertices*> vert_;
	std::vector<uint32_t> textureHandle_;
	// �}�e���A���R���e�i
	std::map<std::string, Material*> materials_;
	//���C�g
	static LightGroup* lightGroup;

};

