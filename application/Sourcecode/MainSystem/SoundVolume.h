#pragma once
class SoundVolume
{
private:
	static float volumeBgm_;
	static float volumeSe_;
public:
	SoundVolume();
	static void SetValumeBGM(float volume) { volumeBgm_ = volume; }
	static void SetValumeSE(float volume) { volumeSe_ = volume; }
public:
	static float GetValumeBGM() { return volumeBgm_; }
	static float GetValumeSE() { return volumeSe_; }
};

