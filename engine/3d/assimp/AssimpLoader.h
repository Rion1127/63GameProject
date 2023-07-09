#pragma once
#include "REngine.h"
#include "myMath.h"
#include "AssimpModel.h"
#include <string>



struct aiMesh;
struct aiMaterial;
struct aiNode;

struct Mesh {
	Vertices Vertices; // ���_�f�[�^�̔z��
	std::wstring diffuseMap; // �e�N�X�`���̃t�@�C���p�X
};

class AssimpLoader
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static AssimpLoader* GetInstance();

	std::unique_ptr<AssimpModel> Load(std::string fileName, AssimpModel* model);
private:
	void LoadVertices(Vertices* vert, const aiMesh* aimesh);
	void LoadMaterial(std::string fileName,Material* material, const aiMaterial* aimaterial);
	void LoadSkin(AssimpModel* model, const aiMesh* aimesh);
	void LoadNode(AssimpModel* model, Node* parent, const aiNode* node);
	AssimpLoader(){};
private:
};

