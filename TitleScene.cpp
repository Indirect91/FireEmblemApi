#include "stdafx.h"
#include "TitleScene.h"

void TitleScene::Init()
{
	//▼윈도우 이동관련 변수 초기화
	moveWindowRateX = 0;
	moveWindowRateY = 0;
	moveWindowRateAccX = 0;
	moveWindowRateAccY = 0;
	resizeWindowRateX = 0;
	resizeWindowRateY = 0;
	resizeWindowRateAccX = 0;
	resizeWindowRateAccY = 0;

	//TODO : 로딩씬으로 이동 필요
	IMAGEMANAGER->AddImage("타이틀백그라운드", L"IMAGE/TitleScene/TitleBackground.png");
	IMAGEMANAGER->AddFrameImage("타이틀팔시온", L"IMAGE/TitleScene/TitleFalchion.png", 34, 1);
	IMAGEMANAGER->AddImage("타이틀선택박스", L"IMAGE/TitleScene/TitlePick.png");
	IMAGEMANAGER->AddImage("타이틀텍스트", L"IMAGE/TitleScene/TitleLogo.png");
	SOUNDMANAGER->addSound("타이틀BGM", "SOUND/FE/TitleBgm.mp3", true, true);
	
	//▼각종 이닛
	SOUNDMANAGER->play("타이틀BGM"); //타이틀 BGM 재생
	selectionBox.x = ((WINSIZEX) / 2 - IMAGEMANAGER->FindImage("타이틀선택박스")->GetWidth() / 2); //픽박스 위치 초기화
	selectionBox.y = 385;	//픽박스 위치 초기화
	isMaptool = false;		//맵툴선택여부 false
	isIngame = false;		//인게임 선택여부 false
	isDone = false;			//윈도우창 재조정이 끝났는지 여부 false
	frameCounter = 0;		//팔시온프레임카운터 초기 0 설정
	pickAlpha = 0.f;		//픽창 알파값 초기 0 설정
}

void TitleScene::Release()
{
	//뉴할당 해준거 현 씬에선 없음
}

void TitleScene::Update()
{
	//▼아래키 입력받았을때
	if (KEYMANAGER->IsOnceKeyDown(VK_DOWN) || KEYMANAGER->IsOnceKeyDown('W'))
	{
		if (selectionBox.y == 385)	selectionBox.y = 450;
		else if (selectionBox.y == 450) selectionBox.y = 520;
	}
	//▼업 키 입력받았을때
	else if (KEYMANAGER->IsOnceKeyDown(VK_UP) || KEYMANAGER->IsOnceKeyDown('S'))
	{
		if (selectionBox.y == 450) selectionBox.y = 385;
		else if (selectionBox.y == 520) selectionBox.y = 450;
	}
	//▼엔터키 입력받았을때. 알파값이 변동중이라면 씹음
	else if (KEYMANAGER->IsOnceKeyDown(VK_RETURN) && SCENEMANAGER->RefAlphaStatue()==SceneManager::tagCoverStatue::Idle)
	{
		if (selectionBox.y == 520) //맵툴이 골라졌을때
		{
			isMaptool = true; //맵툴 골라진 상태로 상태 변환
			moveWindowRateX = 160;	//윈도우 이동시킬 수치
			moveWindowRateY = 90;	
			resizeWindowRateX = 0;
			resizeWindowRateY = 0;
			SCENEMANAGER->RefCoverSpeed() = 0.04f;	//페이드아웃 속도 제어
			SetCoverStatue::FadeOut; //페이드아웃상태로 변환
		}
		else if (selectionBox.y == 385) //인겜이 골라졌을때
		{
			isIngame = true;
			moveWindowRateX = 0;
			moveWindowRateY = 10;
			SCENEMANAGER->RefCoverSpeed() = 0.04f;
			SetCoverStatue::FadeOut;
		}
	}

	//▼맵툴이 골라진 상태일때
	if (isMaptool) 
	{
		//▼윈도 이동 완료가 아닐때
		if (!isDone)
		{
			if (moveWindowRateY <= 0) moveWindowRateY = 0;
			else
			{
				moveWindowRateAccY += 0.4f;
				moveWindowRateY -= moveWindowRateAccY;
			}
			if (moveWindowRateX <= 0) moveWindowRateX = 0;
			else
			{
				moveWindowRateAccX += 0.7f;
				moveWindowRateX -= moveWindowRateAccX;
			}
			if (resizeWindowRateX >= 300) resizeWindowRateX = 300;
			else
			{
				resizeWindowRateX += 13.5f;
			}
			if (resizeWindowRateY >= 240) resizeWindowRateY = 240;
			else
			{
				resizeWindowRateAccY += 1.04f;
				resizeWindowRateY += resizeWindowRateAccY;
			}

			SetWindowPos(_hWnd, 0, 150 + moveWindowRateX, 130 + moveWindowRateY, 1300 - resizeWindowRateX, 640 + resizeWindowRateY, 0);
			if (moveWindowRateX == 0 && moveWindowRateY == 0 && resizeWindowRateX == 100 && resizeWindowRateY == 240) isDone = true;

		}
		else
		{
			Sleep(500);
			SCENEMANAGER->LoadScene("맵툴");
		}
	}

	//▼픽네모 알파가 올라가고있을때
	if (ascending) 
	{
		pickAlpha += 0.01f; //알파 증가
		if (pickAlpha >= 1.f)
		{
			pickAlpha = 1.f;
			ascending = false;
		}
	}
	else //아닐때
	{
		pickAlpha -= 0.01f; //알파 감소
		if (pickAlpha <= 0.3f)
		{
			pickAlpha = 0.3f; //최저치 0.3까지만
			ascending = true;
		}
	}

	//▼픽값 항시 변동
	IMAGEMANAGER->FindImage("타이틀선택박스")->SetAlpha(pickAlpha);
}

void TitleScene::Render()
{
	//▼뭐 랜더할지 너무 이름이 명확해서 더이상 설명이 필요없다
	IMAGEMANAGER->FindImage("타이틀백그라운드")->Render(0, 0); 
	IMAGEMANAGER->FindImage("타이틀텍스트")->Render(0,0);
	IMAGEMANAGER->FindImage("타이틀선택박스")->Render(selectionBox.x, selectionBox.y);
	
}
