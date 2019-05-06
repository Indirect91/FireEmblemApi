#include "../stdafx.h"
#include "TitleScene.h"

TitleScene::TitleScene()
{
	isMaptool=0;
	isNewPressed=0;
	isContinuePressed = 0;
	isDone=0;
	ascending=0;
	Tcounter=0;
	Mcounter = 0;
	falchionframeX=0;
					
	 pickAlpha=0;
	 titleAlpha=0;
	 moveWindowRateX=0;
	 moveWindowRateY=0;
	 moveWindowRateAccX=0;
	 moveWindowRateAccY=0;
	 resizeWindowRateX=0;
	 resizeWindowRateY=0;
	 resizeWindowRateAccX=0;
	 resizeWindowRateAccY=0;
}


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
	
	//�尢�� �̴�
	SOUNDMANAGER->FadeIn();
	SOUNDMANAGER->play("Ÿ��ƲBGM"); //Ÿ��Ʋ BGM ���
	SCENEMANAGER->RefcoverAlpha() = 1;
	SetCoverStatue::FadeIn;
	selectionBox.x = ((WINSIZEX) / 2 - IMAGEMANAGER->FindImage("Ÿ��Ʋ���ùڽ�")->GetWidth() / 2); //�ȹڽ� ��ġ �ʱ�ȭ
	selectionBox.y = 385;		//�ȹڽ� ��ġ �ʱ�ȭ
	isMaptool = false;			//�������ÿ��� false
	isNewPressed = false;		//������ ���ÿ��� false
	isContinuePressed = false;	//��Ƽ�� ����
	isDone = false;				//������â �������� �������� ���� false
	Tcounter = 0;		
	Mcounter = 0;
	falchionframeX = 0;			//�Ƚÿ�������ī���� �ʱ� 0 ����
	pickAlpha = 0.f;			//��â ���İ� �ʱ� 0 ����
	titleAlpha = 0.f;			//Ÿ��Ʋ ���İ� �ʱ� 0 ����
}

void TitleScene::Release()
{
	//���Ҵ� ���ذ� �� ������ ����
}

void TitleScene::Update()
{
	Tcounter++;
	//��Ʒ�Ű �Է¹޾�����
	if ((KEYMANAGER->IsOnceKeyDown(VK_DOWN) || KEYMANAGER->IsOnceKeyDown('W'))&&SCENEMANAGER->RefAlphaStatue() == SceneManager::tagCoverStatue::Idle)
	{

		if (selectionBox.y == 385)
		{
			selectionBox.y = 450;
			SOUNDMANAGER->play("SelectionTitle");
		}
		else if (selectionBox.y == 450) 
		{ 
			selectionBox.y = 520; 
			SOUNDMANAGER->play("SelectionTitle");
		}
	}
	//��� Ű �Է¹޾�����
	else if ((KEYMANAGER->IsOnceKeyDown(VK_UP) || KEYMANAGER->IsOnceKeyDown('S')) && SCENEMANAGER->RefAlphaStatue() == SceneManager::tagCoverStatue::Idle)
	{
		if (selectionBox.y == 450)
		{
			selectionBox.y = 385;
			SOUNDMANAGER->play("SelectionTitle");
		}
		else if (selectionBox.y == 520)
		{
			selectionBox.y = 450;
			SOUNDMANAGER->play("SelectionTitle");
		}
	}

	//�忣��Ű �Է¹޾�����. ���İ��� �������̶�� ����
	else if ((KEYMANAGER->IsOnceKeyDown('A')||KEYMANAGER->IsOnceKeyDown(VK_RETURN)) && SCENEMANAGER->RefAlphaStatue()==SceneManager::tagCoverStatue::Idle)
	{
		//������� ���������
		if (selectionBox.y == 520) 
		{
			isMaptool = true; //���� ����� ���·� ���� ��ȯ
			moveWindowRateX = 160;	//������ �̵���ų ��ġ
			moveWindowRateY = 90;	
			resizeWindowRateX = 0;
			resizeWindowRateY = 0;
			SCENEMANAGER->RefCoverSpeed() = 0.04f;	//���̵�ƿ� �ӵ� ����
			SetCoverStatue::FadeOut; //���̵�ƿ����·� ��ȯ
			SOUNDMANAGER->play("SelectionTitleComplete");
			//SOUNDMANAGER->setFadeOut();
		}

		//���ΰ��� ���������
		else if (selectionBox.y == 385) 
		{
			isNewPressed = true;
			SCENEMANAGER->RefCoverSpeed() = 0.04f;
			SetCoverStatue::FadeOut;
			SOUNDMANAGER->play("SelectionTitleComplete");
			//SOUNDMANAGER->setFadeOut();

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
			if (resizeWindowRateX >= 500) resizeWindowRateX = 500;
			else
			{
				resizeWindowRateX += 22.5f;
			}
			if (resizeWindowRateY >= 260) resizeWindowRateY = 260;
			else
			{
				resizeWindowRateAccY += 1.04f;
				resizeWindowRateY += resizeWindowRateAccY;
			}

			SetWindowPos(_hWnd, 0, 150 + moveWindowRateX, 130 + moveWindowRateY, 1300 - resizeWindowRateX, 640 + resizeWindowRateY, 0);
			if (moveWindowRateX <= 0 && moveWindowRateY <= 0 && resizeWindowRateX >= 500 && resizeWindowRateY >= 260) isDone = true;

		}
		else
		{
			//�������� �̵��� �Ϸ�Ǿ�����
	
			SCENEMANAGER->LoadScene("MapToolScene");
		}
	}
	else if (isNewPressed)
	{
		Mcounter++;
		if (Mcounter > 50)
		SCENEMANAGER->LoadScene("Battle01");
	}
	else if (isContinuePressed)
	{
		Mcounter++;
		if(Mcounter>50)
		SCENEMANAGER->LoadScene("Battle01");
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

	//��� Ŀư�� ���� �ö��������� ���̾�� �ΰ� ���
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
	IMAGEMANAGER->FindImage("Ÿ��Ʋ�Ƚÿ�")->FrameRender(WINSIZEX/2,WINSIZEY/2+100,falchionframeX,0,Pivot::Centre);
	IMAGEMANAGER->FindImage("Ÿ��Ʋ�ؽ�Ʈ")->Render(0,0);
	IMAGEMANAGER->FindImage("Ÿ��Ʋ���ùڽ�")->Render(selectionBox.x, selectionBox.y);
}

