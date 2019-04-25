#include "../stdafx.h"
#include "Character.h"
#include "Item.h"
#include "Tiles.h"
#include "Cursor.h"

//���¥�� ��� �� ������� �ֵ��̱� ��
Character::Character()
{
	draggingIndex = index;
	draggindDirection = DraggingDirection::LEFT;
	charStatus = CharStatus::Idle;
	isInCamera = false;
	classes = Occupation::Infantary; //�ϴ��� ��� ĳ���� �������� ���
	portraitImg = nullptr;
	portraitAlpha = 0.f;
	frameImg = nullptr;
	item = nullptr;

	isActionTaken = false;
	frame = { 0 };
	frameLoop = 0;
	frameCounter = 0;
	frameInterval = 5;

	classHp = 0;
	classAttack = 0;
	classDefence = 0;
	classLuck = 0;
	classMove = 0;

	AdditionalHp = 0;
	AdditionalAttack = 0;
	AdditionalDefence = 0;
	AdditionalLuck = 0;
	AdditionalMove = 0;

	baseHp = 0;		
	baseAttack = 0;
	baseDefence = 0;
	baseLuck = 0;
	baseMove = 0;

	moveRangeCalculator = 0;
	isCalculated = false;
	tmpCursor = nullptr;
}

//����� Ŀ���� Ȱ��ȭ ��Ű�� �̴��� ���� ����
void Character::Init()
{
	tmpCursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
}

void Character::Release()
{
	//�������� �ܺμ� ������ �׳� �����ڳ� �����ͷ� ����ִ� ����̶� ���Ҵ��� ���⼭ ������
}

