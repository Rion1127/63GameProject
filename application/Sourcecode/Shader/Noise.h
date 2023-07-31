#pragma once
#include "DirectX.h"
#include "IPostEffect.h"
class Noise final:
	public IPostEffect {
private:
	struct ConstBuffTime {
		int32_t time = 0;
		float power;
		float width;
	};

	int32_t time_;
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//定数バッファ用データ構造体
	
	ConstBuffTime* constMap_;
protected:
	//シェーダーへ値を渡す
	void TransferBuff() override;
	void SendToShader() override;
public:
	Noise();
	void PreDraw();
};