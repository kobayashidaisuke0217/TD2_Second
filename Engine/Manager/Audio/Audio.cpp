#include "Audio.h"
#include <cassert>

Audio* Audio::GetInstance() {
	static Audio instance;

	return &instance;
}

void Audio::Initialize() {
	HRESULT result;
	// Xaudio2のインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	audioHandle_ = -1;
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();
	const char* groupName = "SoundVolume";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Ingame", volume[inGameBGM]);
	globalVariables->AddItem(groupName, "Result", volume[ResultBGM]);
	globalVariables->AddItem(groupName, "Jump", volume[Jump]);
	globalVariables->AddItem(groupName, "start", volume[GameStart]);
	globalVariables->AddItem(groupName, "Reverce", volume[Reverce]);
	globalVariables->AddItem(groupName, "Death", volume[Death]);
	globalVariables->AddItem(groupName, "DeleteEnemy", volume[DeleteEnemy]);
	globalVariables->AddItem(groupName, "Block", volume[Block]);
	ApplyGlobalVariables();
}
void Audio::Update()
{
	ApplyGlobalVariables();
	/*for (int i = 0; i < 20; i++) {
		volume[i] = std::clamp(volume[i], 0.0f, 1.0f);
	}*/
}
uint32_t Audio::SoundLoadWave(const char* filename) {
	audioHandle_++;
	// .wavファイルを開く
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	// ファイルオープン失敗を検出する
	assert(file.is_open());

	// .wavデータ読み込み
	// RIFFヘッダの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルはRIFF?
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	// タイプはWAVE?
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}
	// Formatチャンクの読み込み
	FormatChunk format = {};
	// チャンクの読み込み
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);
	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	// Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Waveファイルを閉じる
	file.close();

	// returnするための音声データ
	soundDatas[audioHandle_].wfex = format.fmt;
	soundDatas[audioHandle_].pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundDatas[audioHandle_].bufferSize = data.size;

	return audioHandle_;
}

void Audio::SoundUnload(SoundData* soundData) {
	// バッファのメモリを解放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void Audio::SoundPlayWave(uint32_t audioHandle) {
	HRESULT result;

	//波形フォーマットもとにSourceVoiceの生成

	sourceVoice[audioHandle] = nullptr;
	result = xAudio2->CreateSourceVoice(&sourceVoice[audioHandle], &soundDatas[audioHandle].wfex);
	sourceVoice[audioHandle]->SetVolume(volume[audioHandle]);
	assert(SUCCEEDED(result));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundDatas[audioHandle].pBuffer;
	buf.AudioBytes = soundDatas[audioHandle].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// 波形データの再生

	result = sourceVoice[audioHandle]->SubmitSourceBuffer(&buf);
	result = sourceVoice[audioHandle]->Start();

}
void Audio::SoundPlayloop(uint32_t audioHandle) {
	HRESULT result;

	//波形フォーマットもとにSourceVoiceの生成
	result = xAudio2->CreateSourceVoice(&sourceVoice[audioHandle], &soundDatas[audioHandle].wfex);
	sourceVoice[audioHandle]->SetVolume(volume[audioHandle]);
	assert(SUCCEEDED(result));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundDatas[audioHandle].pBuffer;
	buf.AudioBytes = soundDatas[audioHandle].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = XAUDIO2_LOOP_INFINITE;

	// 波形データの再生
	result = sourceVoice[audioHandle]->SubmitSourceBuffer(&buf);
	result = sourceVoice[audioHandle]->Start();

}

void Audio::StopAudio(uint32_t audioHandle)
{
	HRESULT result;
	result = sourceVoice[audioHandle]->Stop(0);

}

void Audio::ApplyGlobalVariables()
{
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();
	const char* groupName = "SoundVolume";
	globalVariables->CreateGroup(groupName);
	volume[6] = globalVariables->GetFloatValue(groupName, "Ingame");
	volume[7] = globalVariables->GetFloatValue(groupName, "Result");
	volume[Jump] = globalVariables->GetFloatValue(groupName, "Jump");
	volume[GameStart] = globalVariables->GetFloatValue(groupName, "start");
	volume[Reverce] = globalVariables->GetFloatValue(groupName, "Reverce");
	volume[Death] = globalVariables->GetFloatValue(groupName, "Death");
	volume[DeleteEnemy] = globalVariables->GetFloatValue(groupName, "DeleteEnemy");

		volume[Block] = globalVariables->GetFloatValue(groupName, "Block");
		
}
