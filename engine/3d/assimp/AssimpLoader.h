#pragma once
#include "REngine.h"
#include "myMath.h"
#include "AssimpModel.h"

struct aiMesh;
struct aiMaterial;
struct aiBone;

struct Mesh {
	Vertices Vertices; // 頂点データの配列
	std::wstring diffuseMap; // テクスチャのファイルパス
};

struct ImportSettings // インポートするときのパラメータ
{
public:
	
	const wchar_t* filename = nullptr; // ファイルパス
	std::vector<Mesh>& meshes; // 出力先のメッシュ配列
	bool inverseU = false; // U座標を反転させるか
	bool inverseV = false; // V座標を反転させるか


};

class AssimpLoader
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static AssimpLoader* GetInstance();

	bool Load(ImportSettings* setting); // モデルをロードする
	

	std::unique_ptr<AssimpModel> Load(std::string fileName);
private:
	void LoadVertices(Vertices* vert, const aiMesh* aimesh);
	void LoadMaterial(std::string fileName,Material* material, const aiMaterial* aimaterial);
	void LoadSkin(AssimpModel model, const aiMesh* aimesh);
	AssimpLoader(){};
	void LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV);
	void LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src);

	void LoadBones(Mesh& dst,const aiMesh* pMesh);
private:
	
};