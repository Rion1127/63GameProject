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

std::unique_ptr<AssimpModel> AssimpLoader::Load(std::string fileName, AssimpModel* model)
{
	std::unique_ptr<AssimpModel> result =
		std::move(std::make_unique<AssimpModel>());

	
	//以下のフラグの数値を代入していく
	uint32_t flag = 0;


	flag |= aiProcess_Triangulate;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_FlipUVs;
	//flag |= aiProcess_GenUVCoords;
	/*flag |= aiProcess_TransformUVCoords;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;
	flag |= aiProcess_LimitBoneWeights;*/


	result->scene = result->importer.ReadFile(fileName, flag);

	if (result->scene == nullptr)
	{
		// もし読み込みエラーがでたら表示する
		printf(result->importer.GetErrorString());
		printf("\n");
		OutputDebugStringA("scene = nullptr");
		return nullptr;
	}
	result->vertices_.resize(result->scene->mNumMeshes);
	result->materials_.resize(result->scene->mNumMeshes);
	for (uint32_t i = 0; i < result->scene->mNumMeshes; ++i)
	{
		result->vertices_[i] = std::move(std::make_unique<Vertices>());
		result->materials_[i] = std::move(std::make_unique<Material>());
		//各種情報読み込み
		LoadVertices(result->vertices_[i].get(), *result->scene->mMeshes);
		if (result->scene->HasMaterials()) {
			LoadMaterial(fileName, result->materials_[i].get(), *result->scene->mMaterials);
		}
	}
	LoadSkin(result.get(), *result->scene->mMeshes);
	//頂点データを更新したので転送する
	for (uint32_t i = 0; i < result->scene->mNumMeshes; ++i)
	{
		result->vertices_[i]->Map();
	}
	LoadNode(result.get(), nullptr, result->scene->mRootNode);

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

	vert->CreateBuffer();
}

void AssimpLoader::LoadMaterial(std::string fileName, Material* material, const aiMaterial* aimaterial)
{
	//テクスチャ
	aiString path;
	if (aimaterial->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
	{
		auto wFileName = ToWideString(fileName);
		// テクスチャパスは相対パスで入っているので、ファイルの場所とくっつける
		//auto dir = GetDirectoryPath(fileName);
		auto file = std::string(path.C_Str());

		std::wstring filename_ = wFileName + ToWideString(file);

		filename_ = ReplaceExtension(filename_, "png");
		material->textureFilename_ = ToUTF8(filename_);
		TextureManager::GetInstance()->
			LoadGraph(material->textureFilename_, material->textureFilename_);
		material->texture_ = *TextureManager::GetInstance()->GetTexture(material->textureFilename_);
	}
}

void AssimpLoader::LoadSkin(AssimpModel* model, const aiMesh* aimesh)
{
	// スキニング情報を持つメッシュかどうかを確認します
	if (aimesh->HasBones() == true)
	{
		// ボーン番号とスキンウェイトのペア
		struct WeightSet
		{
			uint32_t index;
			float weight;
		};
		size_t vertNum = 0;
		for (auto& v : model->vertices_) {
			vertNum += v->vertices_.size();
		}

		// 二次元配列（ジャグ配列） list:頂点が影響を受けるボーンの全リスト vector:それを全頂点分
		std::vector<std::list<WeightSet>> weightLists(vertNum);

		// ボーンの最大数設定
		model->bones.resize(aimesh->mNumBones);

		// スキニング情報の処理
		for (uint32_t i = 0; i < aimesh->mNumBones; i++)
		{
			aiBone* bone = aimesh->mBones[i];

			// ボーンの名前
			model->bones[i].name = bone->mName.C_Str();

			// ボーンの初期姿勢行列(バインドポーズ行列)
			Matrix4 initalMat = ConvertAiMatrixToMatrix(bone->mOffsetMatrix);
			model->bones[i].offsetMat = initalMat.Transpose();

			// ウェイトの読み取り
			for (uint32_t j = 0; j < bone->mNumWeights; j++)
			{
				// 頂点番号
				int vertexIndex = bone->mWeights[j].mVertexId;
				// スキンウェイト
				float weight = bone->mWeights[j].mWeight;
				// その頂点の影響を受けるボーンリストに、ボーンとウェイトのペアを追加
				weightLists[vertexIndex].emplace_back(WeightSet{ i,weight });
			}
		}
		for (auto& v : model->vertices_) {
			// ウェイトの整理
			auto& vertices = v->vertices_;
			// 各頂点について処理
			for (uint32_t i = 0; i < vertices.size(); i++)
			{
				// 頂点のウェイトから最も大きい4つを選択
				auto& weightList = weightLists[i];
				// 大小比較用のラムダ式を指定して降順にソート
				weightList.sort(
					[](auto const& lhs, auto const& rhs)
					{
						return lhs.weight > rhs.weight;
					});

				int weightArrayIndex = 0;
				// 降順ソート済みのウェイトリストから
				for (auto& weightSet : weightList)
				{
					// 頂点データに書き込み
					vertices[i].m_BoneIDs[weightArrayIndex] = weightSet.index;
					vertices[i].m_Weights[weightArrayIndex] = weightSet.weight;
					// 4つに達したら終了
					if (++weightArrayIndex >= 4)
					{
						float weight = 0.f;
						// 2番目以降のウェイトを合計
						for (size_t j = 1; j < 4; j++)
						{
							weight += vertices[i].m_Weights[j];
						}
						// 合計で1,f(100%)になるように調整
						vertices[i].m_Weights[0] = 1.f - weight;
						break;
					}
				}
			}
		}
	}
}

void AssimpLoader::LoadNode(AssimpModel* model, Node* parent, const aiNode* node)
{
	aiString nodeName = node->mName;

	// モデルにノードを追加
	model->nodes.emplace_back();
	Node& modelNode = model->nodes.back();

	// ノード名を取得
	modelNode.name = node->mName.C_Str();

	// ローカル行列
	modelNode.localTransformMat = ConvertAiMatrixToMatrix(node->mTransformation);

	// グローバル行列
	modelNode.globalTransformMat = modelNode.localTransformMat;
	if (parent)
	{
		modelNode.parent = parent;
		// 親の変形を乗算
		modelNode.globalTransformMat *= parent->globalTransformMat;
	}

	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* aimesh = model->scene->mMeshes[node->mMeshes[i]];
		if (aimesh)
		{
			//各種情報読み込み
			LoadVertices(model->vertices_[i].get(), aimesh);
			LoadSkin(model, aimesh);
		}
	}
	//頂点データを更新したので転送する
	for (uint32_t i = 0; i < model->scene->mNumMeshes; ++i)
	{
		model->vertices_[i]->Map();
	}

	// 再帰
	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(model, &modelNode, node->mChildren[i]);
	}
}

Matrix4 AssimpLoader::ConvertAiMatrixToMatrix(const aiMatrix4x4 aimat)
{
	Matrix4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = (float)aimat[i][j];
		}
	}

	return result;
}
