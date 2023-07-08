#include <cassert>
#include "mSound.h"
#pragma comment(lib, "xaudio2.lib")

Microsoft::WRL::ComPtr<IXAudio2> SoundManager::sxAudio2_;
IXAudio2MasteringVoice* SoundManager::smasterVoice_;
std::map<SoundKey, SoundData> SoundManager::ssndMap_;

std::string directoryPath_ = "application/Resources/BGM_SE/";

SoundManager::~SoundManager()
{
}

SoundManager* SoundManager::GetInstance()
{
	static SoundManager instance;
	return &instance;
}

void SoundManager::Init()
{
	XAudio2Create(&sxAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	sxAudio2_->CreateMasteringVoice(&smasterVoice_);
	ssndMap_.clear();
}

SoundKey SoundManager::LoadWave(const std::string& path, const SoundKey& key)
{
	std::ifstream file;
	
	std::string fullPath = directoryPath_ + path;

	file.open(fullPath, std::ios_base::binary);

	assert(file.is_open());

	RiffHeader riff{};
	file.read((char*)&riff, sizeof(riff));

	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	FormatChunk format{};

	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}

	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	ChunkHeader data{};
	file.read((char*)&data, sizeof(data));

	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		file.seekg(data.size, std::ios_base::cur);

		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	std::vector<BYTE> pBuffer_;
	pBuffer_.resize(data.size);
	
	file.read((char*)pBuffer_.data(), data.size);

	file.close();

	SoundData soundData = {};

	soundData.wfex_ = format.fmt;
	soundData.pBuffer_ = pBuffer_;
	soundData.bufferSize_ = data.size;

	ssndMap_.emplace(key, soundData);

	return key;
}

bool SoundManager::IsPlaying(const SoundKey& key) {
	IXAudio2SourceVoice* pSourceVoice = nullptr;//これ保存しとくと止められる
	SoundData* pSnd = &ssndMap_[key];

	sxAudio2_->CreateSourceVoice(&pSourceVoice, &pSnd->wfex_);
	XAUDIO2_VOICE_STATE state{};
	pSourceVoice->GetState(&state);
	return false;
}

void SoundManager::Play(const SoundKey& key, bool loopFlag, float volum, float picth)
{
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	SoundData* pSnd = &ssndMap_[key];

	if (pSnd->sound_ != nullptr)
	{
		pSnd->sound_->Stop();
	}

	sxAudio2_->CreateSourceVoice(&pSourceVoice, &pSnd->wfex_);

	XAUDIO2_BUFFER buf{};

	buf.pAudioData = pSnd->pBuffer_.data();
	buf.AudioBytes = pSnd->bufferSize_;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (loopFlag) buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	//ボリュームセット
	pSourceVoice->SetVolume(volum);
	pSourceVoice->SubmitSourceBuffer(&buf);
	pSourceVoice->SetFrequencyRatio(picth);
	pSourceVoice->Start();

	pSnd->sound_ = pSourceVoice;
}

SoundData* SoundManager::GetSoundData(const SoundKey& key)
{
	
	return &ssndMap_.at(key);
}

void SoundManager::Stop(const SoundKey& key)
{
	SoundData* pSnd = &ssndMap_[key];
	if (pSnd->sound_ != nullptr) {
		pSnd->sound_->Stop();
	}
}

void SoundManager::ReleaseAllSounds()
{
	for (auto itr = ssndMap_.begin(); itr != ssndMap_.end(); itr++)
	{
		//中身が入っていたらすべて止める
		if (itr->second.sound_ != nullptr) {
			itr->second.sound_->Stop();
		}
		itr->second.Release();
	}
	ssndMap_.clear();
}

