#pragma once
#include <memory>
#include "Object3d.h"
#include "Sprite.h"
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
#include "ParticleManager.h"
#include "AssimpLoader.h"

#include "AssimpObject3D.h"
class ParticleScene final:
	public EmptyScene
{
public:
	~ParticleScene();

	void Ini()override;

	void Update()override;

	void Draw()override;
private:
	Controller* controller_ = nullptr;
	TextureManager* textureM = nullptr;
	SoundManager* sound_ = nullptr;
	DebugCamera debugCamera;

	Camera gameCamera;
	//égÇ§ÉJÉÅÉâç¿ïW
	Camera* useVP = nullptr;

	WorldTransform WT_;

	uint32_t texture_;
	uint32_t whiteTexture_;


	std::shared_ptr<LightGroup> lightGroup = nullptr;

	
	std::vector<Mesh> meshes;
	std::vector <uint32_t> texHandle_;

	WorldTransform testWT_;

	AssimpModel testModel_;

	AssimpObject3D assimpObj_;
};