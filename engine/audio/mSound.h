#pragma once
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
#include <map>
#include <wrl.h>
#include <fstream>

typedef std::string SoundKey;

struct ChunkHeader
{
	char id[4];
	int32_t size;
};

struct RiffHeader
{
	ChunkHeader chunk;
	char type[4];
};

struct FormatChunk
{
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

class SoundData
{
public:
	WAVEFORMATEX wfex;

	BYTE* pBuffer;

	unsigned int bufferSize;

	IXAudio2SourceVoice* sound;

	void Release() {
		delete[] pBuffer;

		this->pBuffer = 0;
		this->bufferSize = 0;
		this->wfex = {};

		if (sound != nullptr)
		{
			sound->Stop();
		}
	}
};

class SoundManager {
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	~SoundManager();
	static SoundManager* GetInstance();
	static void Init();

	/// <summary>
	/// WAV�t�@�C���ǂݍ���
	/// </summary>
	/// <param name="path">�t�@�C����</param>
	/// <param name="key">�L�[���[�h
	/// </param>
	static SoundKey LoadWave(std::string path, SoundKey key);
	/// <summary>
	/// �����Đ������ǂ���
	/// </summary>
	/// <param name="key">�ݒ肵���L�[���[�h</param>
	/// <returns>�����Đ������ǂ���</returns>
	static bool IsPlaying(SoundKey key);
	/// <summary>
	/// �Đ�
	/// </summary>
	/// <param name="key">�L�[���[�h</param>
	/// <param name="loopFlag">���[�v</param>
	/// <param name="volum">����
	/// </param>
	static void Play(SoundKey key, bool loopFlag = false, float volum = 1.0f);
	/// <summary>
	/// ���łɓǂݍ��񂾉�����Ԃ�
	/// </summary>
	/// <param name="key">�L�[���[�h</param>
	static SoundData* GetSoundData(SoundKey key);

	static void Stop(SoundKey key);
	
	static void ReleaseAllSounds();

private:
	static ComPtr<IXAudio2> xAudio2;
	static IXAudio2MasteringVoice* masterVoice;
	static std::map<SoundKey, SoundData> sndMap;

};