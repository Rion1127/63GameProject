#pragma once
#include "REngine.h"
#include "myMath.h"
#include "AssimpModel.h"
#include <string>
#include <assimp/matrix4x4.h>

struct aiMesh;
struct aiMaterial;
struct aiNode;

struct Mesh {
	Vertices Vertices; // 頂点データの配列
	std::wstring diffuseMap; // テクスチャのファイルパス
};

class AssimpLoader
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static AssimpLoader* GetInstance();

	std::unique_ptr<AssimpModel> Load(std::string fileName, AssimpModel* model);
	static Matrix4 ConvertAiMatrixToMatrix(const aiMatrix4x4 aimat);
private:
	void LoadVertices(Vertices* vert, const aiMesh* aimesh);
	void LoadMaterial(std::string fileName,Material* material, const aiMaterial* aimaterial);
	void LoadSkin(AssimpModel* model, const aiMesh* aimesh);
	void LoadNode(AssimpModel* model, Node* parent, const aiNode* node);
	AssimpLoader(){};
private:
};

