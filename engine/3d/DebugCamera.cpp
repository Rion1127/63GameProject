#include "DebugCamera.h"
#include <WinUser.h>
#include <imgui.h>

DebugCamera::DebugCamera()
{
	mInput_ = MouseInput::GetInstance();

	camera_.SetEyePos(0, 0, -200);
	camera_.SetTarget(0, 0, 0);
	camera_.SetUpVec(0, 1, 0);

	frontVec_ = { 0, 0, 0 };
	sideVec_ = { 0,0,0 };

	frontdist_ = 50;
}

void DebugCamera::Update()
{
	if (Key::PushKey(DIK_LCONTROL))
	{
		//�}�E�X�̏��̍X�V
		CameraMove();
		camera_.Update(CameraMode::LookAT);
	}
	//ImGui::Begin("debugCamera");
	//float pos[3] = {
	//	camera_.up_.x,
	//	camera_.up_.y,
	//	camera_.up_.z,
	//};
	///*static float target[3] = {
	//	viewProjection_.target_.x,
	//	viewProjection_.target_.y,
	//	viewProjection_.target_.z,
	//};*/
	//ImGui::SliderFloat3("up", pos, -200.f, 200.f);
	///*ImGui::SliderFloat3("target", target, -200.f, 200.f);*/


	//ImGui::End();

	/*viewProjection_.eye_.x = pos[0];
	viewProjection_.eye_.y = pos[1];
	viewProjection_.eye_.z = pos[2];

	viewProjection_.target_.x = target[0];
	viewProjection_.target_.y = target[1];
	viewProjection_.target_.z = target[2];*/
}

void DebugCamera::CameraMove()
{
	Vector3 proviUpVec = { 0,1,0 };

	float speedRate = frontdist_ * 0.002f;
	Vector2 speed = {
		mInput_->GetCursorMoveX() * speedRate,
		mInput_->GetCursorMoveY() * speedRate
	};
	//�J�����������_���W��艜�ɂ���Ƃ�
	if (camera_.up_.y <= 0) {
		speed *= -1;
	}

	//�J�����̐��ʃx�N�g��
	frontVec_ = camera_.target_ - camera_.eye_;
	frontVec_ = frontVec_.normalize();

	sideVec_ = proviUpVec.cross(frontVec_);

	upVec_ = sideVec_.cross(frontVec_);

	//���s�ړ�
	if (mInput_->IsMouseDown(MOUSE_WHEEL)) {
		//�}�E�X�J�[�\�������E�ɓ��������Ƃ�
		cameraTrans_ -= sideVec_ * speed.x;
		camera_.target_ -= sideVec_ * speed.x;
		//�㉺�ɓ��������Ƃ�
		cameraTrans_ -= upVec_ * speed.y;
		camera_.target_ -= upVec_ * speed.y;
	}
	//�g��k��
	else if (!mInput_->IsMouseDown(MOUSE_WHEEL)) {
		frontdist_ += -(float)mInput_->IsMouseWheel() * (frontdist_ * 0.001f);
	}
	//���ʍ��W�ړ�
	if (mInput_->IsMouseDown(MOUSE_LEFT)) {
		//�J��������������Ă�Ƃ��ʏ�ʂ�ɍ��W�𑫂�
		Vector3 moveDistVec{};
		if (camera_.up_.y >= 0) {
			moveDistVec += mInput_->GetCursorMove() * 0.005f;
		}
		//�J�������t���܂ɂȂ�����X.Z���W�𔽓]������
		else {
			moveDistVec = {
				-mInput_->GetCursorMoveX() * 0.005f,
				mInput_->GetCursorMoveY() * 0.005f,
				-mInput_->GetCursorMoveZ() * 0.005f
			};

		}
		moveDist_ += moveDistVec;
	}

	//�J����up_�ϊ�
	camera_.up_ = {
		0,
		cosf(moveDist_.y),
		0
	};

	Vector3 cameraPos = {
		-frontdist_ * sinf(moveDist_.x) * cosf(moveDist_.y) + cameraTrans_.x,
		frontdist_ * sinf(moveDist_.y) + cameraTrans_.y,
		-frontdist_ * cosf(moveDist_.x) * cosf(moveDist_.y) + cameraTrans_.z
	};

	camera_.eye_ = cameraPos;
}
