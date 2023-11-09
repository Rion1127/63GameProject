#pragma once
#include <list>
#include <unordered_map>
#include <memory>

/**
 * @file StateMachine.h
 * @brief ステートマシンについてまとめたクラス
 */

template<typename T>
class State
{
private:
	T id_;
public:

	/// <summary>
	/// 新しい状態を作成します。
	/// </summary>
	/// <param name="id"> 状態ID </param>
	State(T id) :id_(id) {}

	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	virtual ~State() = default;

	/// <summary>
	/// 状態IDを取得します。
	/// </summary>
	const T& GetId() { return id_; }

	/// <summary>
	/// 状態に入ったときに呼ばれます。
	/// </summary>
	virtual void SetUp() = 0;

	/// <summary>
	/// 情報を更新します。
	/// </summary>
	/// <remarks> 毎フレーム呼ばれます。 </remarks>
	virtual void Update() = 0;

	/// <summary>
	/// 描画を更新します。
	/// </sumamry>
	/// <remarks> 毎フレーム呼ばれます。 </remarks>
	virtual void Draw() = 0;

	/// <summary>
	/// 次の状態に移る前に呼ばれます。
	/// </summary>
	virtual void CleanUp() = 0;
};

template<typename T>
class StateMachine
{
private:
	/// <summary>
	/// 状態リスト
	/// </summary>
	std::unordered_map<T, std::shared_ptr<State<T>>> stateList_;

	/// <summary>
	/// 現在の状態
	/// </summary>
	std::shared_ptr<State<T>> state_;
public:

	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	StateMachine() = default;

	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	virtual ~StateMachine() = default;

	/// <summary>
	/// 現在の状態のIDを返します。
	/// </summary>
	/// <returns> 現在の状態のID </returns>
	const T GetCurrentStateName() const {
		if (state_ == nullptr) {
			return 0;
		}
		return state_->GetId();
	}

	/// <summary>
	/// 指定の状態へ移行します。
	/// </summary>
	/// <param name="nextStateId"> 移行先の状態ID </param>
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
	/// ステートマシンの初期化を行います。
	/// </summary>
	/// <remarks> 必ずオーバーライドします。 </remarks>
	virtual void InitStateMachine() = 0;

	/// <summary>
	/// 状態を追加します。
	/// </summary>
	/// <param name="state"> 追加する状態のstd::make_shared</param>
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
	/// 毎フレーム呼ばれます。情報を更新します。
	/// </summary>
	void Update() {
		if (state_ == nullptr) {
			return;
		}
		state_->Update();
	}

	/// <summary>
	/// 毎フレーム呼ばれます。描画を更新します。
	/// </summary>
	void Draw() const {
		if (state_ == nullptr) {
			return;
		}
		state_->Draw();
	}
public:
	/// <summary>
	/// 現在のステータスを返す
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<State<T>> GetNowState() { return state_; }
};