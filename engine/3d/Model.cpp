#include "Model.h"
#include <Windows.h>
#include <d3d12.h>
#include <d3dcompiler.h>

#include <cassert>
#include <string>
#include <fstream>
#include <sstream>
#include "Util.h"
const std::string kBaseDirectory = "application/Resources/Object/";

std::shared_ptr<LightGroup> Model::lightGroup_ = nullptr;

Model::~Model()
{
	materials_.clear();
	vert_.clear();
}

Model* Model::GetInstance()
{
	static Model instance;
	return &instance;
}


Model* Model::CreateOBJ(const std::string& modelname, bool smoothing)
{
	Model* instance = new Model;
	instance->ModelIni(modelname, smoothing);

	return instance;
}

std::unique_ptr<Model> Model::CreateOBJ_uniptr(const std::string& modelname, bool smoothing)
{
	std::unique_ptr<Model> instance = std::make_unique<Model>();
	instance->ModelIni(modelname, smoothing);

	return instance;
}

void Model::SetModel(const Model* model)
{

	vert_.emplace_back(std::move(std::make_unique<Vertices>()));	//��̒��_�f�[�^������
	Vertices& vert = *vert_.back();		//���vert_�̃A�h���X��vert�ɓ����

	for (int32_t i = 0; i < model->vert_[0]->vertices_.size(); i++)
	{
		vert.AddVertices(model->vert_[0]->vertices_[i]);
	}

	for (int32_t i = 0; i < model->vert_[0]->indices_.size(); i++)
	{
		vert.AddIndex(model->vert_[0]->indices_[i]);
	}


	//vert_.resize(model->vert_.size());
	//std::copy(model->vert_.begin(), model->vert_.end(), vert_.begin());

	//textureHandle_.resize(model->textureHandle_.size());
	//std::copy(model->textureHandle_.begin(), model->textureHandle_.end(), textureHandle_.begin());

	//for (size_t i = 0; i < model->materials_.size(); i++) {
	//	materials_.insert(model->materials_.at());
	//}

	//materials_.insert(model->materials_.begin(), model->materials_.end());
	// ���b�V���̃o�b�t�@����
	for (auto& m : vert_)
	{
		m->CreateBuffer();
	}
}

void Model::LoadOBJ(const std::string& modelname)
{
	const std::string filename = modelname + ".obj";
	const std::string directoryPath = kBaseDirectory + modelname + "/";
	// �t�@�C���X�g���[��
	std::ifstream file;
	// .obj�t�@�C�����J��
	file.open(directoryPath + filename);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	name_ = modelname;

	std::string line;	//�t�@�C����1�s������ϐ�

	std::vector<Vector3> positions{}; //���_���W
	std::vector<Vector3> normals{};   // �@���x�N�g��
	std::vector<Vector2> texcoords{}; // �e�N�X�`��UV

	
	

	uint16_t indexCountTex = 0;
	uint16_t smoothIndex = 0;

	while (getline(file, line))
	{

		std::stringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		std::string key;
		getline(line_stream, key, ' ');

		if (key == "o") {
			if (vert_.size() > 0) {
				indexCountTex = 0;
				smoothIndex++;
			}
			vert_.emplace_back(new Vertices);	//��̒��_�f�[�^������
			smoothData_.emplace_back();
		}

		std::unique_ptr<Vertices>* vert = nullptr;		//���vert_�̃A�h���X��vert�ɓ����
		if (vert_.size() > 0)vert = &vert_.back();
		//�}�e���A��
		if (key == "mtllib")
		{
			// �}�e���A���̃t�@�C�����ǂݍ���
			std::string matfilename;
			line_stream >> matfilename;
			// �}�e���A���ǂݍ���
			LoadMaterial(directoryPath, matfilename);
			
		}
		if (key == "usemtl") {
			line_stream >> (*vert)->materialName_;
		}

		if (key == "v")
		{
			// X,Y,Z���W�ǂݍ���
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			positions.emplace_back(position);
		}

		if (key == "vn")
		{
			// X,Y,Z�����ǂݍ���
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			// �@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		// �擪������vt�Ȃ�e�N�X�`��
		if (key == "vt")
		{
			// U,V�����ǂݍ���
			Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			// V�������]
			texcoord.y = 1.0f - texcoord.y;
			// �e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}

		if (key == "f")
		{
			uint32_t faceIndexCount = 0;
			// ���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			std::string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				// ���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				// ���_�ԍ�
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur); // �X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur); // �X���b�V�����΂�
				index_stream >> indexNormal;
				Vertices::VertexPosNormalUv vertex{};
				const int indexPos = indexPosition - 1;
				const int indexNorm = indexNormal - 1;
				const int indexTex = indexTexcoord - 1;
				vertex.pos = positions[indexPos];
				vertex.normal = normals[indexNorm];
				vertex.uv = texcoords[indexTex];

				(*vert)->AddVertices(vertex);
				//�G�b�W�������p�̃f�[�^��ǉ�
				if (smoothing_)
				{
					unsigned short indexVertex = (unsigned short)(*vert)->GetVertexCount() - 1;
					AddSmoothData(indexPosition, indexVertex, smoothIndex);
				}

				// �C���f�b�N�X�f�[�^�̒ǉ�
				if (faceIndexCount >= 3)
				{
					// �l�p�`�|���S����4�_�ڂȂ̂ŁA
					// �l�p�`��0,1,2,3�̓� 2,3,0�ŎO�p�`���\�z����
					(*vert)->AddIndex(indexCountTex - 1);
					(*vert)->AddIndex(indexCountTex);
					(*vert)->AddIndex(indexCountTex - 3);
				}
				else
				{
					(*vert)->AddIndex(indexCountTex);
				}
				faceIndexCount++;
				indexCountTex++;
			}
		}
	}



	LoadTexture();

	file.close();
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	// �t�@�C���X�g���[��
	std::ifstream file;
	// �}�e���A���t�@�C�����J��
	file.open(directoryPath + filename);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	Material* material = nullptr;

	// 1�s���ǂݍ���
	std::string line;
	while (getline(file, line))
	{
		// �擪�̃^�u�����͖�������
		if (line[0] == '\t')
		{
			line.erase(line.begin()); // �擪�̕������폜
		}
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		std::string key;
		getline(line_stream, key, ' ');

		// �擪�̃^�u�����͖�������
		if (key[0] == '\t')
		{
			key.erase(key.begin()); // �擪�̕������폜
		}

		// �擪������newmtl�Ȃ�}�e���A����
		if (key == "newmtl")
		{

			// ���Ƀ}�e���A���������
			if (material)
			{
				// �}�e���A�����R���e�i�ɓo�^
				AddMaterial(material);
			}

			// �V�����}�e���A���𐶐�
			material = new Material;
			// �}�e���A�����ǂݍ���
			line_stream >> material->name_;

		}

		if (key == "Ka")
		{
			Vector3 ambient{};

			line_stream >> ambient.x;
			line_stream >> ambient.y;
			line_stream >> ambient.z;

			material->SetAmbient(ambient);

		}

		if (key == "Kd")
		{
			/*line_stream >> material->textureFilename_;*/
			Vector3 diffuse{};
			line_stream >> diffuse.x;
			line_stream >> diffuse.y;
			line_stream >> diffuse.z;

			material->SetDiffuse(diffuse);
		}

		if (key == "Ks")
		{
			Vector3 specular{};
			line_stream >> specular.x;
			line_stream >> specular.y;
			line_stream >> specular.z;

			material->SetSpecular(specular);
		}

		// �擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd")
		{
			// �e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> material->textureFilename_;

			// �t���p�X����t�@�C���������o��
			size_t pos1;
			pos1 = material->textureFilename_.rfind('\\');
			if (pos1 != std::string::npos)
			{
				material->textureFilename_ = material->textureFilename_.substr(
					pos1 + 1, material->textureFilename_.size() - pos1 - 1);
			}

			pos1 = material->textureFilename_.rfind('/');
			if (pos1 != std::string::npos)
			{
				material->textureFilename_ = material->textureFilename_.substr(
					pos1 + 1, material->textureFilename_.size() - pos1 - 1);
			}
		}
	}
	// �t�@�C�������
	file.close();


	if (material)
	{
		// �}�e���A����o�^
		AddMaterial(material);
	}
}