void Character::Update()
{	
	CheckInCamera(); //ī�޶���� üũ

	//����� ���¸� �ش�
	if (charStatus == CharStatus::IsClicked) //�� ĳ���� Ŭ�� ���̶�
	{
		if(PointCompare(index, draggingIndex))
			charStatus = CharStatus::IsClicked;//Ŭ���� �Ǿ��µ� ���� �ε����� �ٲ����� ���ٸ� ����
		else
			charStatus = CharStatus::IsDragging;//Ŭ�� �� Ŀ���� �̵������� �巡��
	} 
	else if(charStatus == CharStatus::IsDragging)
	{
		charStatus = CharStatus::IsDragging;//Ŭ�� �� Ŀ���� �̵������� �巡��
	}
	else if (PointCompare(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor")->GetIndex(), index)) //Ŀ�� �� ���µ� ����. ������ Ŀ��Ÿ���� �ٲ������ ����
	{
		charStatus = CharStatus::IsCursorOn;
	}
	else //Ŭ���� ���µ�, Ŀ��on ���µ� �ƴ϶��
	{	
		if (isInCamera) //�� ���� �ƴҶ� �ּ� ī�޶� �ӿ��� ���Դٸ�
			{charStatus = CharStatus::Idle;}
		else
			{charStatus = CharStatus::Disable;}
	}




	//����¿� ���� �ൿ
	switch (charStatus)
	{
		//��Ŭ���� ���¶��. 
	case CharStatus::IsClicked:
		//���Ŭ���̳� S�� ������ �Ȼ��� ����
		if (KEYMANAGER->IsOnceKeyDown('S'))
		{
			charStatus = CharStatus::IsCursorOn;
			DisableMoveRange();
			tmpCursor->SetCursorOccupied("");
		}
		//TODO: ȭ��ǥ
		draggingIndex = tmpCursor->GetIndex();
		if (!PointCompare(index, tmpCursor->GetIndex()))
		{
			charStatus = CharStatus::IsDragging;
			draggingIndex = tmpCursor->GetIndex();
		}


		break;

		//��Ŀ���� �ö���ִ� ���¶��.
	case CharStatus::IsCursorOn:
		if (!isCalculated) //�ѹ��� ������ ������� ���ٸ�
		{
			ShowMoveRange(); //���� ����ؼ� ������
			isCalculated = true; //�ѹ� ����ߴٰ� �˸�
		}

		//��� ���¿��� ���콺L��ư�̳� A��ư�� �������� üũ
		if (KEYMANAGER->IsOnceKeyDown('A'))
		{
			charStatus = CharStatus::IsClicked; //���ȴٸ� ���¸� Ŭ���� ���·� �ٲ�

			for (auto& toThicken : blueTiles)
			{
				toThicken->SetBlueAlpha(0.8f);
			}
		}
		break;

	//�� ���̵� �����϶�
	case CharStatus::Idle:
		DisableMoveRange();
		break;

	//�� �巡�� �����϶�
	case CharStatus::IsDragging:
		//���Ŭ���̳� S�� ������ �Ȼ��� ����
		if (KEYMANAGER->IsOnceKeyDown('S'))
		{
			charStatus = CharStatus::IsCursorOn;
			DisableMoveRange();
			tmpCursor->SetCursorOccupied("");
			tmpCursor->SetIndex({ index.x,index.y });
			tmpCursor->SetPositionViaIndex();
		}

		draggingIndex = tmpCursor->GetIndex();

		break;
	case CharStatus::Disable:

		break;
	}


	SetPositionViaIndex(); //���� ���� �ʿ�
}

//��ī�޶� �ӿ� ĳ���Ͱ� �ִ��� üũ�ϴ� �Լ�. ��ȯ�ϱ⵵ ��
BOOL Character::CheckInCamera()
{
	isInCamera = false;
	for (auto& toCheck : DATACENTRE.RefObjects(ObjType::ClippedTile))
	{
		if (toCheck.first == std::to_string(index.y * TILECOLX + index.x))
		{
			isInCamera = true;
			break;
		}
	}
	return isInCamera;
}

//���̵� ���� ���̰� ��
void Character::ShowMoveRange()
{
	//���� ĳ������ �������� �����
	moveRangeCalculator = AdditionalMove + classMove + baseMove;

	//���̵����� ������ ������ �Ͷ߸�
	assert(moveRangeCalculator > -1);

	MakeItBlue(index, moveRangeCalculator);

	//����͸� ���� ��� �����Ϳ� ����ϰ� ���� �λ깰 ��ó��
	for (auto& blueTile : blueTiles)
	{
		blueTile->IncreaseBlueNum();	//��� ���� ���� ����
		blueTile->SetIsChecked(false);	
	}
}
//���̵� ���� �Ⱥ��̰� ��
void Character::DisableMoveRange()
{
	for (auto &blueTile : blueTiles)
	{
		blueTile->DecreaseBlueNum();
	}
	isCalculated = false;
	blueTiles.clear();
}

//��Ÿ�� ���̰� ��ĥ�ϴ� ����Լ�
void Character::MakeItBlue(POINT _pos, UINT _move)
{
	//����� ù Ÿ�� ó��
	Tiles* startTile = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(_pos.y * TILECOLX + _pos.x)));
	startTile->SetIsChecked(_move);										//üũ�� ���� ����
	if (startTile->GetBlueAlpha() < 0.5f) startTile->SetBlueAlpha(0.2f);//���ϰ� ó���Ȱ� �ƴ϶�� ���� ���� ����
	blueTiles.insert(startTile);										//ó���� Ÿ�� Set�� �����

	//���� �˻�
	BOOL UpValidity = true;
	UpValidity &= (0 <= _pos.y-1);
	UpValidity &= _move > 0;
	if (UpValidity)
	{ 
		Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string((_pos.y - 1) * TILECOLX + _pos.x))); 
		UpValidity &= !checkTarget->GetIsChecked();
		UpValidity &= checkTarget->GetObjT() == "";

		if (UpValidity)
		{MakeItBlue({ _pos.x,_pos.y - 1 }, _move - 1);}
	}
	
	//���� �˻�
	BOOL LeftValidity = true;
	LeftValidity &= (0 <= _pos.x-1);
	LeftValidity &= _move > 0;
	if (LeftValidity)
	{
		Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(_pos.y  * TILECOLX + _pos.x-1)));
		LeftValidity &= !checkTarget->GetIsChecked();
		LeftValidity &= checkTarget->GetObjT() == "";

		if (LeftValidity)
		{
			MakeItBlue({ _pos.x - 1,_pos.y }, _move - 1);
		}
	}

	//��Ʒ� �˻�
	BOOL DownValidity = true;
	DownValidity &= (_pos.y + 1 < TILEROWY);
	DownValidity &= _move > 0;
	if (DownValidity)
	{
		Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string((_pos.y + 1) * TILECOLX + _pos.x)));
		DownValidity &= !checkTarget->GetIsChecked();
		DownValidity &= checkTarget->GetObjT() == "";

		if (DownValidity)
		{
			MakeItBlue({ _pos.x,_pos.y + 1 }, _move - 1);
		}
	}

	//��� �˻�
	BOOL RightValidity = true;
	RightValidity &= (_pos.x + 1 < TILECOLX);
	RightValidity &= _move > 0;
	if (RightValidity)
	{
		Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(_pos.y * TILECOLX + _pos.x+1)));
		RightValidity &= !checkTarget->GetIsChecked();
		RightValidity &= checkTarget->GetObjT() == "";

		if (RightValidity)
			{MakeItBlue({ _pos.x + 1,_pos.y }, _move - 1);}
	}
}

