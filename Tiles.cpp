#include "stdafx.h"
#include "Tiles.h"

Tiles::Tiles() //�����ɶ��� �׳� ����·� ���
{
	rc = { 0 };
	object = "";
	terrain = "";
	objX = 0;
	objY = 0;
	counter = 0;
}

void Tiles::Init()
{
	rc = { 0 };
	object = "";
	terrain = "";
	objX = 0;
	objY = 0;
	counter = 0;
}

void Tiles::Release()
{
	//���Ҵ��Ѱ� ����
}

void Tiles::Update()
{
	//�������Ʈ�� ����������� �۵�
	if (object != "")
	{
		counter++;
		if (counter % 5 == 0)
		{
			objX++;
			if (objX > 4) objX = 0;
		}
	}
}

void Tiles::Render()
{
	//��Ÿ�Ͽ� ������ ��������� Ÿ���� ������
	if (terrain != "")
	{
		IMAGEMANAGER->FindImage(terrain)->Render(rc.left, rc.top);
		if (object != "")
		{
			IMAGEMANAGER->FindImage(object)->SetSize(IMAGEMANAGER->FindImage(object)->GetFrameSize());
			IMAGEMANAGER->FindImage(object)->FrameRender(rc.left, rc.top, objX, objY);
		}
	}
	else
	{
		D2DRENDERER->DrawRectangle(RelativeCameraRect(rc));
	}
}

