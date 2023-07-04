#pragma once
#include "Texture.h"
#include "Vector3.h"
#include "Color.h"
#include "WorldTransform.h"

struct VertexParticle
{
	Vector3 pos;
	float scale;
	Vector3 rot;
	Vector2 ancorPoint = { 0.0f,0.0f };
	Color color;
};

class IParticle
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	IParticle();
	~IParticle();
	void Init(int vertexCount);

	inline void SetTexture(const Texture& texture) { this->texture = texture; }

	void Update();

	void Draw();

	struct Particle
	{
		//���W
		Vector3 position = {};
		Vector3 basePos = {};
		Vector3 endPos = {};
		Vector3* parentPos;
		//���x
		Vector3 velocity = {};
		Vector3 baseVelo = {};
		Vector3 veloAdd = {};

		Vector3 rot = {};
		Vector3 addRot = {};
		//���݃t���[��
		int frame = 0;
		//�I���t���[��
		int end_frame = 0;
		// ���[�J���X�P�[��
		float scale = 1.0f;			//�X�P�[��
		float maxScale = 1.f;
		float baseScale = 0;
		//�F(RGBA)
		Color color = {};
		Color originColor = {};	//�F�̏����l
		Color endColor = {};	//�F�̍ŏI�l

		int particleType = 0;
		Vector2 ancorPoint_ = { 0,0 };
	};
private:
	//�f�[�^�]��
	void TransferBuff();
	//end_frame�𒴂�����폜
	void DeleteUpdate();

	//void SetTextureRect(const Vec2 leftTopPos, const Vec2 rightDownPos);

public:
	//�p����̃p�[�e�B�N���̏���
	//�����X�V
	virtual void MoveUpdate() = 0;
public:

	std::vector<VertexParticle> vertices_;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	ComPtr<ID3D12Resource> vertBuff_;

	WorldTransform transform;

	Vector3 pos;
	Vector3 scale;
	Vector3 rot;
	Texture texture;

	std::vector<Particle> particles;
};


