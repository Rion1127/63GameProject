#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <cassert>
#include "AssimpLoader.h"
#include "Util.h"



AssimpLoader* AssimpLoader::GetInstance()
{
	static AssimpLoader instance;
	return &instance;
}



bool AssimpLoader::Load(ImportSettings* setting)
{
	assert(setting->filename);

	auto& meshes = setting->meshes;
	auto inverseU = setting->inverseU;
	auto inverseV = setting->inverseV;

	auto path = ToUTF8(setting->filename);

	Assimp::Importer importer;
	//以下のフラグの数値を代入していく
	uint32_t flag = 0;

	/*flag |= aiProcess_Triangulate;
	flag |= aiProcess_PreTransformVertices;
	flag |= aiProcess_JoinIdenticalVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
	flag |= aiProcess_TransformUVCoords;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;
	flag |= aiProcess_LimitBoneWeights;*/

	flag |= aiProcess_Triangulate;
	flag |= aiProcess_JoinIdenticalVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
	flag |= aiProcess_TransformUVCoords;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;
	flag |= aiProcess_LimitBoneWeights;

	auto scene = importer.ReadFile(path, flag);

	if (scene == nullptr)
	{
		// もし読み込みエラーがでたら表示する
		printf(importer.GetErrorString());
		printf("\n");
		OutputDebugStringA("scene = nullptr");
		return false;
	}

	// 読み込んだデータを自分で定義したMesh構造体に変換する
	meshes.clear();
	meshes.resize(scene->mNumMeshes);
	for (uint32_t i = 0; i < meshes.size(); ++i)
	{
		const auto pMesh = scene->mMeshes[i];
		LoadMesh(meshes[i], pMesh, inverseU, inverseV);
		const auto pMaterial = scene->mMaterials[i];
		if (scene->mNumMaterials > i) {
			LoadTexture(setting->filename, meshes[i], pMaterial);
		}
		

	}

	scene = nullptr;

	return true;
}

std::unique_ptr<AssimpModel> AssimpLoader::Load(std::string fileName)
{
	std::unique_ptr<AssimpModel> result =
		std::move(std::make_unique<AssimpModel>());

	Assimp::Importer importer;
	//以下のフラグの数値を代入していく
	uint32_t flag = 0;

	flag |= aiProcess_Triangulate;
	flag |= aiProcess_JoinIdenticalVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
	flag |= aiProcess_TransformUVCoords;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;
	flag |= aiProcess_LimitBoneWeights;

	auto scene = importer.ReadFile(fileName, flag);

	if (scene == nullptr)
	{
		// もし読み込みエラーがでたら表示する
		printf(importer.GetErrorString());
		printf("\n");
		OutputDebugStringA("scene = nullptr");
		return nullptr;
	}
	result->vertices_.resize(scene->mNumMeshes);
	result->materials_.resize(scene->mNumMeshes);
	for (uint32_t i = 0; i < scene->mNumMeshes; ++i)
	{
		result->vertices_[i] = std::move(std::make_unique<Vertices>());
		result->materials_[i] = std::move(std::make_unique<Material>());
		LoadVertices(result->vertices_[i].get(), *scene->mMeshes);
		if (scene->HasMaterials()) {
			LoadMaterial(fileName,result->materials_[i].get(), *scene->mMaterials);
		}
	}

	return std::move(result);
}

void AssimpLoader::LoadVertices(Vertices* vert, const aiMesh* aimesh)
{
	aiVector3D zero3D(0.0f, 0.0f, 0.0f);

	//頂点データを代入
	vert->vertices_.resize(aimesh->mNumVertices);
	for (auto i = 0u; i < aimesh->mNumVertices; ++i)
	{
		auto position = &(aimesh->mVertices[i]);
		auto normal = &(aimesh->mNormals[i]);
		auto uv = (aimesh->HasTextureCoords(0)) ? &(aimesh->mTextureCoords[0][i]) : &zero3D;

		Vertices::VertexPosNormalUv vertex = {};
		vertex.pos = Vector3(position->x, position->y, position->z);
		vertex.normal = Vector3(normal->x, normal->y, normal->z);
		vertex.uv = Vector2(uv->x, uv->y);

		vert->vertices_[i] = vertex;
	}
	//インデックスデータを代入
	vert->indices_.resize(aimesh->mNumFaces * 3);
	for (auto i = 0u; i < aimesh->mNumFaces; ++i)
	{
		const auto& face = aimesh->mFaces[i];

		vert->indices_[i * 3 + 0] = (uint16_t)face.mIndices[0];
		vert->indices_[i * 3 + 1] = (uint16_t)face.mIndices[1];
		vert->indices_[i * 3 + 2] = (uint16_t)face.mIndices[2];
	}


}

