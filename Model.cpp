#include "Model.h"
#include <Windows.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <cassert>
#include <string>
#include "WorldTransform.h"
#include "Material.h"
#include "Texture.h"
#include <fstream>
#include <sstream>
#include "Util.h"

const std::string kBaseDirectory = "Resources/";

//コマンドリストを格納する
static DirectXCommon* directX_ = nullptr;

LightGroup* Model::lightGroup = nullptr;

Model::~Model()
{
	for (auto m : vert_)
	{
		delete m;
	}
	vert_.clear();
	for (auto m : materials_)
	{
		delete m.second;
	}
	materials_.clear();
}

Model* Model::GetInstance()
{
	static Model instance;
	return &instance;
}

void Model::Ini()
{
	directX_ = DirectXCommon::GetInstance();
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

	return /*std::move(*/instance/*)*/;
}

void Model::SetBlend(int blend)
{
	if (blend > 3) blend = 3;
	else if (blend < 0) blend = 0;
	// パイプラインステートとルートシグネチャの設定コマンド
	directX_->GetCommandList()->SetPipelineState(
		PipelineManager::GetObj3dPipeline(blend)->gerPipelineState());

	directX_->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetObj3dPipeline(blend)->GetRootSignature());
}

void Model::SetModel(const Model* model)
{

	vert_.emplace_back(new Vertices);	//空の頂点データを入れる
	Vertices* vert = vert_.back();		//空のvert_のアドレスをvertに入れる

	for (size_t i = 0; i < model->vert_[0]->vertices.size(); i++)
	{
		vert->AddVertices(model->vert_[0]->vertices[i]);
	}

	for (size_t i = 0; i < model->vert_[0]->indices.size(); i++)
	{
		vert->AddIndex(model->vert_[0]->indices[i]);
	}


	//vert_.resize(model->vert_.size());
	//std::copy(model->vert_.begin(), model->vert_.end(), vert_.begin());

	//textureHandle_.resize(model->textureHandle_.size());
	//std::copy(model->textureHandle_.begin(), model->textureHandle_.end(), textureHandle_.begin());

	//for (size_t i = 0; i < model->materials_.size(); i++) {
	//	materials_.insert(model->materials_.at());
	//}

	//materials_.insert(model->materials_.begin(), model->materials_.end());

	for (size_t i = 0; i < model->textureHandle_.size(); i++)
	{
		textureHandle_.push_back(model->textureHandle_[i]);
	}

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

	std::vector<XMFLOAT3> positions; //頂点座標
	std::vector<XMFLOAT3> normals;   // 法線ベクトル
	std::vector<XMFLOAT2> texcoords; // テクスチャUV

	vert_.emplace_back(new Vertices);	//空の頂点データを入れる
	Vertices* vert = vert_.back();		//空のvert_のアドレスをvertに入れる

	int indexCountTex = 0;

	while (getline(file, line))
	{

		std::stringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		//マテリアル
		if (key == "mtllib")
		{
			// マテリアルのファイル名読み込み
			std::string filename;
			line_stream >> filename;
			// マテリアル読み込み
			LoadMaterial(directoryPath, filename);
		}

		if (key == "v")
		{
			// X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			positions.emplace_back(position);
		}

		if (key == "vn")
		{
			// X,Y,Z成分読み込み
			XMFLOAT3 normal{};
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
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			// V方向反転
			texcoord.y = 1.0f - texcoord.y;
			// テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}

		if (key == "f")
		{
			int faceIndexCount = 0;
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
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];

				vert->AddVertices(vertex);
				//エッジ平滑化用のデータを追加
				if (smoothing)
				{
					AddSmoothData(indexPosition, (unsigned short)vert->GetVertexCount() - 1);
				}

				// インデックスデータの追加
				if (faceIndexCount >= 3)
				{
					// 四角形ポリゴンの4点目なので、
					// 四角形の0,1,2,3の内 2,3,0で三角形を構築する
					vert->AddIndex(indexCountTex - 1);
					vert->AddIndex(indexCountTex);
					vert->AddIndex(indexCountTex - 3);
				}
				else
				{
					vert->AddIndex(indexCountTex);
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
			XMFLOAT3 ambient{};

			line_stream >> ambient.x;
			line_stream >> ambient.y;
			line_stream >> ambient.z;

			material->SetAmbient(ambient);

		}

		if (key == "Kd")
		{
			/*line_stream >> material->textureFilename_;*/
			XMFLOAT3 diffuse{};
			line_stream >> diffuse.x;
			line_stream >> diffuse.y;
			line_stream >> diffuse.z;

			material->SetDiffuse(diffuse);
		}

		if (key == "Ks")
		{
			/*line_stream >> material->textureFilename_;*/
			XMFLOAT3 specular{};
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
		material->Ini(DirectXCommon::GetInstance()->GetDevice());
		// マテリアルを登録
		AddMaterial(material);
	}
}

void Model::LoadTexture()
{
	std::string directoryPath = name_ + "/";

	for (auto& m : materials_)
	{
		Material* material = m.second;

		// テクスチャあり
		if (material->textureFilename_.size() > 0)
		{
			// マテリアルにテクスチャ読み込み
			material->LoadTexture(directoryPath);
		}
		// テクスチャなし
		else
		{
			// マテリアルにテクスチャ読み込み
			material->LoadTexture("white1×1.png");
		}
		textureHandle_.push_back(material->textureHandle_);
	}
}

void Model::AddMaterial(Material* material)
{
	// コンテナに登録
	materials_.emplace(material->name_, material);
}

void Model::ModelIni(const std::string& modelname, bool smoothing)
{
	this->smoothing = smoothing;
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

void Model::PreDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(
		PipelineManager::GetObj3dPipeline(3)->gerPipelineState());

	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetObj3dPipeline(3)->GetRootSignature());

	// プリミティブ形状の設定コマンド
	DirectXCommon::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
}

void Model::ObjChangeColor(float x, float y, float z, float w)
{
	for (auto& m : materials_)
	{
		m.second->ChangeColor(x, y, z, w);
	}
}

void Model::ObjChangeColor(XMFLOAT4 color_)
{
	for (auto& m : materials_)
	{
		m.second->ChangeColor(color_);
	}
}

void Model::DrawOBJ(WorldTransform* worldTransform)
{
	lightGroup->Draw(3);
	for (auto& m : materials_)
	{
		m.second->Draw(textureHandle_.at(0));
	}
	for (auto& v : vert_)
	{

		v->Draw(worldTransform, textureHandle_.at(0));
	}
}

void Model::DrawOBJ(WorldTransform* worldTransform, uint32_t textureHandle)
{
	for (auto& m : materials_)
	{
		m.second->Draw(textureHandle);
	}
	for (auto& v : vert_)
	{
		v->Draw(worldTransform, textureHandle);
	}
}

void Model::AddSmoothData(unsigned short indexPositon, unsigned short indexVertex)
{
	smoothData[indexPositon].emplace_back(indexVertex);
}

void Model::CalculateSmoothedVertexNormals()
{
	auto itr = smoothData.begin();
	for (; itr != smoothData.end(); ++itr)
	{
		//各面用の共通頂点コレクション
		std::vector<unsigned short>& v = itr->second;
		//前兆店の法線を平均する
		DirectX::XMVECTOR normal = {};
		for (unsigned short index : v)
		{
			float x = vert_[0]->vertices[index].normal.x;
			float y = vert_[0]->vertices[index].normal.y;
			float z = vert_[0]->vertices[index].normal.z;

			normal += XMVectorSet(x, y, z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());
		//共通法線を使用するすべての頂点データに書き込む
		for (unsigned short index : v)
		{
			float x = normal.m128_f32[0];
			float y = normal.m128_f32[1];
			float z = normal.m128_f32[2];

			vert_[0]->vertices[index].normal = { x,y,z };
			int a = 0;
		}
	}
	vert_[0]->Map();
}
