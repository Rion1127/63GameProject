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

	
	//�ȉ��̃t���O�̐��l�������Ă���
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
		// �����ǂݍ��݃G���[���ł���\������
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
		//�e����ǂݍ���
		LoadVertices(result->vertices_[i].get(), *result->scene->mMeshes);
		if (result->scene->HasMaterials()) {
			LoadMaterial(fileName, result->materials_[i].get(), *result->scene->mMaterials);
		}
	}
	LoadSkin(result.get(), *result->scene->mMeshes);
	//���_�f�[�^���X�V�����̂œ]������
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

	vert->CreateBuffer();
}

void AssimpLoader::LoadMaterial(std::string fileName, Material* material, const aiMaterial* aimaterial)
{
	//�e�N�X�`��
	aiString path;
	if (aimaterial->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
	{
		auto wFileName = ToWideString(fileName);
		// �e�N�X�`���p�X�͑��΃p�X�œ����Ă���̂ŁA�t�@�C���̏ꏊ�Ƃ�������
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
	// �X�L�j���O���������b�V�����ǂ������m�F���܂�
	if (aimesh->HasBones() == true)
	{
		// �{�[���ԍ��ƃX�L���E�F�C�g�̃y�A
		struct WeightSet
		{
			uint32_t index;
			float weight;
		};
		size_t vertNum = 0;
		for (auto& v : model->vertices_) {
			vertNum += v->vertices_.size();
		}

		// �񎟌��z��i�W���O�z��j list:���_���e�����󂯂�{�[���̑S���X�g vector:�����S���_��
		std::vector<std::list<WeightSet>> weightLists(vertNum);

		// �{�[���̍ő吔�ݒ�
		model->bones.resize(aimesh->mNumBones);

		// �X�L�j���O���̏���
		for (uint32_t i = 0; i < aimesh->mNumBones; i++)
		{
			aiBone* bone = aimesh->mBones[i];

			// �{�[���̖��O
			model->bones[i].name = bone->mName.C_Str();

			// �{�[���̏����p���s��(�o�C���h�|�[�Y�s��)
			Matrix4 initalMat = ConvertAiMatrixToMatrix(bone->mOffsetMatrix);
			model->bones[i].offsetMat = initalMat.Transpose();

			// �E�F�C�g�̓ǂݎ��
			for (uint32_t j = 0; j < bone->mNumWeights; j++)
			{
				// ���_�ԍ�
				int vertexIndex = bone->mWeights[j].mVertexId;
				// �X�L���E�F�C�g
				float weight = bone->mWeights[j].mWeight;
				// ���̒��_�̉e�����󂯂�{�[�����X�g�ɁA�{�[���ƃE�F�C�g�̃y�A��ǉ�
				weightLists[vertexIndex].emplace_back(WeightSet{ i,weight });
			}
		}
		for (auto& v : model->vertices_) {
			// �E�F�C�g�̐���
			auto& vertices = v->vertices_;
			// �e���_�ɂ��ď���
			for (uint32_t i = 0; i < vertices.size(); i++)
			{
				// ���_�̃E�F�C�g����ł��傫��4��I��
				auto& weightList = weightLists[i];
				// �召��r�p�̃����_�����w�肵�č~���Ƀ\�[�g
				weightList.sort(
					[](auto const& lhs, auto const& rhs)
					{
						return lhs.weight > rhs.weight;
					});

				int weightArrayIndex = 0;
				// �~���\�[�g�ς݂̃E�F�C�g���X�g����
				for (auto& weightSet : weightList)
				{
					// ���_�f�[�^�ɏ�������
					vertices[i].m_BoneIDs[weightArrayIndex] = weightSet.index;
					vertices[i].m_Weights[weightArrayIndex] = weightSet.weight;
					// 4�ɒB������I��
					if (++weightArrayIndex >= 4)
					{
						float weight = 0.f;
						// 2�Ԗڈȍ~�̃E�F�C�g�����v
						for (size_t j = 1; j < 4; j++)
						{
							weight += vertices[i].m_Weights[j];
						}
						// ���v��1,f(100%)�ɂȂ�悤�ɒ���
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

	// ���f���Ƀm�[�h��ǉ�
	model->nodes.emplace_back();
	Node& modelNode = model->nodes.back();

	// �m�[�h�����擾
	modelNode.name = node->mName.C_Str();

	// ���[�J���s��
	modelNode.localTransformMat = ConvertAiMatrixToMatrix(node->mTransformation);

	// �O���[�o���s��
	modelNode.globalTransformMat = modelNode.localTransformMat;
	if (parent)
	{
		modelNode.parent = parent;
		// �e�̕ό`����Z
		modelNode.globalTransformMat *= parent->globalTransformMat;
	}

	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* aimesh = model->scene->mMeshes[node->mMeshes[i]];
		if (aimesh)
		{
			//�e����ǂݍ���
			LoadVertices(model->vertices_[i].get(), aimesh);
			LoadSkin(model, aimesh);
		}
	}
	//���_�f�[�^���X�V�����̂œ]������
	for (uint32_t i = 0; i < model->scene->mNumMeshes; ++i)
	{
		model->vertices_[i]->Map();
	}

	// �ċA
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
