#pragma once
#include "Texture.h"
#include "Vector3.h"
#include "Color.h"
#include "WorldTransform.h"
#include "Timer.h"
#include "PipelineManager.h"

struct IEmitter;

class IParticle
{
protected:
	struct VertexParticle
	{
		Vector3 pos;
		float scale;
		Vector3 rot;
		Vector2 ancorPoint = { 0.0f,0.0f };
		Color color;
	};
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
		int32_t frame = 0;
		//�I���t���[��
		int32_t end_frame = 0;
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
protected:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//���_�f�[�^
	std::vector<VertexParticle> vertices_;

	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	ComPtr<ID3D12Resource> vertBuff_;

	Texture texture;

	WorldTransform transform_;

	int32_t maxParticleNum_;
	//�p�[�e�B�N���f�[�^
	std::vector<Particle> particles_;

	bool isBillBoard;
	std::string shaderName_;
	PipeLineState state_;
	IEmitter* emitter_;
public:
	IParticle(std::string shaderName = "Particle") {
		shaderName_ = shaderName;
		isBillBoard = false;
		state_ = PipeLineState::Alpha;
	};
	virtual ~IParticle();

	void Update();

	void Draw();

	virtual void Add() = 0;
	void SetEmitter(IEmitter* emitter) { emitter_ = emitter; }
public:
	std::string GetShaderName() { return shaderName_; }
	size_t GetParticleNum() { return particles_.size(); }
	PipeLineState GetPipelineState() { return state_; }
private:
	//�f�[�^�]��
	void TransferBuff();
	//end_frame�𒴂�����폜
	void DeleteUpdate();
protected:
	void Init(int32_t vertexCount);
	//�p����̃p�[�e�B�N���̏���
	//�����X�V
	virtual void MoveUpdate() = 0;
};

struct IEmitter {
	std::unique_ptr<IParticle> particle;
	int32_t addNum;	//����������
	int32_t time;	//�p�[�e�B�N����������
	Vector3 pos;	//�G�~�b�^�[���S���W
	Vector3* parentPos = nullptr;	//�G�~�b�^�[���S���W
	Vector3 addVec;	//�ړ������x�N�g��
	float scale;
	bool isActive = true;
	Timer popCoolTime_;
};

struct OneceEmitter : public IEmitter {
	OneceEmitter() { isActive = false; }
};
//�t���O��false�ɂȂ�܂ŏo������
struct ContinuousEmitter : public IEmitter {
	
};

