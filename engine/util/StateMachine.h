#pragma once
#include <list>
#include <unordered_map>
#include <memory>

template<typename T>
class State
{
private:
	T id_;
public:

	/// <summary>
	/// �V������Ԃ��쐬���܂��B
	/// </summary>
	/// <param name="id"> ���ID </param>
	State(T id) :id_(id) {}

	/// <summary>
	/// �f�t�H���g�f�X�g���N�^
	/// </summary>
	virtual ~State() = default;

	/// <summary>
	/// ���ID���擾���܂��B
	/// </summary>
	const T& GetId() { return id_; }

	/// <summary>
	/// ��Ԃɓ������Ƃ��ɌĂ΂�܂��B
	/// </summary>
	virtual void SetUp() = 0;

	/// <summary>
	/// �����X�V���܂��B
	/// </summary>
	/// <remarks> ���t���[���Ă΂�܂��B </remarks>
	virtual void Update() = 0;

	/// <summary>
	/// �`����X�V���܂��B
	/// </sumamry>
	/// <remarks> ���t���[���Ă΂�܂��B </remarks>
	virtual void Draw() = 0;

	/// <summary>
	/// ���̏�ԂɈڂ�O�ɌĂ΂�܂��B
	/// </summary>
	virtual void CleanUp() = 0;
};

template<typename T>
class StateMachine
{
private:
	/// <summary>
	/// ��ԃ��X�g
	/// </summary>
	std::unordered_map<T, std::shared_ptr<State<T>>> stateList_;

	/// <summary>
	/// ���݂̏��
	/// </summary>
	std::shared_ptr<State<T>> state_;
public:

	/// <summary>
	/// �f�t�H���g�R���X�g���N�^
	/// </summary>
	StateMachine() = default;

	/// <summary>
	/// �f�t�H���g�f�X�g���N�^
	/// </summary>
	virtual ~StateMachine() = default;

	/// <summary>
	/// ���݂̏�Ԃ�ID��Ԃ��܂��B
	/// </summary>
	/// <returns> ���݂̏�Ԃ�ID </returns>
	const T GetCurrentStateName() const {
		if (state_ == nullptr) {
			return 0;
		}
		return state_->GetId();
	}

	/// <summary>
	/// �w��̏�Ԃֈڍs���܂��B
	/// </summary>
	/// <param name="nextStateId"> �ڍs��̏��ID </param>
	void GoToState(T nextStateId) {

		if (!stateList_.contains(nextStateId)) {
			//Print << U"Error: Not exist state: " << nextStateId;
			return;
		}

		if (state_ != nullptr) {
			state_->CleanUp();
		}

		state_ = stateList_[nextStateId];
		state_->SetUp();
	}

	/// <summary>
	/// �X�e�[�g�}�V���̏��������s���܂��B
	/// </summary>
	/// <remarks> �K���I�[�o�[���C�h���܂��B </remarks>
	virtual void InitStateMachine() = 0;

	/// <summary>
	/// ��Ԃ�ǉ����܂��B
	/// </summary>
	/// <param name="state"> �ǉ������Ԃ�std::make_shared</param>
	void AddState(const std::shared_ptr<State<T>>& state) {
		if (state == nullptr) {
			//Print << U"Error: This state is nullptr";
			return;
		}
		if (stateList_.contains(state->GetId())) {
			// Print << U"Error: Already exist state: " << state->GetId();
			return;
		}
		stateList_[state->GetId()] = state;
	}

	/// <summary>
	/// ���t���[���Ă΂�܂��B�����X�V���܂��B
	/// </summary>
	void Update() {
		if (state_ == nullptr) {
			return;
		}
		state_->Update();
	}

	/// <summary>
	/// ���t���[���Ă΂�܂��B�`����X�V���܂��B
	/// </summary>
	void Draw() const {
		if (state_ == nullptr) {
			return;
		}
		state_->Draw();
	}
public:
	std::shared_ptr<State<T>> GetNowState() { return state_; }
};