#include "stdafx.h"
#include "TitleScene.h"

void TitleScene::Init()
{
	//�������� �̵����� ���� �ʱ�ȭ
	moveWindowRateX = 0;
	moveWindowRateY = 0;
	moveWindowRateAccX = 0;
	moveWindowRateAccY = 0;
	resizeWindowRateX = 0;
	resizeWindowRateY = 0;
	resizeWindowRateAccX = 0;
	resizeWindowRateAccY = 0;

	//TODO : �ε������� �̵� �ʿ�
	IMAGEMANAGER->AddImage("Ÿ��Ʋ��׶���", L"IMAGE/TitleScene/TitleBackground.png");
	IMAGEMANAGER->AddFrameImage("Ÿ��Ʋ�Ƚÿ�", L"IMAGE/TitleScene/TitleFalchion.png", 34, 1);
	IMAGEMANAGER->AddImage("Ÿ��Ʋ���ùڽ�", L"IMAGE/TitleScene/TitlePick.png");
	IMAGEMANAGER->AddImage("Ÿ��Ʋ�ؽ�Ʈ", L"IMAGE/TitleScene/TitleLogo.png");
	SOUNDMANAGER->addSound("Ÿ��ƲBGM", "SOUND/FE/TitleBgm.mp3", true, true);
	
	//�尢�� �̴�
	SOUNDMANAGER->play("Ÿ��ƲBGM"); //Ÿ��Ʋ BGM ���
	selectionBox.x = ((WINSIZEX) / 2 - IMAGEMANAGER->FindImage("Ÿ��Ʋ���ùڽ�")->GetWidth() / 2); //�ȹڽ� ��ġ �ʱ�ȭ
	selectionBox.y = 385;	//�ȹڽ� ��ġ �ʱ�ȭ
	isMaptool = false;		//�������ÿ��� false
	isIngame = false;		//�ΰ��� ���ÿ��� false
	isDone = false;			//������â �������� �������� ���� false
	Tcounter = 0;		
	falchionframeX = 0;		//�Ƚÿ�������ī���� �ʱ� 0 ����
	pickAlpha = 0.f;		//��â ���İ� �ʱ� 0 ����
	titleAlpha = 0.f;		//Ÿ��Ʋ ���İ� �ʱ� 0 ����
}

void TitleScene::Release()
{
	//���Ҵ� ���ذ� �� ������ ����
}

void TitleScene::Update()
{
	Tcounter++;
	//��Ʒ�Ű �Է¹޾�����
	if (KEYMANAGER->IsOnceKeyDown(VK_DOWN) || KEYMANAGER->IsOnceKeyDown('W'))
	{
		if (selectionBox.y == 385)	selectionBox.y = 450;
		else if (selectionBox.y == 450) selectionBox.y = 520;
	}
	//��� Ű �Է¹޾�����
	else if (KEYMANAGER->IsOnceKeyDown(VK_UP) || KEYMANAGER->IsOnceKeyDown('S'))
	{
		if (selectionBox.y == 450) selectionBox.y = 385;
		else if (selectionBox.y == 520) selectionBox.y = 450;
	}
	//�忣��Ű �Է¹޾�����. ���İ��� �������̶�� ����
	else if (KEYMANAGER->IsOnceKeyDown(VK_RETURN) && SCENEMANAGER->RefAlphaStatue()==SceneManager::tagCoverStatue::Idle)
	{
		if (selectionBox.y == 520) //������ ���������
		{
			isMaptool = true; //���� ����� ���·� ���� ��ȯ
			moveWindowRateX = 160;	//������ �̵���ų ��ġ
			moveWindowRateY = 90;	
			resizeWindowRateX = 0;
			resizeWindowRateY = 0;
			SCENEMANAGER->RefCoverSpeed() = 0.04f;	//���̵�ƿ� �ӵ� ����
			SetCoverStatue::FadeOut; //���̵�ƿ����·� ��ȯ
		}
		else if (selectionBox.y == 385) //�ΰ��� ���������
		{
			isIngame = true;
			moveWindowRateX = 0;
			moveWindowRateY = 10;
			SCENEMANAGER->RefCoverSpeed() = 0.04f;
			SetCoverStatue::FadeOut;
		}
	}

	//������� ����� �����϶�
	if (isMaptool) 
	{
		//������ �̵� �Ϸᰡ �ƴҶ�
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
			//�������� �̵��� �Ϸ�Ǿ�����
			
			SCENEMANAGER->LoadScene("MapToolScene");
		}
	}

	//�����̾�� �ø��� ���� �������� ��� �Ƚÿ��� ������
	if (Tcounter % 3 == 0 && Tcounter>54)
	{
		falchionframeX += 1;
		if (falchionframeX > 33)
			falchionframeX = 0;
		if (Tcounter > 153)
			Tcounter = 0;
	}

	//���ȳ׸� ���İ� �ö󰡰�������
	if (ascending&&SCENEMANAGER->RefAlphaStatue() == SceneManager::tagCoverStatue::Idle)
	{
		pickAlpha += 0.01f; //���� ����
		if (pickAlpha >= 1.f)
		{
			pickAlpha = 1.f;
			ascending = false;
		}
	}
	//���ȳ׸� ���İ� ��������������
	else if(ascending==false && SCENEMANAGER->RefAlphaStatue() == SceneManager::tagCoverStatue::Idle)//�ƴҶ�
	{
		pickAlpha -= 0.01f; //���� ����
		if (pickAlpha <= 0.3f)
		{
			pickAlpha = 0.3f; //����ġ 0.3������
			ascending = true;
		}
	}

	//�� Ŀư�� ���� �ö��������� ���̾�� �ΰ� ���
	if (SCENEMANAGER->RefAlphaStatue() == SceneManager::tagCoverStatue::Idle)
	{
		titleAlpha += 0.01f;
		if (titleAlpha > 1.f) titleAlpha = 1.f;
	}

	//����İ� �׽� ����
	IMAGEMANAGER->FindImage("Ÿ��Ʋ���ùڽ�")->SetAlpha(pickAlpha);
	IMAGEMANAGER->FindImage("Ÿ��Ʋ�ؽ�Ʈ")->SetAlpha(titleAlpha);
}

void TitleScene::Render()
{
	//�幹 �������� �ʹ� �̸��� ��Ȯ�ؼ� ���̻� ������ �ʿ����
	IMAGEMANAGER->FindImage("Ÿ��Ʋ��׶���")->Render(0, 0); 
	IMAGEMANAGER->FindImage("Ÿ��Ʋ�Ƚÿ�")->SetSize(IMAGEMANAGER->FindImage("Ÿ��Ʋ�Ƚÿ�")->GetFrameSize());
	IMAGEMANAGER->FindImage("Ÿ��Ʋ�Ƚÿ�")->SetReverseY(true);
	IMAGEMANAGER->FindImage("Ÿ��Ʋ�Ƚÿ�")->FrameRender(WINSIZEX/2,WINSIZEY/2+100,falchionframeX,0,Pivot::Center);
	IMAGEMANAGER->FindImage("Ÿ��Ʋ�ؽ�Ʈ")->Render(0,0);
	IMAGEMANAGER->FindImage("Ÿ��Ʋ���ùڽ�")->Render(selectionBox.x, selectionBox.y);
}
