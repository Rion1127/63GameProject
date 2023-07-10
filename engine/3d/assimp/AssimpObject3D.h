#pragma once
#include "AssimpModel.h"
#include "WorldTransform.h"
#include <assimp/anim.h>
#include "Quaternion.h"

struct aiNode;

struct Animation
{
	uint32_t index = 0;
	Timer timer;
	bool isPlay = false;
};

class AssimpObject3D
{
private:
	std::unique_ptr<AssimpModel> model_;
	WorldTransform worldTransform_;
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	static const int32_t MAX_BONES = 32;
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBuffDataSkin {
		Matrix4 bones[MAX_BONES];
	};
	//�萔�o�b�t�@�̐���
	ComPtr<ID3D12Resource> constBuff_ = nullptr;
	//�萔�o�b�t�@�̃}�b�s���O
	ConstBuffDataSkin* constMap_ = nullptr;

	Animation animation_;
public:
	AssimpObject3D();

	void Update();

	void Draw();
public:
	void PlayAnimation();
	void ParseNodeHeirarchy(const float currentTime, const uint32_t index, const Matrix4& parentMat, const aiNode* rootNode);
	aiNodeAnim* FindNodeAnim(const std::string& nodeName, aiAnimation* animation);

	uint32_t FindScaleIndex(const aiNodeAnim* nodeAnim, const float currentTime);
	uint32_t FindRotIndex(const aiNodeAnim* nodeAnim, const float currentTime);
	uint32_t FindPosIndex(const aiNodeAnim* nodeAnim, const float currentTime);

	Vector3 CalcCurrentScale(const aiNodeAnim* nodeAnim, const float currentTime);
	Quaternion CalcCurrentRot(const aiNodeAnim* nodeAnim, const float currentTime);
	Vector3 CalcCurrentPos(const aiNodeAnim* nodeAnim, const float currentTime);
public:
	void SetPos(Vector3 pos) { worldTransform_.position_ = pos; }
	void SetScale(Vector3 scale) { worldTransform_.scale_ = scale; }
	void SetRot(Vector3 rot) { worldTransform_.rotation_ = rot; }
	void SetModel(std::unique_ptr<AssimpModel> model) { model_ = std::move(model); }
};

