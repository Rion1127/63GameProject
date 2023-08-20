#include <assimp/scene.h>
#include "AssimpObject3D.h"
#include "PipelineManager.h"
#include "Util.h"
#include "DirectX.h"
#include "AssimpLoader.h"
#include "mInput.h"

AssimpObject3D::AssimpObject3D()
{
	constBuff_ = CreateBuff(constMap_);
	SetModel(AssimpLoader::GetInstance()->Load("application/Resources/boneTest/AMove.fbx", nullptr));
	animation_.timer.SetLimitTime(180);
}

void AssimpObject3D::Update()
{
	HRESULT result;

	std::vector<Bone>& bones = model_->bones;
	if (model_->scene->HasAnimations()) {
		PlayAnimation();
	}

	ConstBuffDataSkin* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	if (model_->scene->HasAnimations()) {
		for (int32_t i = 0; i < bones.size(); i++) {
			//���̎p���s��
			Matrix4 matCurrentPose;
			matCurrentPose.UnitMatrix();

			constMap->bones[i] = bones[i].currentMat * matCurrentPose;
		}
	}
	else {
		//���̎p���s��
		Matrix4 matCurrentPose;
		matCurrentPose.UnitMatrix();
		constMap->bones[0] = matCurrentPose;
	}
	constBuff_->Unmap(0, nullptr);

	worldTransform_.Update();

}

void AssimpObject3D::Draw()
{
	//���[�g�p�����[�^�z��4�Ԗڂ��w��
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(4, constBuff_->GetGPUVirtualAddress());
	model_->Draw(worldTransform_);
}

void AssimpObject3D::PlayAnimation()
{
	Matrix4 identity;
	identity.UnitMatrix();
	// �A�j���[�V�����Đ����Ȃ�
	animation_.isPlay = true;
	if (animation_.isPlay == false)
	{
		ParseNodeHeirarchy(0.f, 0, identity, model_->scene->mRootNode);
		return;
	}

	// ���݂̃t���[��
	float endTime = (float)model_->scene->mAnimations[animation_.index]->mDuration - 6.f;
	float currentTime = animation_.timer.GetTimeRate() * endTime;
	ParseNodeHeirarchy(currentTime, animation_.index, identity, model_->scene->mRootNode);
	//���Z�b�g����������s�����x�v�Z���Ȃ���
	if (currentTime >= endTime) {
		animation_.timer.Reset();
		currentTime = animation_.timer.GetTimeRate() * endTime;
		ParseNodeHeirarchy(currentTime, animation_.index, identity, model_->scene->mRootNode);
	}


	static bool animeStart = true;

	if (Key::TriggerKey(DIK_Z)) {
		animeStart = (animeStart == true) ? false : true;
	}
	if (animeStart)animation_.timer.AddTime(1);

	if (Key::TriggerKey(DIK_T)) {
		animation_.timer.AddTime(1);
	}
}

