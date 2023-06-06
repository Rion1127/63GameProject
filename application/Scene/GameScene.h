#pragma once
#include <memory>
#include "Model.h"
#include "Sprite.h"
#include "Particle.h"
#include "WorldTransform.h"
#include "WinAPI.h"
#include "DirectX.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"
#include "Texture.h"
#include "EmptyScene.h"
#include <sstream>
#include <fstream>
#include "LightGroup.h"
#include "Object3d.h"

#include "AssimpObject3D.h"
#include "AssimpLoader.h"

class GameScene :
	public EmptyScene
{
public:
	~GameScene();

	void Ini()override;

	void Update()override;

	void Draw()override;
private:
	DirectXCommon* directX = nullptr;
	WinAPI* winApi_ = nullptr;
	DirectXInput* input_ = nullptr;
	Controller* controller_ = nullptr;
	TextureManager* textureM = nullptr;
	SoundManager* sound_ = nullptr;
	DebugCamera debugCamera;
	
	void CheckAllCollision();	//すべての当たり判定

	Camera gameCamera;
	void cameraUpdate();
	//使うカメラ座標
	Camera* useVP = nullptr;

	LightGroup* lightGroup = nullptr;


	std::unique_ptr<Object3d> skyDome_;

	std::unique_ptr<Object3d> sphere_;


	std::unique_ptr<Object3d> testObj;

	std::vector<Mesh> meshes;
	std::vector <uint32_t> texHandle_;
	std::unique_ptr<ImportSettings> importSetting_;

	WorldTransform testWT_;

	AssinpModel testModel_;

	AssimpObject3D assimpObj_;

	float ambientColor0[3] = { 1,1,1 };

	//光線方向初期化値
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	//点光源
	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };

	//スポットライト
	float spotlightDir[3] = { 0,-1,0 };
	float spotlightPos[3] = { 0,5,0 };
	float spotLightColor[3] = { 1,0,1 };
	float spotLightAtten[3] = { 0.0f,0.0f, 0.0f };
	float spotLightFactorAngle[2] = { 20.f,30.f };

	//丸影
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f,0.5f };

	float fighterPos[3] = { 1,0.0f,0 };
	enum LIGHTTYPE {
		DIRECTION_,
		POINT_,
		SPOT_,
		NUMEND_
	};
	//ライトのタイプ
	int lightType_ = DIRECTION_;

	void DirectionalLightUpdate();
	void PointLightUpdate();
	void SpotLightUpdate();

	SoundKey gameBGM_;

	Sprite testSprite_;
	Sprite testSprite2_;
	uint32_t uvtexture_;
};

