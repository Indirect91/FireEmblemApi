#include "../stdafx.h"
#include "Cursor.h"
#include "Tiles.h"


//��Ŀ�� ������
Cursor::Cursor()
{
	cursorFrame = 0;								//Ŀ�� ������
	cursorCounter = 0;								//Ŀ�� �����ӿø� ī����
	cursorState = CursorState::cursorDeactivated;	//Ŀ�� ��Ȱ��ȭ ���
	cursorTurn = IngameStatus::PlayerTurn;			//�ϴ��� �÷��̾� �� ������ ���
	cursorTurnPrev = IngameStatus::PlayerTurn;			//�ϴ��� �÷��̾� �� ������ ���
	cursorOccupied = "";							//Ŀ�� ������ ĳ���͸�
	prevLocation = index;							//Ŀ�� �巡�׽� ��������
}

//��Ŀ�� �̴�
void Cursor::Init()
{
	this->index = {5,5};//�ϴ� {5,5} �� ���

	//���������� ��¥�� ����ȵ�. �� �÷��̾��� �δ밡 ���� �����Ѵٸ� ù° �Ʊ��� �����͸� ������
	for (auto& getFirst : DATACENTRE.RefObjects(ObjType::PlayerArmy))
	{
		this->index = getFirst.second->GetIndex();
		break;
	}

	cursorFrame = 0;				//Ŀ�� ������
	cursorCounter = 0;				//Ŀ���� �ʹ� Ŀ�� �����ӵ��� ī����
	SetPositionViaIndex();			//Ŀ�� �����ǵ� �ε����� �°� ����

}

//��Ŀ�� ������
void Cursor::Release()
{
	//���Ҵ� ����
}

//��Ŀ�� ������Ʈ
void Cursor::Update()
{
	//����°��� ���� �ൿ
	switch (cursorState)
	{
	case Cursor::CursorState::cursorActivated:
		for (auto& clipped : clippedTiles)
		{
			RECT RelativeRc = CAMERA.RelativeCameraRect(clipped.second->GetPosition());
			if (PtInRect(&RelativeRc, _ptMouse))
			{
				SetIndex(clipped.second->GetIndex());
				SetPositionViaIndex();
				break;
			}
		}
		break;
	case Cursor::CursorState::cursorDeactivated: 
		break;
	}
	CursorFrameManage();
}


//��Ŀ���� ������ ����
void Cursor::CursorFrameManage()
{
	cursorCounter++;
	if (cursorCounter % 10 == 0)
	{
		cursorFrame++;
		if (cursorFrame > 3)cursorFrame = 0;
	}
}


//���̵��� �����ߴٸ� �̵��ϰ� ������ True�� ��ȯ
BOOL Cursor::MoveUp()
{
	if (cursorOccupied != "")
	{
		if (DATACENTRE.CheckObjectExistance(ObjType::BlueTiles, TwoDimentionArrayToOneString({ index.x,index.y - 1 }, TILECOLX))
			|| DATACENTRE.CheckObjectExistance(ObjType::FoeTiles, TwoDimentionArrayToOneString({ index.x,index.y - 1 }, TILECOLX)))
		{
			index.y -= 1;
			SetPositionViaIndex();
			return true;
		}
	}
	else if (index.y == 0)
	{
		return false;
	}
	else
	{
		index.y -= 1;
		SetPositionViaIndex();
		return true;
	}
}

//���̵��� �����ߴٸ� �̵��ϰ� ������ True�� ��ȯ
BOOL Cursor::MoveDown()
{
	if (cursorOccupied != "")
	{
		if (DATACENTRE.CheckObjectExistance(ObjType::BlueTiles, TwoDimentionArrayToOneString({ index.x,index.y + 1 }, TILECOLX))
			|| DATACENTRE.CheckObjectExistance(ObjType::FoeTiles, TwoDimentionArrayToOneString({ index.x,index.y + 1 }, TILECOLX)))
		{
			index.y += 1;
			SetPositionViaIndex();
			return true;
		}
	}
	else if (index.y == TILEROWY)
	{
		return false;
	}
	else
	{
		index.y += 1;
		SetPositionViaIndex();
		return true;
	}
}

//���̵��� �����ߴٸ� �̵��ϰ� ������ True�� ��ȯ
BOOL Cursor::MoveLeft()
{
	if (cursorOccupied != "")
	{
		if (DATACENTRE.CheckObjectExistance(ObjType::BlueTiles, TwoDimentionArrayToOneString({ index.x - 1,index.y }, TILECOLX))
			|| DATACENTRE.CheckObjectExistance(ObjType::FoeTiles, TwoDimentionArrayToOneString({ index.x - 1,index.y }, TILECOLX)))
		{
			index.x -= 1;
			SetPositionViaIndex();
			return true;
		}
	}
	else if (index.x == 0)
	{
		return false;
	}
	else
	{
		index.x -= 1;
		SetPositionViaIndex();
		return true;
	}
}

//���̵��� �����ߴٸ� �̵��ϰ� ������ True�� ��ȯ
BOOL Cursor::MoveRight()
{
	if (cursorOccupied != "")
	{
		if (DATACENTRE.CheckObjectExistance(ObjType::BlueTiles, TwoDimentionArrayToOneString({ index.x + 1,index.y }, TILECOLX))
			|| DATACENTRE.CheckObjectExistance(ObjType::FoeTiles, TwoDimentionArrayToOneString({ index.x + 1,index.y }, TILECOLX)))
		{
			index.x += 1;
			SetPositionViaIndex();
			return true;
		}
	}
	else if (index.x == TILECOLX)
	{
		return false;
	}
	else
	{
		index.x += 1;
		SetPositionViaIndex();
		return true;
	}
}

//�巣��.
void Cursor::Render()
{	
	switch (cursorTurn)
	{
	case IngameStatus::PlayerTurn:
		IMAGEMANAGER->FindImage("CursorBlue")->Render(_ptMouse.x, _ptMouse.y);
		break;

	case IngameStatus::EnemyTurn:
		IMAGEMANAGER->FindImage("CursorRed")->Render(_ptMouse.x, _ptMouse.y);
		break;

	case IngameStatus::ESCMenu:
	case IngameStatus::SelectionUI:
	case IngameStatus::StartPlacement:
	case IngameStatus::TurnChanging:
		IMAGEMANAGER->FindImage("Cursor")->Render(_ptMouse.x, _ptMouse.y);
		break;
	}

	IMAGEMANAGER->FindImage("Ÿ��Ŀ��")->SetSize({ TILESIZE, TILESIZE });
	IMAGEMANAGER->FindImage("Ÿ��Ŀ��")->RelativeFrameRender(index.x * TILESIZE, index.y * TILESIZE, cursorFrame, 0);

	IMAGEMANAGER->FindImage("CursorBattle")->SetAlpha(isOtherUnitOn.name!="");
	IMAGEMANAGER->FindImage("CursorBattle")->SetSize({32,32});
	IMAGEMANAGER->FindImage("CursorBattle")->RelativeRender(index.x * TILESIZE +8, (index.y * TILESIZE)-30);

}