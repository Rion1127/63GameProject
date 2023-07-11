#pragma once

#include <d3d12.h>

#include "DirectX.h"
#include "myMath.h"
#include "WorldTransform.h"

// �萔�o�b�t�@�p�f�[�^�\����
struct ConstVPBuff {
	Matrix4 view;       // ���[���h �� �r���[�ϊ��s��
	Matrix4 projection; // �r���[ �� �v���W�F�N�V�����ϊ��s��
	Vector3 cameraPos;  // �J�������W�i���[���h���W�j
};

enum class CameraMode {
	LookAT,	//���W�ƒ����_���W
	LookTo	//���W�ƃI�C���[�p
};

class Camera {
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	Camera();
	//�J�������W
	void SetEyePos(float x, float y, float z);
	void SetEyePos(Vector3 pos);
	//�^�[�Q�b�g�Z�b�g
	void SetTarget(float x, float y, float z);
	void SetTarget(Vector3 pos);
	//�A�b�v�x�N�g��
	void SetUpVec(float x, float y, float z);
	void SetUpVec(Vector3 upVec);
	void MoveTo(Vector3 goal, float speed);
	//���W�ƒ����_���W�ōs��v�Z
	void UpdateLookAt();
	//���W�ƃI�C���[�p�ōs��v�Z(�������̓f�o�b�O�J�������Ή����Ă��Ȃ����߁A�o�O��)
	void UpdateLookTo();

	void Update(CameraMode mode);

	Vector3 eye_;
	Vector3 target_;
	Vector3 up_;
	Vector3 rot_;

	WorldTransform WT_;

	Matrix4 matView_{};
	//�������e�s��̌v�Z
	Matrix4 matProjection_{};
	//�r���{�[�h�s��
	Matrix4 matBillboard_;
	//Y������̃r���{�[�h
	Matrix4 matBillboardY_;

	static Camera* scurrent_;
public:
	/// <summary>
	/// �J�����V�F�C�N
	/// </summary>
	/// <param name="time">�h��鎞��</param>
	/// <param name="power">�h���傫��</param>
	void ShakeSet(uint32_t time, float power);
	void ShakeUpdate();
	void SetOriginalPos();
	bool GetIsShake() { return isShake_; }
public:
	Matrix4 GetMatView();
	Matrix4 GetMatProjection();

private:
	void UpdateMatProjection();
	// �r���[�|�[�g�̃A�X�y�N�g��
	float aspectRatio_;

	//�V�F�C�N����O�̏ꏊ
	Vector3 originalPos_;
	bool isShake_ = false;
	uint32_t maxShakeTime_;
	uint32_t shakeTime_ = 0;
	float power_;
};