void Model::LoadTexture()
{
	std::string directoryPath = name_ + "/";

	for (auto& m : materials_)
	{
		Material& material = *m.second;

		// �e�N�X�`������
		if (material.textureFilename_.size() > 0)
		{
			// �}�e���A���Ƀe�N�X�`���ǂݍ���
			material.LoadTexture(directoryPath);
		}
		// �e�N�X�`���Ȃ�
		else
		{
			// �}�e���A���Ƀe�N�X�`���ǂݍ���
			material.LoadTexture("white1x1.png");
		}
	}
}

void Model::AddMaterial(Material* material)
{
	// �R���e�i�ɓo�^
	materials_.emplace(material->name_, material);
}

void Model::ModelIni(const std::string& modelname, bool smoothing)
{
	smoothing_ = smoothing;
	LoadOBJ(modelname);
	// ���b�V���̃o�b�t�@����
	for (auto& m : vert_)
	{
		m->CreateBuffer();
	}

	if (smoothing)
	{
		CalculateSmoothedVertexNormals();
	}
}

void Model::DrawOBJ(const WorldTransform& worldTransform)
{
	lightGroup_->Draw(3);
	
	for (uint32_t i = 0; i < vert_.size(); i++)
	{
		materials_.find(vert_[i]->materialName_)->second->Draw();
		vert_[i]->Draw(worldTransform);
	}
}

void Model::DrawOBJ(const WorldTransform& worldTransform, uint32_t textureHandle)
{
	for (auto& m : materials_)
	{
		m.second->Draw(textureHandle);
	}
	for (auto& v : vert_)
	{
		v->Draw(worldTransform);
	}
}

void Model::AddSmoothData(unsigned short indexPositon, unsigned short indexVertex, uint32_t dataindex)
{
	smoothData_[dataindex][indexPositon].emplace_back(indexVertex);
}

void Model::CalculateSmoothedVertexNormals()
{
	for (uint32_t i = 0; i < vert_.size(); i++) {
		auto itr = smoothData_[i].begin();
		for (; itr != smoothData_[i].end(); ++itr)
		{
			//�e�ʗp�̋��ʒ��_�R���N�V����
			std::vector<unsigned short>& v = itr->second;
			//�S���_�̖@���𕽋ς���
			Vector3 normal = {};
			for (unsigned short index : v)
			{
				float x = vert_[i]->vertices_[index].normal.x;
				float y = vert_[i]->vertices_[index].normal.y;
				float z = vert_[i]->vertices_[index].normal.z;

				normal.x += x;
				normal.y += y;
				normal.z += z;
			}
			normal = normal.normalize() / (float)v.size();
			//���ʖ@�����g�p���邷�ׂĂ̒��_�f�[�^�ɏ�������
			for (unsigned short index : v)
			{
				float x = normal.x;
				float y = normal.y;
				float z = normal.z;

				vert_[i]->vertices_[index].normal = { x,y,z };
			}
		}
		vert_[i]->Map();
	}
}
