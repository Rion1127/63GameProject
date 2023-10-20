#pragma once

/**
 * @file GameSpeed.h
 * @brief ゲームの速度を管理するクラス
 * @brief 主にスローモーションで利用する
 */

class GameSpeed
{
private:
	static float kGameSpeed_;
	static float kPlayerSpeed_;
	static float kEnemySpeed_;
	static float kResultPlayerSpeed_;
	static float kResultEnemySpeed_;
public:
	static void Update();
public:
	static void SetGameSpeed(float speed) { kGameSpeed_ = speed; }
	static void SetPlayerSpeed(float speed) { kPlayerSpeed_ = speed; }
	static void SetEnemySpeed(float speed) { kEnemySpeed_ = speed; }
	static float GetGameSpeed() { return kGameSpeed_; }
	static float GetPlayerSpeed() { return kResultPlayerSpeed_; }
	static float GetEnemySpeed() { return kResultEnemySpeed_; }
};

