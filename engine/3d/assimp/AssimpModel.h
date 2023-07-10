#pragma once
#include "WorldTransform.h"
#include "Texture.h"
#include "LightGroup.h"
#include <memory>
#include "Material.h"
#include "Vertices.h"
#include "myMath.h"
#include "Timer.h"
#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/cimport.h>

struct Bone
{
	std::string name;
	Matrix4 offsetMat;
	Matrix4 currentMat;
};

struct Node
{
	std::string name;
	Vector3 pos = {0,0,0};
	Vector3 scale = { 1,1,1 };
	Vector3 rot = { 0,0,0 };
	Matrix4 localTransformMat;
	Matrix4 globalTransformMat;
	Node* parent = nullptr;
};

struct aiScene;

struct AssimpModel
{
	std::vector<Bone> bones;
	std::vector<Node> nodes;

	Assimp::Importer importer;
	const aiScene* scene;

	std::vector<std::unique_ptr<Vertices>> vertices_;
	std::vector<std::unique_ptr<Material>> materials_;
public:
	AssimpModel();
	static void CreateModel(std::string fileName);

	void PlayAnimetion();

	void Draw(WorldTransform WT);
};