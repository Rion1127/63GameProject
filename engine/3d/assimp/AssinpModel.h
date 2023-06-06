#pragma once
#include "AssimpLoader.h"
#include "WorldTransform.h"
class AssinpModel
{
public:
	AssinpModel();
	void Create(const wchar_t* modelFile);

	void Draw(WorldTransform WT);
private:
	std::vector<Mesh> meshes;
	std::vector <uint32_t> texHandle_;
	std::unique_ptr<ImportSettings> importSetting_;

	


	WorldTransform WorldTransform_;
public:
	static const int MAX_BONES = 32;

	struct ConstBufferDataSkin {
		DirectX::XMMATRIX bones[MAX_BONES];
	};
};

