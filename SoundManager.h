#pragma once
#include "singletonBase.h"
/*FMOD ����ϱ� ���� ���̺귯�� �߰�*/
#include "SOUND/inc/fmod.hpp"
#pragma comment (lib, "SOUND/lib/fmodex_vc.lib")
using namespace FMOD;
//=============================================================
//	## soundManager ## (���� �Ŵ���)
//=============================================================

//����� ������� (���尹���� 30���� �Ѿ�� ũ�⸦ �� �÷���� �Ѵ�)
#define SOUNDBUFFER 30

class SoundManager : public SingletonBase<SoundManager>
{
private:
	typedef std::map<std::string, Sound**> arrSound;
	typedef std::map<std::string, Sound**>::iterator arrSoundIter;

private:
	System* _system;		//�ý��� Ŭ����
	Sound** _sound;			//���� Ŭ����
	Channel** _channel;		//ä�� Ŭ����

	arrSound _mTotalSound;	//�ʿ� ��Ƶ� �����

public:
	HRESULT Init(void);
	void Release(void);
	void Update(void);

	//���� �߰�(Ű��, �����̸�, BGM?, ������ų�ų�?)
	void addSound(std::string keyName, std::string soundName, bool bgm = false, bool loop = false);

	//���� ���
	void play(std::string keyName, float volume = 1.0f); //0.0f(Min) ~ 1.0f(Max)
	//���� ����
	void stop(std::string keyName);
	//���� �Ͻ�����
	void pause(std::string keyName);
	//���� �ٽ����
	void resume(std::string keyName);

	//�÷��� ���̳�?
	bool isPlaySound(std::string keyName);
	//�Ͻ����� ���̳�?
	bool isPauseSound(std::string keyName);

	SoundManager() : _system(NULL), _sound(NULL), _channel(NULL){}
	~SoundManager() {}
};

