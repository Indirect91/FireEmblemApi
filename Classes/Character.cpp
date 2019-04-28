#include "../stdafx.h"
#include "Character.h"
#include "Item.h"
#include "Tiles.h"
#include "Cursor.h"

//�������
Character::Character()
{
	whosChar = OwnedBy::Ally;
	draggingIndex = index;
	draggindDirection = DraggingDirection::LEFT;
	charStatus = CharStatus::Idle;
	isInCamera = false;
	occupation = Occupation::Archer;
	portraitImg = nullptr;
	portraitAlpha = 0.f;
	frameImg = nullptr;
	item = nullptr;

	isActionTaken = false;
	frame = { 0 };
	frameLoop = 0;
	frameCounter = 0;
	frameInterval = 5;

	occupationData = { 0 };
	ZeroMemory(&additionalData, sizeof(additionalData));
	memset(&baseData, 0, sizeof(baseData));

	RangeCalculator = 0;
	isMoveCalculated = false;
	isRangeCalculated = false;
	cursor = nullptr;
	dragValidity = false;
	isActionTaken = false;
}

//������ ĳ�� ������ ���� ������ �����ε�
Character::Character(Occupation _job, std::string _charName, POINT _index, OwnedBy _whos) : whosChar(_whos)
{
	index = _index;
	draggingIndex = _index;
	draggindDirection = DraggingDirection::LEFT;
	charStatus = CharStatus::Idle;
	isInCamera = false;
	occupation = _job;
	portraitImg = nullptr;
	portraitAlpha = 0.f;
	frameImg = nullptr;
	item = nullptr;
	isActionTaken = false;
	frame.x = static_cast<UINT>(_whos);
	frame.y = static_cast<UINT>(_job);
	frameLoop = 0;
	frameCounter = 0;
	frameInterval = 5;

	occupationData = { 0 };
	ZeroMemory(&additionalData, sizeof(additionalData));
	memset(&baseData, 0, sizeof(baseData));

	RangeCalculator = 0;
	isMoveCalculated = false;
	isRangeCalculated = false;
	cursor = nullptr;
	dragValidity = false;
	isActionTaken = false;

	SetOccupation(_job);
	SetImg(_charName);
	SetPositionViaIndex();
}

//����� Ŀ���� Ȱ��ȭ ��Ű�� �̴��� ���� ����
void Character::Init()
{
	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
	assert(cursor != nullptr); //Ŀ�� ����� �ȵ������� �Ͷ߸�
	isActionTaken = false;
}

void Character::Release()
{
	//�������� �ܺμ� ������ �׳� �����ڳ� �����ͷ� ����ִ� ����̶� ���Ҵ��� ���⼭ ������
}

