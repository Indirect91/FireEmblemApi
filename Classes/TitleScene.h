#pragma once
#include "GameNode.h"
class TitleScene : public GameNode
{
	struct selectionTool
	{
		UINT x;		//박스 좌표
		UINT y;		//박스 좌표
	};
	
	BOOL isMaptool;		//맵툴이 픽된 여부
	BOOL isPlayPressed;	//플레이버튼 눌렸는지 여부
	BOOL isDone;		//윈도우 재배치 끝났는지 여부
	BOOL ascending;		//픽하는 빨간네모 알파값 상승중인지
	UINT Tcounter;		//그냥 카운터
	UINT falchionframeX;//팔시온 프레임 카운터X

	float pickAlpha;		//픽하는 네모 알파값
	float titleAlpha;		//픽하는 네모 알파값
	float moveWindowRateX;	//윈도우 이동시킬 수치
	float moveWindowRateY;	//윈도우 이동시킬 수치
	float moveWindowRateAccX; //윈도우 이동시킬 수치 가속
	float moveWindowRateAccY; //윈도우 이동시킬 수치 가속
	float resizeWindowRateX; //윈도우 크기 재조정할 수치
	float resizeWindowRateY; //윈도우 크기 재조정할 수치
	float resizeWindowRateAccX; //윈도우 크기 재조정할 수치
	float resizeWindowRateAccY; //윈도우 크기 재조정할 수치

	selectionTool selectionBox; //고르는 박스


public:

	void Init();
	void Release();
	void Update();
	void Render();

	TitleScene() {};
	~TitleScene() {};
};

