#pragma once
#include "REngine.h"

struct aiMesh;
struct aiMaterial;
struct aiBone;

struct Mesh {
	Vertices Vertices; // ���_�f�[�^�̔z��
	std::wstring diffuseMap; // �e�N�X�`���̃t�@�C���p�X
};

struct BoneData {
	static const uint32_t NUM_BONES_PER_VERTEX = 4;

	uint32_t IDs[NUM_BONES_PER_VERTEX];
	float Weights[NUM_BONES_PER_VERTEX];
	Matrix4 boneMatrix_;
};

struct ImportSettings // �C���|�[�g����Ƃ��̃p�����[�^
{
public:
	
	const wchar_t* filename = nullptr; // �t�@�C���p�X
	std::vector<Mesh>& meshes; // �o�͐�̃��b�V���z��
	bool inverseU = false; // U���W�𔽓]�����邩
	bool inverseV = false; // V���W�𔽓]�����邩

	std::vector<BoneData> boneData;
};

class AssimpLoader
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static AssimpLoader* GetInstance();

	bool Load(ImportSettings* setting); // ���f�������[�h����

private:
	void LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV);
	void LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src);

	void LoadBones(uint32_t MeshIndex, const aiMesh* pMesh, ImportSettings* setting);
private:
	
};