void Character::Update()
{
	CheckInCamera(); //ī�޶���� üũ

	if (cursor->GetCursorTurn() == Cursor::CursorTurn::PlayerTurn && whosChar == OwnedBy::Player)
	{
		//����� ���¸� �ش�
		if (isActionTaken) //�ൿ�� �������� �ִ� ĳ���Ͷ��
		{
			charStatus = CharStatus::IsActed;
		} //�ൿ�� ���·� ����
		else if (charStatus == CharStatus::IsClicked) //�� ĳ���� Ŭ�� ���̶�
		{
			if (PointCompare(index, draggingIndex))
				charStatus = CharStatus::IsClicked;//Ŭ���� �Ǿ��µ� ���� �ε����� �ٲ����� ���ٸ� ����
			else
				charStatus = CharStatus::IsDragging;//Ŭ�� �� Ŀ���� �̵������� �巡��
		}
		else if (charStatus == CharStatus::IsDragging)
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
			{
				charStatus = CharStatus::Idle;
			}
			else
			{
				charStatus = CharStatus::Disable;
			}
		}
	}
	else if (cursor->GetCursorTurn() == Cursor::CursorTurn::PlayerTurn && whosChar == OwnedBy::Enemy)
	{
		charStatus = CharStatus::IsCheckingOut;
	}
	else if (cursor->GetCursorTurn() == Cursor::CursorTurn::EnemyTurn)
	{

	}
	else
	{
		assert(false && "Turn Error");
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
			cursor->SetCursorOccupied("");
		}
		//TODO: ȭ��ǥ
		draggingIndex = cursor->GetIndex();
		if (!PointCompare(index, cursor->GetIndex()))
		{
			charStatus = CharStatus::IsDragging;
			draggingIndex = cursor->GetIndex();
		}


		break;

		//��Ŀ���� �ö���ִ� ���¶��.
	case CharStatus::IsCursorOn:
		if (!isMoveCalculated) //�ѹ��� ������ ������� ���ٸ�
		{
			ShowMoveRange(&Character::BlueRedShow); //���� ����ؼ� ������
			isMoveCalculated = true; //�ѹ� ����ߴٰ� �˸�
		}

		//��� ���¿��� ���콺L��ư�̳� A��ư�� �������� üũ
		if (KEYMANAGER->IsOnceKeyDown('A'))
		{
			charStatus = CharStatus::IsClicked; //���ȴٸ� ���¸� Ŭ���� ���·� �ٲ�

			for (auto& toThickenBlue : blueTiles)
			{
				toThickenBlue->SetBlueAlpha(0.8f);
			}
			for (auto& toThickenRed : redTiles)
			{
				toThickenRed->SetRedAlpha(0.8f);
			}
			for (auto& toThickenGreen : greenTiles)
			{
				toThickenGreen->SetGreenAlpha(0.8f);
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
			cursor->SetCursorOccupied("");
			cursor->SetIndex({ index.x,index.y });
			cursor->SetPositionViaIndex();
		}

		draggingIndex = cursor->GetIndex();

		break;
	case CharStatus::Disable:

		break;
	case CharStatus::IsActed:
		//TODO : ESC�޴� ������
		break;
	case CharStatus::IsCheckingOut:

		if (!isMoveCalculated) //�ѹ��� ������ ������� ���ٸ�
		{
			ShowMoveRange(&Character::BlueRedShow); //���� ����ؼ� ������
			isMoveCalculated = true; //�ѹ� ����ߴٰ� �˸�
		}

		break;

	default:
		assert((BOOL)charStatus || "missing character status"); //���� �ȸ���� �ִٴ� ���̴� ���� �Ͷ߸�
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
			return isInCamera;
			break;
		}
	}
	return isInCamera;
}