void AssimpObject3D::ParseNodeHeirarchy(const float currentTime, const uint32_t index, const Matrix4& parentMat, const aiNode* rootNode)
{
	aiMatrix4x4 aiMat = rootNode->mTransformation;

	Matrix4 currentPoseMat = AssimpLoader::ConvertAiMatrixToMatrix(aiMat).Transpose();

	// �m�[�h�A�j���[�V�������擾����
	std::string nodeName = rootNode->mName.C_Str();
	aiAnimation* animation = model_->scene->mAnimations[index];  // �K�؂ȃA�j���[�V������I������K�v������܂�

	const aiNodeAnim* nodeAnim = FindNodeAnim(nodeName, animation);

	// �m�[�h�A�j���[�V����������ꍇ�A�m�[�h�̕ϊ��s���⊮����
	if (nodeAnim)
	{
		// �X�P�[�����O��⊮
		Vector3 scale = CalcCurrentScale(nodeAnim, currentTime);

		// ��]��⊮
		Quaternion rot = CalcCurrentRot(nodeAnim, currentTime);

		// ���W��⊮
		Vector3 pos = CalcCurrentPos(nodeAnim, currentTime);

		// �s��̍���
		currentPoseMat = CalculateWorldMat(pos, scale, rot);
	}

	Matrix4 globalTransformMat = currentPoseMat * parentMat;

	Vector3 wscale = worldTransform_.scale_;
	for (uint32_t i = 0; i < model_->bones.size(); i++)
	{
		if (model_->bones[i].name == nodeName)
		{
			//�X�P�[���𖳗���菬�������Ă���
			Matrix4 scale = {
				wscale.x,0,0,0,
				0,wscale.y,0,0,
				0,0,wscale.z,0,
				0,0,0,1.0f
			};
			Matrix4 initalMat = model_->bones[i].offsetMat;
			Matrix4 invWorldMat = AssimpLoader::ConvertAiMatrixToMatrix(model_->scene->mRootNode->mTransformation);
			model_->bones[i].currentMat = initalMat * globalTransformMat * invWorldMat * scale;
		}
	}

	for (uint32_t i = 0; i < rootNode->mNumChildren; i++)
	{
		ParseNodeHeirarchy(currentTime, index, globalTransformMat, rootNode->mChildren[i]);
	}
}

aiNodeAnim* AssimpObject3D::FindNodeAnim(const std::string& nodeName, aiAnimation* animation)
{
	for (unsigned int i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim* nodeAnim = animation->mChannels[i];
		if (nodeAnim->mNodeName.data == nodeName)
		{
			return nodeAnim;
		}
	}
	return nullptr;
}

