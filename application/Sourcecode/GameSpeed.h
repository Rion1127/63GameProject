#pragma once
class GameSpeed
{
private:
	static float gameSpeed_;
	static float playerSpeed_;
	static float enemySpeed_;

	static float resultPlayerSpeed_;
	static float resultEnemySpeed_;
public:
	static void Update();
public:
	static void SetGameSpeed(float speed) { gameSpeed_ = speed; }
	static void SetPlayerSpeed(float speed) { playerSpeed_ = speed; }
	static void SetEnemySpeed(float speed) { enemySpeed_ = speed; }
	static float GetGameSpeed() { return gameSpeed_; }
	static float GetPlayerSpeed() { return resultPlayerSpeed_; }
	static float GetEnemySpeed() { return resultEnemySpeed_; }
};

