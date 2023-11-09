#pragma once
#include "IPostEffect.h"

/**
 * @file LineBlur.h
 * @brief ラインブラーを掛けるクラス
 */

class LineBlur final :
	public IPostEffect
{
private:
	struct ConstBuffRad {
		float radian;
	};

	float radian_;
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//定数バッファ用データ構造体

	ConstBuffRad* constMap_;
protected:
	//シェーダーへ値を渡す
	void TransferBuff() override;
	void SendToShader() override;
public:
	LineBlur();
	void SetRadian(float rad) { radian_ = rad; }
};

