#pragma once
class SoundVolume
{
private:
	static float kVolumeBgm_;
	static float kVolumeSe_;
public:
	SoundVolume();
	static void SetValumeBGM(float volume) { kVolumeBgm_ = volume; }
	static void SetValumeSE(float volume) { kVolumeSe_ = volume; }
public:
	static float GetValumeBGM() { return kVolumeBgm_; }
	static float GetValumeSE() { return kVolumeSe_; }
};

