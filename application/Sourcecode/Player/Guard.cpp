#include "Guard.h"
#include "Player.h"
#include "mInput.h"
#include "Util.h"

Guard::Guard()
{
	col_.radius = 0.7f;
	colObj_ = MakeUnique<Object3d>();
	colObj_->SetModel(Model::CreateOBJ_uniptr("sphere", false));
	colObj_->SetAmbient("sphere", { 0.0f, 0, 1.0f });

	timer_.SetLimitTime(40);
	timer_.SetTime(timer_.GetLimitTimer());
	isGurdNow_ = false;

	enabledTime_ = { 5,30 };
}

void Guard::Init()
{
	const auto& enemy = player_->GetAttackManager()->GetLockOnEnemy();
	Vector3 frontVec;
	Vector3 lockOnVec;
	Vector2 frontVec2;
	frontDist_ = 0.8f;
	if (enemy != nullptr) {
		//�G���n�[�h���b�N���Ă���Ȃ�G�̕����Ɍ���
		if (enemy->GetIsHardLockOn()) {
			frontVec = player_->GetWorldTransform()->position_;

			lockOnVec = player_->GetLockOnVec();
			//���b�N�I�����Ă���G�ւ̃x�N�g�����Ƃ�
			frontVec2 = {
				lockOnVec.x,
				lockOnVec.z
			};
			float rotY = Radian(Vec2Angle(frontVec2));
			player_->SetRot({ 0,rotY,0 });

			//��]��񂩂琳�ʃx�N�g��(2D)���擾
			frontVec = {
				sinf(player_->GetWorldTransform()->rotation_.y),
				0,
				cosf(player_->GetWorldTransform()->rotation_.y),
			};

			frontVec = frontVec.normalize();
			frontVec = frontVec * frontDist_;
			frontVec.y = 0;
		}
		else {
			//��]��񂩂琳�ʃx�N�g��(2D)���擾
			frontVec = {
				sinf(player_->GetWorldTransform()->rotation_.y),
				0,
				cosf(player_->GetWorldTransform()->rotation_.y),
			};
			//���b�N�I�����Ă���G�ւ̃x�N�g�����Ƃ�
			frontVec2 = {
				frontVec.x,
				frontVec.z
			};
		}
	}
	else {
		//��]��񂩂琳�ʃx�N�g��(2D)���擾
		frontVec = {
			sinf(player_->GetWorldTransform()->rotation_.y),
			0,
			cosf(player_->GetWorldTransform()->rotation_.y),
		};
		//���b�N�I�����Ă���G�ւ̃x�N�g�����Ƃ�
		frontVec2 = {
			frontVec.x,
			frontVec.z
		};
	}
	
	col_.center = player_->GetCol().center + frontVec;
	col_.center.y += colObj_->GetTransform()->scale_.y;

	colObj_->GetTransform()->SetPosition(col_.center);
	Vector3 scale = { col_.radius ,col_.radius ,col_.radius };
	colObj_->GetTransform()->SetScale(scale);

	timer_.Reset();
	isGurdNow_ = true;
}

void Guard::Update()
{
	//enabledTime_�̊Ԃ������瓖���蔻���L���ɂ���
	if (enabledTime_.start < timer_.GetTimer() &&
		enabledTime_.end > timer_.GetTimer()) {
		col_.isActive = true;
	}
	else {
		col_.isActive = false;
	}
	
	Vector3 frontVec = player_->GetWorldTransform()->position_;
	//��]��񂩂琳�ʃx�N�g��(2D)���擾
	frontVec = {
		sinf(player_->GetWorldTransform()->rotation_.y),
		0,
		cosf(player_->GetWorldTransform()->rotation_.y),
	};

	frontVec = frontVec.normalize();
	frontVec = frontVec * frontDist_;
	frontVec.y = 0;
	col_.center = player_->GetCol().center + frontVec;
	col_.center.y += colObj_->GetTransform()->scale_.y;
	colObj_->GetTransform()->SetPosition(col_.center);

	timer_.AddTime(1);
	if (timer_.GetIsEnd()) {
		isGurdNow_ = false;
	}
	colObj_->Update();
}

void Guard::DrawDebug()
{
	if (col_.isActive) {
		colObj_->Draw();
	}
}

void Guard::GuardHit()
{
	//�K�[�h������������K�[�h���Ԃ�����
	timer_.Reset();
	timer_.SetTime(enabledTime_.start);
	Init();
}
