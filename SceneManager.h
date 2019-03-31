#pragma once
#include "SingletonBase.h"
//=============================================================
//	## sceneManager ## (���Ŵ���)
//=============================================================

class SceneManager : public SingletonBase<SceneManager>
{
public:
	//�� ȭ�� ��ȯ ���� ��Ӱ� �ϴ� �뵵�� Ŀư. �ڷ������� ����
	enum class tagCoverStatue : UINT
	{FadeOut, FadeIn, Idle};
private:

	//���ڷ��� �̸��� �ʹ� �� ������ �ص� �͵�
	typedef std::map<std::string,class GameNode*> mSceneList;
	typedef std::map<std::string,class GameNode*>::iterator miSceneList;

	//�庯����
	float coverAlpha;				//Ŀư�� ���İ�
	float coverSpeed;				//Ŀư ����ӵ�
	mSceneList _mSceneList;			//������ ��(ȭ��)���� ��Ƶ� ��
	tagCoverStatue coverStatue;		//Ŀư ���������� ����
	class GameNode* _currentScene;	//�����

public:
	SceneManager();
	~SceneManager();

	void Update(void);
	void Render(void);


	//��Ŀ������ �����ڹ�ȯ, Ŀư���� ����
	tagCoverStatue &RefAlphaStatue() { return coverStatue; }; 
	//��Ŀ���ӵ� ������ ��ȯ, Ŀư�ӵ� ����
	float &RefCoverSpeed() { return coverSpeed; };

	//����߰�
	class GameNode* AddScene(std::string sceneName,class GameNode* scene);
	//�������
	HRESULT LoadScene(std::string sceneName);
};

