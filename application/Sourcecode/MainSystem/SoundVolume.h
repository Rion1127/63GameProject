#pragma once
class SoundVolume
{
private:
	float volumeBgm_;
	float volumeSe_;
public:
	SoundVolume();
	void SetValumeBGM(float volume) { volumeBgm_ = volume; }
	void SetValumeSE(float volume) { volumeSe_ = volume; }
};

