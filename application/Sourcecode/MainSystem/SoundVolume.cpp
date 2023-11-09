#include "SoundVolume.h"

/**
 * @file SoundVolume.cpp
 * @brief ゲーム全体の音量を管理しているクラス
 */

float SoundVolume::kVolumeBgm_ = 1.0f;
float SoundVolume::kVolumeSe_ = 1.0f;

SoundVolume::SoundVolume()
{
	kVolumeBgm_ = 1.0f;
	kVolumeSe_ = 1.0f;
}