void AssimpLoader::LoadMaterial(std::string fileName, Material* material, const aiMaterial* aimaterial)
{
	aiString path;
	if (aimaterial->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
	{
		auto wFileName = ToWideString(fileName);
		// テクスチャパスは相対パスで入っているので、ファイルの場所とくっつける
		//auto dir = GetDirectoryPath(fileName);
		auto file = std::string(path.C_Str());

		std::wstring filename_ = wFileName + ToWideString(file);

		filename_ = ReplaceExtension(filename_, "tga");
		material->textureFilename_ = ToUTF8(filename_);
	}
	else
	{
		
	}
	
}

void AssimpLoader::LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV)
{
	
}

void AssimpLoader::LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src)
{
	

}

void AssimpLoader::LoadBones(Mesh& dst, const aiMesh* pMesh)
{
	//// スキニング情報を持つメッシュかどうかを確認します
	//if (pMesh->HasBones() == true)
	//{
	//	// ボーン番号とスキンウェイトのペア
	//	struct WeightSet
	//	{
	//		uint32_t index;
	//		float weight;
	//	};

	//	// 二次元配列（ジャグ配列） list:頂点が影響を受けるボーンの全リスト vector:それを全頂点分
	//	std::vector<std::list<WeightSet>> weightLists(dst.Vertices.vertices_.size());

	//	// ボーンの最大数設定
	//	dst->ge .resize(pMesh->mNumBones);

	//	// スキニング情報の処理
	//	for (uint32_t i = 0; i < pMesh->mNumBones; i++)
	//	{
	//		aiBone* bone = pMesh->mBones[i];

	//		// ボーンの名前
	//		model->bones[i].name = bone->mName.C_Str();

	//		// ボーンの初期姿勢行列(バインドポーズ行列)
	//		Mat4 initalMat = ConvertMat4FromAssimpMat(bone->mOffsetMatrix);
	//		model->bones[i].offsetMat = initalMat.Transpose();

	//		// ウェイトの読み取り
	//		for (uint32_t j = 0; j < bone->mNumWeights; j++)
	//		{
	//			// 頂点番号
	//			int vertexIndex = bone->mWeights[j].mVertexId;
	//			// スキンウェイト
	//			float weight = bone->mWeights[j].mWeight;
	//			// その頂点の影響を受けるボーンリストに、ボーンとウェイトのペアを追加
	//			weightLists[vertexIndex].emplace_back(WeightSet{ i,weight });
	//		}
	//	}

	//	// ウェイトの整理
	//	auto& vertices = model->mesh.vertices;
	//	// 各頂点について処理
	//	for (uint32_t i = 0; i < vertices.size(); i++)
	//	{
	//		// 頂点のウェイトから最も大きい4つを選択
	//		auto& weightList = weightLists[i];
	//		// 大小比較用のラムダ式を指定して降順にソート
	//		weightList.sort(
	//			[](auto const& lhs, auto const& rhs)
	//			{
	//				return lhs.weight > rhs.weight;
	//			});

	//		int weightArrayIndex = 0;
	//		// 降順ソート済みのウェイトリストから
	//		for (auto& weightSet : weightList)
	//		{
	//			// 頂点データに書き込み
	//			vertices[i].boneIndex[weightArrayIndex] = weightSet.index;
	//			vertices[i].boneWeight[weightArrayIndex] = weightSet.weight;
	//			// 4つに達したら終了
	//			if (++weightArrayIndex >= maxBoneIndices)
	//			{
	//				float weight = 0.f;
	//				// 2番目以降のウェイトを合計
	//				for (size_t j = 1; j < maxBoneIndices; j++)
	//				{
	//					weight += vertices[i].boneWeight[j];
	//				}
	//				// 合計で1,f(100%)になるように調整
	//				vertices[i].boneWeight[0] = 1.f - weight;
	//				break;
	//			}
	//		}
	//	}
	//}
}
