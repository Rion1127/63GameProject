#pragma once
#include "Framework.h"
#include "mInput.h"
#include "Texture.h"
#include "Util.h"
#include "WinAPI.h"
#include "DirectX.h"
#include "GameScene.h"
#include "mSound.h"
#include "SceneManager.h"
#include "PipelineManager.h"
#include "Sprite.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include "LoadManager.h"
#include "MultipleRenderTarget.h"
#include "MultiTexture.h"
#include "Bloom.h"
#include "Noise.h"
#include "GaussianBlur.h"
#include "CrossFilter.h"

enum class PostEffectName {
	None,
	MultiRenderTexture,
	Gaussian,
	RadialBlur,
	Bloom,
	Noise,
	CrossFilter,
	END
};

class Framework
{
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	LoadManager loadManager_;
	
	std::unique_ptr<Bloom> bloom_;
	std::unique_ptr<Noise> noise_;
	std::unique_ptr<GaussianBlur> gaussianBlur_;
	std::unique_ptr<RadialBlur> radialBlur_;
	std::unique_ptr<CrossFilter> crossFilter_;
	std::unique_ptr<MultiTexture> multiRenderTexture_;
	size_t postEffectnum = 0;
public:
	virtual ~Framework() = default;

	virtual void Init();

	virtual void Finalize();

	virtual void Update();

	virtual void Draw() = 0;

	//実行
	void Run();
};

