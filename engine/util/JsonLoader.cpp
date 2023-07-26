#include "JsonLoader.h"
#include <fstream>
#include <cassert>

JsonLoader* JsonLoader::GetInstance()
{
	static JsonLoader instance;
	return &instance;
}

JsonLoader::JsonLoader()
{
	kDefaultBaseDirectory = "Resources/JSON/";
}

void JsonLoader::LoadFile(std::string fileName)
{
	//�A�����ăt���p�X�𓾂�
	const std::string fullpath = kDefaultBaseDirectory + fileName;

	//�t�@�C���X�g���[��
	std::ifstream file;
	//�t�@�C�����J��
	file.open(fullpath);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	//��
	file >> deserialized;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"�𕶎���Ƃ��Ď擾
	std::string name =
		deserialized["name"].get<std::string>();
	//�����d����ʃf�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	LevelData* levelData = new LevelData();
	
	//"object"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		if (type.compare("MESH") == 0)
		{
			levelData->object.emplace_back(new Object3D);
			Object3D* newobj = levelData->object.back();

			//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];
			//���s�ړ�
			Vector3 pos = {
				(float)transform["translation"][1],
				(float)transform["translation"][2],
				-(float)transform["translation"][0],
			};
			newobj->SetPos(pos);
			//��]�p
			Vector3 rot = {
				-(float)transform["rotation"][1],
				-(float)transform["rotation"][2],
				(float)transform["rotation"][0],
			};
			newobj->SetRot(rot);
			//�X�P�[�����O
			Vector3 scale = {
				(float)transform["scaling"][1],
				(float)transform["scaling"][2],
				(float)transform["scaling"][0],
			};
			newobj->SetScale(scale);

			if (object.contains("children"))
			{
				levelData->object.emplace_back(new Object3D);
				Object3D& newobj = *levelData->object.back();

				Object3D* parent = levelData->object.at(levelData->object.size() - 2);

				newobj.SetParent(parent->GetWorldTransform());

			}
		}
	}

	levelData_.push_back(*levelData);
	delete levelData;
}