//�������ӵ����� �κ�
void Character::AdjustFrame()
{
	frameCounter++; //������ �׽� ����

	//��Ŭ���� �����϶�
	if (charStatus==CharStatus::IsClicked)
	{
		frameInterval = 2; //���� ������ �����δ�
	}
	//��׼��� �̹� ���� ���¶��
	else if (isActionTaken)
	{
		frameInterval = 16;
	}
	else
	{
		frameInterval = 7;
	}

	if (frameCounter % frameInterval == 0) //������ �����Ӱ��ݿ� ����
	{
		frameLoop++; //������ �̹��� ����
		if (frameLoop >= 4) frameLoop = 0; //��� ĳ���� �̹����� 0~3����
	}

	//���� UI�� �� �κ�
	if(charStatus==CharStatus::IsCursorOn || charStatus == CharStatus::IsClicked)
	{
		portraitAlpha += 0.1f;
		if(portraitAlpha>=1) portraitAlpha = 1;
	}
	else
	{
		portraitAlpha -= 0.1f;
		if (portraitAlpha <= 0) portraitAlpha = 0;
	}
}

void Character::SetOccupation(Occupation _job)
{
	this->classes = _job;
	switch (this->classes)
	{
	case Occupation::Infantary:
		classHp = 25;
		classAttack = 5;
		classDefence = 5;
		classLuck = 5;
		classMove = 4;
		break;
	case Occupation::Knight:
		classHp = 25;
		classAttack = 5;
		classDefence = 5;
		classLuck = 5;
		classMove = 6;
		break;
	case Occupation::Mage:
		classHp = 20;
		classAttack = 8;
		classDefence = 2;
		classLuck = 5;
		classMove = 4;
		break;
	case Occupation::Rogue:
		classHp = 20;
		classAttack = 8;
		classDefence = 2;
		classLuck = 10;
		classMove = 4;
		break;
	}
}

void Character::SetImg(std::string _CharName)
{
	portraitImg = IMAGEMANAGER->FindImage("�ʻ�ȭ" + _CharName);
	frameImg = IMAGEMANAGER->FindImage("ĳ����" + _CharName);
}

//�����ڰ� �޾ƿ°� �˾Ƽ� ����
void Character::SetInitialChar(Occupation _job, POINT _frame, std::string _charName, POINT _index)
{
	SetOccupation(_job);
	SetFrame(_frame);
	SetImg(_charName);
	SetIndex(_index);
	SetPositionViaIndex();
}

void Character::Render()
{
	if (isInCamera)
	{
		AdjustFrame(); //������ ������ �κ��� ���� �ƴҋ��� ���ư����ϴ� ������ ���� 

		frameImg->SetSize({ TILESIZE, TILESIZE });
		frameImg->RelativeFrameRender(position.left, position.top, frame.x + frameLoop, frame.y);

		if (isActionTaken)
		{
			IMAGEMANAGER->FindImage("ActionTaken")->SetAlpha(0.5f);
			IMAGEMANAGER->FindImage("ActionTaken")->SetSize({ TILESIZE, TILESIZE });
			IMAGEMANAGER->FindImage("ActionTaken")->RelativeRender(position.left, position.top);
		}

		portraitImg->SetAlpha(portraitAlpha);
		if (index.x == 6 && index.y == 6)
		{
			portraitImg->Render(900, 300);
		}
		else
		{
			portraitImg->Render(900, 100);
		}
	}
	if (charStatus == CharStatus::IsDragging)
	{
		frameImg->SetSize({ TILESIZE, TILESIZE });
		frameImg->SetAlpha(0.7f);
		frameImg->RelativeFrameRender(draggingIndex.x * TILESIZE, draggingIndex.y * TILESIZE, frame.x + frameLoop, frame.y + (UINT)draggindDirection);
	}
}