uint32_t AssimpObject3D::FindScaleIndex(const aiNodeAnim* nodeAnim, const float currentTime)
{
	for (uint32_t i = 0; i < nodeAnim->mNumScalingKeys - 1; i++)
	{
		if (currentTime < nodeAnim->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}
	return 0;
}

uint32_t AssimpObject3D::FindRotIndex(const aiNodeAnim* nodeAnim, const float currentTime)
{
	for (uint32_t i = 0; i < nodeAnim->mNumRotationKeys - 1; i++)
	{
		if (currentTime >= nodeAnim->mRotationKeys[i].mTime &&
			currentTime < nodeAnim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}
	return 0;
}

uint32_t AssimpObject3D::FindPosIndex(const aiNodeAnim* nodeAnim, const float currentTime)
{
	for (uint32_t i = 0; i < nodeAnim->mNumPositionKeys - 1; i++)
	{
		if (currentTime >= nodeAnim->mRotationKeys[i].mTime &&
			currentTime < nodeAnim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}
	return 0;
}

Vector3 AssimpObject3D::CalcCurrentScale(const aiNodeAnim* nodeAnim, const float currentTime)
{
	Vector3 result;
	if (nodeAnim->mNumScalingKeys == 1)
	{
		result.x = (float)nodeAnim->mScalingKeys[0].mValue.x;
		result.y = (float)nodeAnim->mScalingKeys[0].mValue.y;
		result.z = (float)nodeAnim->mScalingKeys[0].mValue.z;

		return result;
	}
	else if (nodeAnim->mNumScalingKeys > 1)
	{
		uint32_t scaleIndex = FindScaleIndex(nodeAnim, currentTime);
		uint32_t scaleNextIndex = scaleIndex + 1;

		aiVectorKey curKey = nodeAnim->mScalingKeys[scaleIndex];
		aiVectorKey nextKey = nodeAnim->mScalingKeys[scaleNextIndex];

		float deltaTime = (float)(nextKey.mTime - curKey.mTime);
		float factor = (float)(currentTime - curKey.mTime) / deltaTime;

		Vector3 s =
		{
			nodeAnim->mScalingKeys[scaleIndex].mValue.x,
			nodeAnim->mScalingKeys[scaleIndex].mValue.y,
			nodeAnim->mScalingKeys[scaleIndex].mValue.z,
		};

		Vector3 e =
		{
			nodeAnim->mScalingKeys[scaleNextIndex].mValue.x,
			nodeAnim->mScalingKeys[scaleNextIndex].mValue.y,
			nodeAnim->mScalingKeys[scaleNextIndex].mValue.z,
		};

		result = Lerp(s, e, factor);
		return result;
	}

	return { 0,0,0 };
}

Quaternion AssimpObject3D::CalcCurrentRot(const aiNodeAnim* nodeAnim, const float currentTime)
{
	Quaternion result;
	if (nodeAnim->mNumRotationKeys == 1)
	{
		result.x = (float)nodeAnim->mRotationKeys[0].mValue.x;
		result.y = (float)nodeAnim->mRotationKeys[0].mValue.y;
		result.z = (float)nodeAnim->mRotationKeys[0].mValue.z;
		result.w = (float)nodeAnim->mRotationKeys[0].mValue.w;

		return result;
	}
	else if (nodeAnim->mNumRotationKeys > 1)
	{
		uint32_t rotIndex = FindRotIndex(nodeAnim, currentTime);
		uint32_t rotNextIndex = rotIndex + 1;

		aiQuatKey curKey = nodeAnim->mRotationKeys[rotIndex];
		aiQuatKey nextKey = nodeAnim->mRotationKeys[rotNextIndex];

		float deltaTime = (float)(nextKey.mTime - curKey.mTime);
		float factor = (float)(currentTime - curKey.mTime) / deltaTime;

		Quaternion s =
		{
			nodeAnim->mRotationKeys[rotIndex].mValue.x,
			nodeAnim->mRotationKeys[rotIndex].mValue.y,
			nodeAnim->mRotationKeys[rotIndex].mValue.z,
			nodeAnim->mRotationKeys[rotIndex].mValue.w,
		};

		Quaternion e =
		{
			nodeAnim->mRotationKeys[rotNextIndex].mValue.x,
			nodeAnim->mRotationKeys[rotNextIndex].mValue.y,
			nodeAnim->mRotationKeys[rotNextIndex].mValue.z,
			nodeAnim->mRotationKeys[rotNextIndex].mValue.w,
		};

		result = s.Slerp(e, factor);
		return result;
	}

	return Quaternion{};
}

Vector3 AssimpObject3D::CalcCurrentPos(const aiNodeAnim* nodeAnim, const float currentTime)
{
	Vector3 result;
	if (nodeAnim->mNumPositionKeys == 1)
	{
		result.x = (float)nodeAnim->mPositionKeys[0].mValue.x;
		result.y = (float)nodeAnim->mPositionKeys[0].mValue.y;
		result.z = (float)nodeAnim->mPositionKeys[0].mValue.z;

		return result;
	}
	else if (nodeAnim->mNumPositionKeys > 1)
	{
		uint32_t posIndex = FindPosIndex(nodeAnim, currentTime);
		uint32_t posNextIndex = posIndex + 1;

		aiVectorKey curKey = nodeAnim->mPositionKeys[posIndex];
		aiVectorKey nextKey = nodeAnim->mPositionKeys[posNextIndex];

		float deltaTime = (float)(nextKey.mTime - curKey.mTime);
		float factor = (float)(currentTime - curKey.mTime) / deltaTime;

		Vector3 s =
		{
			nodeAnim->mPositionKeys[posIndex].mValue.x,
			nodeAnim->mPositionKeys[posIndex].mValue.y,
			nodeAnim->mPositionKeys[posIndex].mValue.z,
		};

		Vector3 e =
		{
			nodeAnim->mPositionKeys[posNextIndex].mValue.x,
			nodeAnim->mPositionKeys[posNextIndex].mValue.y,
			nodeAnim->mPositionKeys[posNextIndex].mValue.z,
		};

		result = Lerp(s, e, factor);
		return result;
	}

	return { 0,0,0 };
}
