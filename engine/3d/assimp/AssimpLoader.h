#pragma once
#include "REngine.h"

struct aiMesh;
struct aiMaterial;
struct aiBone;

struct Mesh {
	Vertices Vertices; // 頂点データの配列
	std::wstring diffuseMap; // テクスチャのファイルパス
};

struct BoneData {
	static const uint32_t NUM_BONES_PER_VERTEX = 4;

	uint32_t IDs[NUM_BONES_PER_VERTEX];
	float Weights[NUM_BONES_PER_VERTEX];
	Matrix4 boneMatrix_;
};

struct ImportSettings // インポートするときのパラメータ
{
public:
	
	const wchar_t* filename = nullptr; // ファイルパス
	std::vector<Mesh>& meshes; // 出力先のメッシュ配列
	bool inverseU = false; // U座標を反転させるか
	bool inverseV = false; // V座標を反転させるか

	std::vector<BoneData> boneData;
};

class AssimpLoader
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static AssimpLoader* GetInstance();

	bool Load(ImportSettings* setting); // モデルをロードする

private:
	void LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV);
	void LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src);

	void LoadBones(uint32_t MeshIndex, const aiMesh* pMesh, ImportSettings* setting);
private:
	
};