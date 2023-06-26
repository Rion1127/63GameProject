#pragma once
#include <random>
class RRandom
{
private:
	//乱数シード生成器
	static std::random_device seed_gen;
	////メルセンヌ・ツイスター
	static std::mt19937_64 engine;
public:
	static float RandF(float min, float max);
	static int32_t Rand(int32_t min, int32_t max);

};