//���̵� ���� ���̰� ��
void Character::ShowMoveRange(void (Character:: * colourShow)(INT))
{

	//���� ĳ������ �ൿ ���� ������ ǥ����
	INT ActionRange = additionalData.Range + occupationData.Range + baseData.Range;  //��Ÿ� or ������
	INT MoveRange = additionalData.Move + occupationData.Move + baseData.Move;		//�̵�����
	RangeCalculator = ActionRange + MoveRange;										//�ൿ+�̵�����

	//��0������ ���� ������ �Ͷ߸�
	assert(ActionRange > 0);
	assert(MoveRange > 0);
	assert(RangeCalculator > 0);

	//�忬��� �ӽ� ť ����. ť ��� ���� ���� Bfs(�ʺ�켱 �˰���)�� �ƴ� Dfs()�� ��������
	std::queue<Tiles*> BfsFloodFill;

	//����� Ÿ���� �ϳ� �־���
	Tiles* initTile = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(index.y * TILECOLX + index.x)));
	initTile->SetCheckedNum(RangeCalculator);//�˻��� �Ʊ� ����ص� ���� �̵��� �־��
	BfsFloodFill.push(initTile);			//���� Ÿ�� �ϳ� �־��ְ� ����

	while (!BfsFloodFill.empty())
	{
		//��׼���Ʈ
		auto toExamen = BfsFloodFill.front();   //ť�� ù° ��� Ȯ�� �� ��Ƶ�
		BfsFloodFill.pop();						//ť ù°��� �Ͷ߸�
		AvailableTiles.insert(toExamen);		//��� ������ Ÿ�Ͽ� �־��

		//���� �˻�
		bool UpValidity = true;
		UpValidity &= (0 <= toExamen->GetIndex().y - 1);
		UpValidity &= toExamen->GetCheckedNum() > 1;
		if (UpValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string((toExamen->GetIndex().y - 1) * TILECOLX + toExamen->GetIndex().x)));
			UpValidity &= !checkTarget->GetCheckedNum();
			UpValidity &= checkTarget->GetObjT() == "";
			if (UpValidity)
			{
				for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
				{
					if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
					{
						redTiles.insert(checkTarget);
						UpValidity &= false;
					}
					else
					{
						UpValidity &= true;
					}
				}
			}
			if (UpValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
				BfsFloodFill.push(checkTarget);
			}
		}

		//���� �˻�
		bool LeftValidity = true;
		LeftValidity &= (0 <= toExamen->GetIndex().x - 1);
		LeftValidity &= toExamen->GetCheckedNum() > 1;
		if (LeftValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(toExamen->GetIndex().y * TILECOLX + toExamen->GetIndex().x - 1)));
			LeftValidity &= !checkTarget->GetCheckedNum();
			LeftValidity &= checkTarget->GetObjT() == "";
			if (LeftValidity)
			{
				for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
				{
					if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
					{
						redTiles.insert(checkTarget);
						LeftValidity &= false;
					}
					else
					{
						LeftValidity &= true;
					}
				}
			}

			if (LeftValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
				BfsFloodFill.push(checkTarget);
			}
		}

		//��Ʒ� �˻�
		bool DownValidity = true;
		DownValidity &= (toExamen->GetIndex().y + 1 < TILEROWY);
		DownValidity &= toExamen->GetCheckedNum() > 1;
		if (DownValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string((toExamen->GetIndex().y + 1) * TILECOLX + toExamen->GetIndex().x)));
			DownValidity &= !checkTarget->GetCheckedNum();
			DownValidity &= checkTarget->GetObjT() == "";
			if (DownValidity)
			{
				for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
				{
					if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
					{
						redTiles.insert(checkTarget);
						DownValidity &= false;
					}
					else
					{
						DownValidity &= true;
					}
				}
			}

			if (DownValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
				BfsFloodFill.push(checkTarget);
			}
		}

		//��� �˻�
		bool RightValidity = true;
		RightValidity &= (toExamen->GetIndex().x + 1 < TILECOLX);
		RightValidity &= toExamen->GetCheckedNum() > 1;
		if (RightValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(toExamen->GetIndex().y * TILECOLX + toExamen->GetIndex().x + 1)));
			RightValidity &= !checkTarget->GetCheckedNum();
			RightValidity &= checkTarget->GetObjT() == "";
			if (RightValidity)
			{
				for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
				{
					if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
					{
						redTiles.insert(checkTarget);
						RightValidity &= false;
					}
					else
					{
						RightValidity &= true;
					}
				}
			}

			if (RightValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
				BfsFloodFill.push(checkTarget);
			}
		}
	}
	(this->*colourShow)(ActionRange);
}
//���̵� ���� �Ⱥ��̰� ��
void Character::DisableMoveRange()
{
	if (isMoveCalculated)
	{
		for (auto& redTile : redTiles)
		{
			redTile->DecreaseRedNum();
		}

		for (auto& greenTile : greenTiles)
		{
			greenTile->DecreaseGreenNum();
		}

		for (auto& blueTile : blueTiles)
		{

			blueTile->DecreaseBlueNum();
		}
		blueTiles.clear();
		redTiles.clear();
		greenTiles.clear();

		AvailableTiles.clear();
		isMoveCalculated = false;
	}
}

void Character::DisableActionRange()
{
	if (isRangeCalculated)
	{
		for (auto& purpleTile : purpleTiles)
		{
			purpleTile->DecreasePurpleNum();
		}

		//isCalculated = false;
		isRangeCalculated = false;
	}
	purpleTiles.clear();
	blueTiles.clear();
	redTiles.clear();
	greenTiles.clear();

	AvailableTiles.clear();
}


void Character::ShowActionRange()
{
	ShowMoveRange(&Character::PurpleShow);
	isRangeCalculated = true;
}

//�������ӵ����� �κ�
void Character::AdjustFrame()
{
	frameCounter++; //������ �׽� ����

	//��Ŭ���� �����϶�
	if (charStatus == CharStatus::IsClicked)
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
	if (charStatus == CharStatus::IsCursorOn || charStatus == CharStatus::IsClicked)
	{
		portraitAlpha += 0.1f;
		if (portraitAlpha >= 1) portraitAlpha = 1;
	}
	else
	{
		portraitAlpha -= 0.1f;
		if (portraitAlpha <= 0) portraitAlpha = 0;
	}
}

