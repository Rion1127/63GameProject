#pragma once
class GameSpeed
{
private:
	static float gameSpeed_;
public:
	static void SetGameSpeed(float speed) { gameSpeed_ = speed; }
	static float GetGameSpeed() { return gameSpeed_; }
};

