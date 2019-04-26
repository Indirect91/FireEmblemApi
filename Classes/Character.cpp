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
	occupation = Occupation::Infantary; //�ϴ��� ��� ĳ���� �������� ���
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

	RangeCalculator = 0;
	isCalculated = false;
	tmpCursor = nullptr;
	dragValidity = false;
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

			for (auto& toThicken : AvailableTiles)
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
	//���� ĳ������ �ൿ ���� ������ ǥ����
	INT ActionRange = (((UINT)occupation) % (UINT)Occupation::RangeSeparater) + 1;  //��Ÿ� or ������
	INT MoveRange = AdditionalMove + classMove + baseMove;							//�̵�����
	RangeCalculator = ActionRange + MoveRange;										//�ൿ+�̵�����

	//��0������ ���� ������ �Ͷ߸�
	assert(ActionRange > 0);
	assert(MoveRange > 0);
	assert(RangeCalculator > 0);

	//�忬��� �ӽ� ť ����. ť ��� ���� ���� Bfs(�ʺ�켱 �˰���)�� �ƴ� Dfs()�� ��������
	std::queue<Tiles *> BfsFloodFill;

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
		BOOL UpValidity = true;
		UpValidity &= (0 <= toExamen->GetIndex().y - 1);
		UpValidity &= toExamen->GetCheckedNum() > 0;
		if (UpValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string((toExamen->GetIndex().y - 1) * TILECOLX + toExamen->GetIndex().x)));
			UpValidity &= !checkTarget->GetCheckedNum();
			UpValidity &= checkTarget->GetObjT() == "";

			if (UpValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum()-1);
				BfsFloodFill.push(checkTarget);
			}
		}

		//���� �˻�
		BOOL LeftValidity = true;
		LeftValidity &= (0 <= toExamen->GetIndex().x - 1);
		LeftValidity &= toExamen->GetCheckedNum() > 0;
		if (LeftValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(toExamen->GetIndex().y * TILECOLX + toExamen->GetIndex().x - 1)));
			LeftValidity &= !checkTarget->GetCheckedNum();
			LeftValidity &= checkTarget->GetObjT() == "";

			if (LeftValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
				BfsFloodFill.push(checkTarget);
			}
		}

		//��Ʒ� �˻�
		BOOL DownValidity = true;
		DownValidity &= (toExamen->GetIndex().y + 1 < TILEROWY);
		DownValidity &= toExamen->GetCheckedNum() > 0;
		if (DownValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string((toExamen->GetIndex().y + 1) * TILECOLX + toExamen->GetIndex().x)));
			DownValidity &= !checkTarget->GetCheckedNum();
			DownValidity &= checkTarget->GetObjT() == "";

			if (DownValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
				BfsFloodFill.push(checkTarget);
			}
		}

		//��� �˻�
		BOOL RightValidity = true;
		RightValidity &= (toExamen->GetIndex().x + 1 < TILECOLX);
		RightValidity &= toExamen->GetCheckedNum() > 0;
		if (RightValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(toExamen->GetIndex().y * TILECOLX + toExamen->GetIndex().x + 1)));
			RightValidity &= !checkTarget->GetCheckedNum();
			RightValidity &= checkTarget->GetObjT() == "";

			if (RightValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
				BfsFloodFill.push(checkTarget);
			}
		}
	}

	//�忬�� ���� ��� �ൿ������ ���� �׼�
	for (auto& blueTile : AvailableTiles)
	{
		if (blueTile->GetBlueAlpha() < 0.5f) blueTile->SetBlueAlpha(0.2f);
		blueTile->IncreaseBlueNum();	//��� ���� ���� ����
		blueTile->SetCheckedNum(0);	
	}
}
//���̵� ���� �Ⱥ��̰� ��
void Character::DisableMoveRange()
{
	for (auto &blueTile : AvailableTiles)
	{
		blueTile->DecreaseBlueNum();
	}
	isCalculated = false;
	AvailableTiles.clear();
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
	this->occupation = _job;
	switch (this->occupation)
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
	case Occupation::Archer:
		classHp = 15;
		classAttack = 8;
		classDefence = 2;
		classLuck = 10;
		classMove = 4;
		break;
	default:
		assert("Occupation does not exist");
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