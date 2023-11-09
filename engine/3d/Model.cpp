#include "Model.h"
#include <Windows.h>
#include <d3d12.h>
#include <d3dcompiler.h>

#include <cassert>
#include <string>
#include <fstream>
#include <sstream>
#include <imgui.h>
#include "Util.h"

/**
 * @file Model.cpp
 * @brief 読み込んだ頂点データ・マテリアルを管理している
 */

const std::string kBaseDirectory = "application/Resources/Object/";

std::shared_ptr<LightGroup> Model::lightGroup_ = nullptr;

Model::Model(bool isShadowSet)
{
	isShadowActive_ = isShadowSet;
	if (isShadowActive_) {
		shadowNum_ = lightGroup_->GetIsNotAvtiveCircleShadow();
		lightGroup_->SetCircleShadowActive(shadowNum_,true);
		shadowAtten_ = { 0.5f,0.6f,0.0f };
		shadowFactorAngle_ = { 0.0f,0.5f };
	}
}

Model::~Model()
{
	if (isShadowActive_)
	{
		lightGroup_->SetCircleShadowActive(shadowNum_, false);
	}
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

std::unique_ptr<Model> Model::CreateOBJ_uniptr(const std::string& modelname, bool smoothing, bool isShadow)
{
	std::unique_ptr<Model> instance = std::make_unique<Model>(isShadow);
	instance->ModelIni(modelname, smoothing);

	return instance;
}

void Model::SetModel(const Model* model)
{

	vert_.emplace_back(std::move(std::make_unique<Vertices>()));	//空の頂点データを入れる
	Vertices& vert = *vert_.back();		//空のvert_のアドレスをvertに入れる

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
	// メッシュのバッファ生成
	for (auto& m : vert_)
	{
		m->CreateBuffer();
	}
}

void Model::LoadOBJ(const std::string& modelname)
{
	const std::string filename = modelname + ".obj";
	const std::string directoryPath = kBaseDirectory + modelname + "/";
	// ファイルストリーム
	std::ifstream file;
	// .objファイルを開く
	file.open(directoryPath + filename);
	// ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	name_ = modelname;

	std::string line;	//ファイルの1行を入れる変数

	std::vector<Vector3> positions{}; //頂点座標
	std::vector<Vector3> normals{};   // 法線ベクトル
	std::vector<Vector2> texcoords{}; // テクスチャUV




	uint16_t indexCountTex = 0;
	uint16_t smoothIndex = 0;

	while (getline(file, line))
	{

		std::stringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		if (key == "o") {
			if (vert_.size() > 0) {
				indexCountTex = 0;
				smoothIndex++;
			}
			vert_.emplace_back(new Vertices);	//空の頂点データを入れる
			smoothData_.emplace_back();
		}

		std::unique_ptr<Vertices>* vert = nullptr;		//空のvert_のアドレスをvertに入れる
		if (vert_.size() > 0)vert = &vert_.back();
		//マテリアル
		if (key == "mtllib")
		{
			// マテリアルのファイル名読み込み
			std::string matfilename;
			line_stream >> matfilename;
			// マテリアル読み込み
			LoadMaterial(directoryPath, matfilename);

		}
		if (key == "usemtl") {
			line_stream >> (*vert)->materialName_;
		}

		if (key == "v")
		{
			// X,Y,Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			positions.emplace_back(position);
		}

		if (key == "vn")
		{
			// X,Y,Z成分読み込み
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			// 法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		// 先頭文字列がvtならテクスチャ
		if (key == "vt")
		{
			// U,V成分読み込み
			Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			// V方向反転
			texcoord.y = 1.0f - texcoord.y;
			// テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}

		if (key == "f")
		{
			uint32_t faceIndexCount = 0;
			// 半角スペース区切りで行の続きを読み込む
			std::string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				// 頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				// 頂点番号
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur); // スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur); // スラッシュを飛ばす
				index_stream >> indexNormal;
				Vertices::VertexPosNormalUv vertex{};
				const int indexPos = indexPosition - 1;
				const int indexNorm = indexNormal - 1;
				const int indexTex = indexTexcoord - 1;
				vertex.pos = positions[indexPos];
				vertex.normal = normals[indexNorm];
				vertex.uv = texcoords[indexTex];

				(*vert)->AddVertices(vertex);
				//エッジ平滑化用のデータを追加
				if (smoothing_)
				{
					unsigned short indexVertex = (unsigned short)(*vert)->GetVertexCount() - 1;
					AddSmoothData(indexPosition, indexVertex, smoothIndex);
				}

				// インデックスデータの追加
				if (faceIndexCount >= 3)
				{
					// 四角形ポリゴンの4点目なので、
					// 四角形の0,1,2,3の内 2,3,0で三角形を構築する
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
	// ファイルストリーム
	std::ifstream file;
	// マテリアルファイルを開く
	file.open(directoryPath + filename);
	// ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	Material* material = nullptr;

	// 1行ずつ読み込む
	std::string line;
	while (getline(file, line))
	{
		// 先頭のタブ文字は無視する
		if (line[0] == '\t')
		{
			line.erase(line.begin()); // 先頭の文字を削除
		}
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		// 先頭のタブ文字は無視する
		if (key[0] == '\t')
		{
			key.erase(key.begin()); // 先頭の文字を削除
		}

		// 先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{

			// 既にマテリアルがあれば
			if (material)
			{
				// マテリアルをコンテナに登録
				AddMaterial(material);
			}

			// 新しいマテリアルを生成
			material = new Material;
			// マテリアル名読み込み
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

		// 先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd")
		{
			// テクスチャのファイル名読み込み
			line_stream >> material->textureFilename_;

			// フルパスからファイル名を取り出す
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
	// ファイルを閉じる
	file.close();


	if (material)
	{
		// マテリアルを登録
		AddMaterial(material);
	}
}

void Model::LoadTexture()
{
	std::string directoryPath = name_ + "/";

	for (auto& m : materials_)
	{
		Material& material = *m.second;

		// テクスチャあり
		if (material.textureFilename_.size() > 0)
		{
			// マテリアルにテクスチャ読み込み
			material.LoadTexture(directoryPath);
		}
		// テクスチャなし
		else
		{
			// マテリアルにテクスチャ読み込み
			material.LoadTexture("white1x1.png");
		}
	}
}

void Model::AddMaterial(Material* material)
{
	// コンテナに登録
	materials_.emplace(material->name_, material);
}

void Model::ModelIni(const std::string& modelname, bool smoothing)
{
	smoothing_ = smoothing;
	LoadOBJ(modelname);
	// メッシュのバッファ生成
	for (auto& m : vert_)
	{
		m->CreateBuffer();
	}

	if (smoothing)
	{
		CalculateSmoothedVertexNormals();
	}
}

void Model::ShadowUpdate(Vector3 pos)
{
	if (isShadowActive_) {
		lightGroup_->SetCircleShadowActive(shadowNum_, true);
		lightGroup_->SetCircleShadowCasterPos(shadowNum_, pos + shadowOffsetPos_);
		lightGroup_->SetCircleShadowDir(shadowNum_, { 0,-1,0 });
		lightGroup_->SetCircleShadowAtten(shadowNum_, shadowAtten_);
		lightGroup_->SetCircleShadowFactorAngle(shadowNum_, shadowFactorAngle_);
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

void Model::DrawShadowInfo(const std::string& imguiName)
{
	ImGui::Begin(imguiName.c_str());

	float atten[3] = { shadowAtten_.x,shadowAtten_.y, shadowAtten_.z};
	ImGui::DragFloat3("Atten", atten,0.01f);

	shadowAtten_ = { atten[0],atten[1] ,atten[2] };

	float factorAngle[2] = { shadowFactorAngle_.x,shadowFactorAngle_.y};
	ImGui::DragFloat2("FactorAngle", factorAngle, 0.01f);

	shadowFactorAngle_ = { factorAngle[0],factorAngle[1]};
	
	ImGui::End();
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
			//各面用の共通頂点コレクション
			std::vector<unsigned short>& v = itr->second;
			//全頂点の法線を平均する
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
			//共通法線を使用するすべての頂点データに書き込む
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
