#include <cassert>
#include "mSound.h"

Microsoft::WRL::ComPtr<IXAudio2> SoundManager::xAudio2;
IXAudio2MasteringVoice* SoundManager::masterVoice;
std::map<SoundKey, SoundData> SoundManager::sndMap;

std::string directoryPath_ = "Resources/BGM_SE/";

SoundManager::~SoundManager()
{
}

SoundManager* SoundManager::GetInstance()
{
	static SoundManager obj;
	return &obj;
}

void SoundManager::Init()
{
	XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	xAudio2->CreateMasteringVoice(&masterVoice);
	sndMap.clear();
}

SoundKey SoundManager::LoadWave(std::string path, SoundKey key)
{
	std::ifstream file;
	
	std::string fullPath = directoryPath_ + path;

	file.open(fullPath, std::ios_base::binary);

	assert(file.is_open());

	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	FormatChunk format = {};

	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}

	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		file.seekg(data.size, std::ios_base::cur);

		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	file.close();

	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	sndMap.emplace(key, soundData);

	return key;
}

bool SoundManager::IsPlaying(SoundKey key) {
	IXAudio2SourceVoice* pSourceVoice = nullptr;//これ保存しとくと止められる
	SoundData* pSnd = &sndMap[key];

	xAudio2->CreateSourceVoice(&pSourceVoice, &pSnd->wfex);
	XAUDIO2_VOICE_STATE state{};
	pSourceVoice->GetState(&state);
	return false;
}

void SoundManager::Play(SoundKey key, bool loopFlag, float volum)
{
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	SoundData* pSnd = &sndMap[key];

	if (pSnd->sound != nullptr)
	{
		pSnd->sound->Stop();
	}

	xAudio2->CreateSourceVoice(&pSourceVoice, &pSnd->wfex);

	XAUDIO2_BUFFER buf{};

	buf.pAudioData = pSnd->pBuffer;
	buf.AudioBytes = pSnd->bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (loopFlag) buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	//ボリュームセット
	pSourceVoice->SetVolume(volum);
	pSourceVoice->SubmitSourceBuffer(&buf);
	pSourceVoice->Start();

	pSnd->sound = pSourceVoice;
}

SoundData* SoundManager::GetSoundData(SoundKey key)
{
	
	return &sndMap.at(key);
}

void SoundManager::Stop(SoundKey key)
{
	SoundData* pSnd = &sndMap[key];
	if (pSnd->sound != nullptr) {
		pSnd->sound->Stop();
	}
}

void SoundManager::ReleaseAllSounds()
{
	for (auto itr = sndMap.begin(); itr != sndMap.end(); itr++)
	{
		//中身が入っていたらすべて止める
		if (itr->second.sound != nullptr) {
			itr->second.sound->Stop();
		}
		itr->second.Release();
	}
	sndMap.clear();
}

