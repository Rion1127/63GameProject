#include <DirectXMath.h>
#include "Easing.h"

/// 元のコード
//t 経過時間    b最初の位置    c移動量    d移動時間 
#pragma region Back

float Easing::Back::easeIn(float t, float b, float c, float d)
{
	float s = 1.70158f;
	float postFix = t /= d;
	return c * (postFix)*t * ((s + 1) * t - s) + b;
}

float Easing::Back::easeOut(float t, float b, float c, float d)
{
	float s = 1.70158f;
	return c * ((t = t / d - 1) * t * ((s + 1) * t + s) + 1) + b;
}

float Easing::Back::easeInOut(float t, float b, float c, float d)
{
	float s = 1.70158f;
	if ((t /= d / 2) < 1) return c / 2 * (t * t * (((s *= (1.525f)) + 1) * t - s)) + b;
	float postFix = t -= 2;
	return c / 2 * ((postFix)*t * (((s *= (1.525f)) + 1) * t + s) + 2) + b;
}
#pragma endregion

#pragma region Bounce
//float Easing::Bounce::easeIn(float t, float b, float c, float d)
//{
//	return c - Easing::Bounce::easeOut(d - t, 0, c, d) + b;
//}
//
//float Easing::Bounce::easeOut(float t, float b, float c, float d)
//{
//	if ((t /= d) < (1 / 2.75f)) {
//		return c * (7.5625f * t * t) + b;
//	}
//	else if (t < (2 / 2.75f)) {
//		float postFix = t -= (1.5f / 2.75f);
//		return c * (7.5625f * (postFix)*t + .75f) + b;
//	}
//	else if (t < (2.5 / 2.75)) {
//		float postFix = t -= (2.25f / 2.75f);
//		return c * (7.5625f * (postFix)*t + .9375f) + b;
//	}
//	else {
//		float postFix = t -= (2.625f / 2.75f);
//		return c * (7.5625f * (postFix)*t + .984375f) + b;
//	}
//}
//
//float Easing::Bounce::easeInOut(float t, float b, float c, float d)
//{
//	if (t < d / 2) return Easing::Bounce::easeIn(t * 2, 0, c, d) * .5f + b;
//	else return Easing::Bounce::easeOut(t * 2 - d, 0, c, d) * .5f + c * .5f + b;
//}
#pragma endregion

#pragma region Circ
float Easing::Circ::easeIn(float t, float b, float c, float d)
{
	return -c * (float)(sqrt(1 - (t /= d) * t) - 1) + b;
}

float Easing::Circ::easeOut(float t, float b, float c, float d)
{
	return c * (float)sqrt(1 - (t = t / d - 1) * t) + b;
}

float Easing::Circ::easeInOut(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return -c / 2 * ((float)sqrt(1 - t * t) - 1) + b;
	return c / 2 * ((float)sqrt(1 - t * (t -= 2)) + 1) + b;
}
#pragma endregion

#pragma region Cubic
float Easing::Cubic::easeIn(float t, float b, float c, float d)
{
	return c * (t /= d) * t * t + b;
}

float Easing::Cubic::easeOut(float t, float b, float c, float d)
{
	return c * ((t = t / d - 1) * t * t + 1) + b;
}

float Easing::Cubic::easeInOut(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return c / 2 * t * t * t + b;
	return c / 2 * ((t -= 2) * t * t + 2) + b;
}
#pragma endregion

/// 改変後コード

#pragma region Back
float Easing::Back::easeIn(float start, float end, float t)
{
	float c1 = 1.70158f;
	float c3 = c1 + 1.0f;
	float time = (c3 * t * t * t - c1 * t * t);
	return start * (1.0f - time) + end * time;
}

float Easing::Back::easeOut(float start, float end, float t)
{
	float c1 = 1.70158f;
	float c3 = c1 + 1.0f;
	float time = 1 + c3 * (float)pow(t - 1, 3) + c1 * (float)pow(t - 1, 2);
	return start * (1.0f - time) + end * time;
}

float Easing::Back::easeInOut(float start, float end, float t)
{
	float c1 = 1.70158f;
	float c2 = c1 * 1.525f;
	float time = 0.0f;
	if (t < 0.5f) time = ((float)pow(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) / 2;
	else time = ((float)pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2) / 2;
	return start * (1.0f - time) + end * time;
}
#pragma endregion

#pragma region Bounce
float Easing::Bounce::easeIn(float t, float b, float c, float d)
{
	return c - Easing::Bounce::easeOut(d - t, 0, c, d) + b;
}

float Easing::Bounce::easeOut(float t, float b, float c, float d)
{
	if ((t /= d) < (1 / 2.75f)) {
		return c * (7.5625f * t * t) + b;
	}
	else if (t < (2 / 2.75f)) {
		float postFix = t -= (1.5f / 2.75f);
		return c * (7.5625f * (postFix)*t + .75f) + b;
	}
	else if (t < (2.5 / 2.75)) {
		float postFix = t -= (2.25f / 2.75f);
		return c * (7.5625f * (postFix)*t + .9375f) + b;
	}
	else {
		float postFix = t -= (2.625f / 2.75f);
		return c * (7.5625f * (postFix)*t + .984375f) + b;
	}
}

float Easing::Bounce::easeInOut(float t, float b, float c, float d)
{
	if (t < d / 2) return Easing::Bounce::easeIn(t * 2, 0, c, d) * .5f + b;
	else return Easing::Bounce::easeOut(t * 2 - d, 0, c, d) * .5f + c * .5f + b;
}
#pragma endregion

#pragma region Circ
float Easing::Circ::easeIn(float start, float end, float t)
{
	float time = 1.0f - (float)sqrt(1 - (float)pow(t, 2));
	return start * (1.0f - time) + end * time;
}

float Easing::Circ::easeOut(float start, float end, float t)
{
	float time = (float)sqrt(1 - (float)pow(t - 1, 2));
	return start * (1.0f - time) + end * time;
}

float Easing::Circ::easeInOut(float start, float end, float t)
{
	float time;
	if (t < 0.5f) time = 16.0f * t * t * t * t * t;
	else time = 1.0f - (float)pow(-2.0f * t + 2.0f, 5) / 2.0f;
	return start * (1.0f - time) + end * time;
}
#pragma endregion

#pragma region Quint
float Easing::Quint::easeIn(float start, float end, float t)
{
	float time = t * t * t * t * t;
	return start * (1.0f - time) + end * time;
}

float Easing::Quint::easeOut(float start, float end, float t)
{
	float time = 1.0f - (float)pow(1 - t, 5);
	return start * (1.0f - time) + end * time;
}

float Easing::Quint::easeInOut(float start, float end, float t)
{
	float time;
	if (t < 0.5f) time = 4.0f * t * t * t * t * t;
	else time = 1.0f - (float)pow(-2 * t + 2, 5) / 2;
	return start * (1.0f - time) + end * time;
}
#pragma endregion

#pragma region Cubic
float Easing::Cubic::easeIn(float start, float end, float t)
{
	float time = t * t * t;
	return start * (1.0f - time) + end * time;
}

float Easing::Cubic::easeOut(float start, float end, float t)
{
	float time = 1.0f - (float)pow(1 - t, 3);
	return start * (1.0f - time) + end * time;
}

float Easing::Cubic::easeInOut(float start, float end, float t)
{
	float time;
	if (t < 0.5f) time = 4.0f * t * t * t;
	else time = 1.0f - (float)pow(-2 * t + 2, 3) / 2;
	return start * (1.0f - time) + end * time;
}
#pragma endregion
