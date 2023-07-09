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
	//�ȉ��̃t���O�̐��l�������Ă���
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
		// �����ǂݍ��݃G���[���ł���\������
		printf(importer.GetErrorString());
		printf("\n");
		OutputDebugStringA("scene = nullptr");
		return false;
	}

	// �ǂݍ��񂾃f�[�^�������Œ�`����Mesh�\���̂ɕϊ�����
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
	//�ȉ��̃t���O�̐��l�������Ă���
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
		// �����ǂݍ��݃G���[���ł���\������
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

	//���_�f�[�^����
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
	//�C���f�b�N�X�f�[�^����
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
		// �e�N�X�`���p�X�͑��΃p�X�œ����Ă���̂ŁA�t�@�C���̏ꏊ�Ƃ�������
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
	//// �X�L�j���O���������b�V�����ǂ������m�F���܂�
	//if (pMesh->HasBones() == true)
	//{
	//	// �{�[���ԍ��ƃX�L���E�F�C�g�̃y�A
	//	struct WeightSet
	//	{
	//		uint32_t index;
	//		float weight;
	//	};

	//	// �񎟌��z��i�W���O�z��j list:���_���e�����󂯂�{�[���̑S���X�g vector:�����S���_��
	//	std::vector<std::list<WeightSet>> weightLists(dst.Vertices.vertices_.size());

	//	// �{�[���̍ő吔�ݒ�
	//	dst->ge .resize(pMesh->mNumBones);

	//	// �X�L�j���O���̏���
	//	for (uint32_t i = 0; i < pMesh->mNumBones; i++)
	//	{
	//		aiBone* bone = pMesh->mBones[i];

	//		// �{�[���̖��O
	//		model->bones[i].name = bone->mName.C_Str();

	//		// �{�[���̏����p���s��(�o�C���h�|�[�Y�s��)
	//		Mat4 initalMat = ConvertMat4FromAssimpMat(bone->mOffsetMatrix);
	//		model->bones[i].offsetMat = initalMat.Transpose();

	//		// �E�F�C�g�̓ǂݎ��
	//		for (uint32_t j = 0; j < bone->mNumWeights; j++)
	//		{
	//			// ���_�ԍ�
	//			int vertexIndex = bone->mWeights[j].mVertexId;
	//			// �X�L���E�F�C�g
	//			float weight = bone->mWeights[j].mWeight;
	//			// ���̒��_�̉e�����󂯂�{�[�����X�g�ɁA�{�[���ƃE�F�C�g�̃y�A��ǉ�
	//			weightLists[vertexIndex].emplace_back(WeightSet{ i,weight });
	//		}
	//	}

	//	// �E�F�C�g�̐���
	//	auto& vertices = model->mesh.vertices;
	//	// �e���_�ɂ��ď���
	//	for (uint32_t i = 0; i < vertices.size(); i++)
	//	{
	//		// ���_�̃E�F�C�g����ł��傫��4��I��
	//		auto& weightList = weightLists[i];
	//		// �召��r�p�̃����_�����w�肵�č~���Ƀ\�[�g
	//		weightList.sort(
	//			[](auto const& lhs, auto const& rhs)
	//			{
	//				return lhs.weight > rhs.weight;
	//			});

	//		int weightArrayIndex = 0;
	//		// �~���\�[�g�ς݂̃E�F�C�g���X�g����
	//		for (auto& weightSet : weightList)
	//		{
	//			// ���_�f�[�^�ɏ�������
	//			vertices[i].boneIndex[weightArrayIndex] = weightSet.index;
	//			vertices[i].boneWeight[weightArrayIndex] = weightSet.weight;
	//			// 4�ɒB������I��
	//			if (++weightArrayIndex >= maxBoneIndices)
	//			{
	//				float weight = 0.f;
	//				// 2�Ԗڈȍ~�̃E�F�C�g�����v
	//				for (size_t j = 1; j < maxBoneIndices; j++)
	//				{
	//					weight += vertices[i].boneWeight[j];
	//				}
	//				// ���v��1,f(100%)�ɂȂ�悤�ɒ���
	//				vertices[i].boneWeight[0] = 1.f - weight;
	//				break;
	//			}
	//		}
	//	}
	//}
}
