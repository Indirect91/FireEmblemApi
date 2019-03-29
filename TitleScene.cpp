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
	Tcounter = 0;		
	falchionframeX = 0;		//팔시온프레임카운터 초기 0 설정
	pickAlpha = 0.f;		//픽창 알파값 초기 0 설정
	titleAlpha = 0.f;		//타이틀 알파값 초기 0 설정
}

void TitleScene::Release()
{
	//뉴할당 해준거 현 씬에선 없음
}

void TitleScene::Update()
{
	Tcounter++;
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
			if (resizeWindowRateY >= 260) resizeWindowRateY = 260;
			else
			{
				resizeWindowRateAccY += 1.04f;
				resizeWindowRateY += resizeWindowRateAccY;
			}

			SetWindowPos(_hWnd, 0, 150 + moveWindowRateX, 130 + moveWindowRateY, 1300 - resizeWindowRateX, 640 + resizeWindowRateY, 0);
			if (moveWindowRateX <= 0 && moveWindowRateY <= 0 && resizeWindowRateX >= 300 && resizeWindowRateY >= 260) isDone = true;

		}
		else
		{
			//▼윈도우 이동이 완료되었을떄
			
			SCENEMANAGER->LoadScene("MapToolScene");
		}
	}

	//▼파이어엠블렘 시리즈 대대로 내려오는 명검 팔시온의 프레임
	if (Tcounter % 3 == 0 && Tcounter>54)
	{
		falchionframeX += 1;
		if (falchionframeX > 33)
			falchionframeX = 0;
		if (Tcounter > 153)
			Tcounter = 0;
	}

	//▼픽네모 알파가 올라가고있을때
	if (ascending&&SCENEMANAGER->RefAlphaStatue() == SceneManager::tagCoverStatue::Idle)
	{
		pickAlpha += 0.01f; //알파 증가
		if (pickAlpha >= 1.f)
		{
			pickAlpha = 1.f;
			ascending = false;
		}
	}
	//▼픽네모 알파가 내려가고있을때
	else if(ascending==false && SCENEMANAGER->RefAlphaStatue() == SceneManager::tagCoverStatue::Idle)//아닐때
	{
		pickAlpha -= 0.01f; //알파 감소
		if (pickAlpha <= 0.3f)
		{
			pickAlpha = 0.3f; //최저치 0.3까지만
			ascending = true;
		}
	}

	//씬 커튼이 전부 올라갔을때부터 파이어엠블렘 로고 출력
	if (SCENEMANAGER->RefAlphaStatue() == SceneManager::tagCoverStatue::Idle)
	{
		titleAlpha += 0.01f;
		if (titleAlpha > 1.f) titleAlpha = 1.f;
	}

	//▼알파값 항시 변동
	IMAGEMANAGER->FindImage("타이틀선택박스")->SetAlpha(pickAlpha);
	IMAGEMANAGER->FindImage("타이틀텍스트")->SetAlpha(titleAlpha);
}

void TitleScene::Render()
{
	//▼뭐 랜더할지 너무 이름이 명확해서 더이상 설명이 필요없다
	IMAGEMANAGER->FindImage("타이틀백그라운드")->Render(0, 0); 
	IMAGEMANAGER->FindImage("타이틀팔시온")->SetSize(IMAGEMANAGER->FindImage("타이틀팔시온")->GetFrameSize());
	IMAGEMANAGER->FindImage("타이틀팔시온")->SetReverseY(true);
	IMAGEMANAGER->FindImage("타이틀팔시온")->FrameRender(WINSIZEX/2,WINSIZEY/2+100,falchionframeX,0,Pivot::Center);
	IMAGEMANAGER->FindImage("타이틀텍스트")->Render(0,0);
	IMAGEMANAGER->FindImage("타이틀선택박스")->Render(selectionBox.x, selectionBox.y);
}
