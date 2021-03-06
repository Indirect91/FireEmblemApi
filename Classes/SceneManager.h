#pragma once
#include "SingletonBase.h"

//▼마이어식 싱글톤이 아닌 싱글톤베이스 상속받아서 만드는방식도 해봄
class SceneManager : public SingletonBase<SceneManager>
{
public:
	//▼ 화면 전환 전에 어둡게 하는 용도의 커튼. 자료형식은 공개
	enum class tagCoverStatue : UINT
	{FadeOut, FadeIn, Idle};
private:

	//▼자료형 이름이 너무 길어서 재정의 해둔 것들
	typedef std::map<std::string,class GameNode*> mSceneList;
	typedef std::map<std::string,class GameNode*>::iterator miSceneList;

	//▼변수들
	float coverAlpha;				//커튼의 알파값
	float coverSpeed;				//커튼 제어속도
	mSceneList _mSceneList;			//각각의 씬(화면)들을 담아둘 맵
	tagCoverStatue coverStatue;		//커튼 여닫중인지 여부
	class GameNode* _currentScene;	//현재씬

public:
	SceneManager();
	~SceneManager();

	void Update(void);
	void Render(void);

	//▼커버 알파값 자체 제어
	float &RefcoverAlpha() { return coverAlpha; }
	//▼커버상태 참조자반환, 커튼상태 제어
	tagCoverStatue &RefAlphaStatue() { return coverStatue; }
	//▼커버속도 참조자 반환, 커튼속도 제어
	float &RefCoverSpeed() { return coverSpeed; }

	//▼씬추가
	class GameNode* AddScene(std::string sceneName,class GameNode* scene);
	//▼씬변경
	HRESULT LoadScene(std::string sceneName);
};