void Character::BlueRedShow(INT _actionRange)
{
	//�忬�� ���� ��� �ൿ������ ���� �׼�
	for (auto& toCal : AvailableTiles)
	{
		if (toCal->GetCheckedNum() <= _actionRange)
		{
			if (occupation == Occupation::Cleric || occupation == Occupation::WarCleric)
			{
				if (toCal->GetGreenAlpha() < 0.5f) toCal->SetGreenAlpha(0.4f);
				toCal->IncreaseGreenNum();	//�׸� ���� ���� ����
				greenTiles.insert(toCal);
				toCal->SetCheckedNum(0);
			}
			else
			{
				redTiles.insert(toCal);
			}
		}
		else
		{
			if (toCal->GetBlueAlpha() < 0.5f) toCal->SetBlueAlpha(0.4f);
			toCal->IncreaseBlueNum();	//��� ���� ���� ����
			toCal->SetCheckedNum(0);
			blueTiles.insert(toCal);
		}
	}
	for (auto& redTile : redTiles)
	{
		if (redTile->GetRedAlpha() < 0.5f) redTile->SetRedAlpha(0.4f);
		redTile->IncreaseRedNum();	//���� ���� ���� ����
		redTile->SetCheckedNum(0);
	}
}

void Character::PurpleShow(INT _actionRange)
{
	//�忬�� ���� ��� �ൿ������ ���� �׼�
	for (auto& purpleTile : AvailableTiles)
	{
		if (purpleTile->GetPurpleAlpha() < 0.5f) purpleTile->SetPurpleAlpha(0.4f);
		purpleTile->IncreasePurpleNum();	//���� ���� ���� ����
		purpleTile->SetCheckedNum(0);
		purpleTiles.insert(purpleTile);
	}
}


//������ ���ÿ�
void Character::SetOccupation(Occupation _job)
{
	this->occupation = _job;
	switch (this->occupation)
	{
	case Occupation::Swordsman:
		occupationData.Range = 1;
		occupationData.Health = 25;
		occupationData.Attack = 5;
		occupationData.Defence = 5;
		occupationData.Luck = 90;
		occupationData.Move = 4;
		break;
	case Occupation::Knight:
		occupationData.Range = 1;
		occupationData.Health = 25;
		occupationData.Attack = 5;
		occupationData.Defence = 5;
		occupationData.Luck = 90;
		occupationData.Move = 6;
		break;
	case Occupation::Mage:
		occupationData.Range = 2;
		occupationData.Health = 20;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 80;
		occupationData.Move = 4;
		break;
	case Occupation::Assassin:
		occupationData.Range = 1;
		occupationData.Health = 20;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 95;
		occupationData.Move = 4;
		break;
	case Occupation::Archer:
		occupationData.Range = 2;
		occupationData.Health = 15;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 70;
		occupationData.Move = 4;
		break;
	case Occupation::Cleric:
		occupationData.Range = 1;
		occupationData.Health = 15;
		occupationData.Attack = 2;
		occupationData.Defence = 1;
		occupationData.Luck = 100;
		occupationData.Move = 4;
		break;
	case Occupation::Sniper:
		occupationData.Range = 3;
		occupationData.Health = 15;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 70;
		occupationData.Move = 4;
		break;
	default:
		assert(false && "Occupation does not exist"); //�������� �ʴ� ���� ������ �Ͷ߸�
		break;
	}
}

void Character::SetFrameAuto(Occupation _job, OwnedBy _whos)
{
	frame.x = static_cast<UINT>(_whos);
	frame.y = static_cast<UINT>(_job);
}

//���־��� �̹��� �̸����� ����
void Character::SetImg(std::string _CharName)
{
	portraitImg = IMAGEMANAGER->FindImage("�ʻ�ȭ" + _CharName);
	frameImg = IMAGEMANAGER->FindImage("ĳ����" + _CharName);
}

//�����ڰ� �޾ƿ°� �˾Ƽ� ����
void Character::SetInitialChar(Occupation _job, std::string _charName, POINT _index, OwnedBy _whos)
{
	whosChar = _whos;
	SetOccupation(_job);
	SetFrameAuto(_job, _whos);
	SetImg(_charName);
	SetIndex(_index);
	SetPositionViaIndex();
}

//��׸���
void Character::Render()
{
	if (CheckInCamera())
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