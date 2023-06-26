#include "RRandom.h"

//�����V�[�h������
std::random_device RRandom::seed_gen;
std::mt19937_64 RRandom::engine(seed_gen());

float RRandom::RandF(float min, float max)
{
	//�����Z���k�E�c�C�X�^�[
	std::uniform_real_distribution<float> randf(min, max);
	return randf(engine);
}

int32_t RRandom::Rand(int32_t min, int32_t max)
{
	//�����Z���k�E�c�C�X�^�[
	std::uniform_int_distribution<int32_t> r(min, max);
	return r(engine);
}
