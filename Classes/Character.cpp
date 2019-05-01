#include "../stdafx.h"
#include "Character.h"
#include "Item.h"
#include "Tiles.h"
#include "Cursor.h"
#include "SelectionUI.h"

//�������
Character::Character()
{
	moveSpeed = 8;
	whosChar = OwnedBy::Ally;
	draggingIndex = index;
	moveStartLocation = { 0 };
	draggindDirection = DraggingDirection::IDLE;
	movingDirection = MovingDirection::IDLE;
	charStatus = CharStatus::Idle;
	isInCamera = false;
	occupation = Occupation::Archer;
	portraitImg = nullptr;
	portraitAlpha = 0.f;
	frameImg = nullptr;
	item = nullptr;
	draggingStarted = nullptr;
	isActionTaken = false;
	frame = { 0 };
	frameLoop = 0;
	frameCounter = 0;
	frameInterval = 5;
	name = "";
	occupationData = { 0 };
	ZeroMemory(&additionalData, sizeof(additionalData));
	memset(&baseData, 0, sizeof(baseData));
	draggingIndexPrev = {0};
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
	name = _charName;
	moveSpeed = 8;
	index = _index;
	draggingIndex = _index;
	draggindDirection = DraggingDirection::IDLE;
	movingDirection = MovingDirection::IDLE;
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
	SetImgAuto(_charName);
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

	if (cursor->GetCursorTurn() == IngameStatus::PlayerTurn && whosChar == OwnedBy::Player)
	{
		//����� ���¸� �ش�
		if (isActionTaken) //�ൿ�� �������� �ִ� ĳ���Ͷ��
		{
			charStatus = CharStatus::IsActed;
		} //�ൿ�� ���·� ����
		else if(charStatus == CharStatus::IsMoving)
		{
			charStatus = CharStatus::IsMoving;
		}
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
	else if (cursor->GetCursorTurn() == IngameStatus::PlayerTurn && whosChar == OwnedBy::Enemy)
	{
		charStatus = CharStatus::IsCheckingOut;
	}
	else if (cursor->GetCursorTurn() == IngameStatus::EnemyTurn)
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

			DATACENTRE.ClearObjects(ObjType::BlueTiles);
			DATACENTRE.ClearObjects(ObjType::FoeTiles);
			DATACENTRE.ClearObjects(ObjType::AllyTiles);

			cursor->SetCursorOccupied("");
		}
		draggingIndex = cursor->GetIndex();
		if (!PointCompare(index, cursor->GetIndex()))
		{
			charStatus = CharStatus::IsDragging;

			draggingIndex = cursor->GetIndex();
			draggingIndexPrev = draggingIndex;
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

			for (auto& toThickenRed : redTiles)
			{
				toThickenRed->SetRedAlpha(0.8f);
			}
			for (auto& toThickenGreen : greenTiles)
			{
				toThickenGreen->SetGreenAlpha(0.8f);
			}
			for (auto& toThickenBlue : blueTiles)
			{
				toThickenBlue->SetBlueAlpha(0.8f);
				DATACENTRE.AddObj(ObjType::BlueTiles, TwoDimentionArrayToOneString(toThickenBlue->GetIndex(), TILECOLX), toThickenBlue);
			}
			for (auto& Ally : allyTiles)
			{
				DATACENTRE.AddObj(ObjType::AllyTiles, TwoDimentionArrayToOneString(Ally->GetIndex(), TILECOLX), Ally);
			}
			for (auto& Enemy : foeTiles)
			{
				DATACENTRE.AddObj(ObjType::FoeTiles, TwoDimentionArrayToOneString(Enemy->GetIndex(), TILECOLX), Enemy);
			}
		}
		break;

	//�� ���̵� �����϶�
	case CharStatus::Idle:
		DisableMoveRange();
		break;

	//�� �巡�� �����϶�
	case CharStatus::IsDragging:
	{
		//���Ŭ���̳� S�� ������ �Ȼ��� ����
		if (KEYMANAGER->IsOnceKeyDown('S'))
		{
			charStatus = CharStatus::IsCursorOn;
			DisableMoveRange();
			cursor->SetCursorOccupied("");
			cursor->SetIndex({ index.x,index.y });
			cursor->SetPositionViaIndex();
			cursor->SetIsOtherUnitOn("", OwnedBy::Nobody);

			DATACENTRE.ClearObjects(ObjType::BlueTiles);
			DATACENTRE.ClearObjects(ObjType::FoeTiles);
			DATACENTRE.ClearObjects(ObjType::AllyTiles);
			for (auto& arrowTiles : toMove)
			{
				arrowTiles->SetArrowT("");
			}
			toMove.clear();
			break;
		}

		//����� ����
		cursor->SetIsOtherUnitOn("", OwnedBy::Nobody);
		draggingIndex = cursor->GetIndex(); //�÷��̾� ��ġ
		draggingStarted = nullptr;			//�������� �����

		//if (DATACENTRE.CheckObjectExistance(ObjType::AllyTiles, TwoDimentionArrayToOneString(draggingIndex, TILECOLX)))
		//{
		//
		//}
		if (DATACENTRE.CheckObjectExistance(ObjType::BlueTiles, TwoDimentionArrayToOneString(draggingIndex, TILECOLX)))
		{
			draggingStarted = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::BlueTiles, TwoDimentionArrayToOneString(draggingIndex, TILECOLX)));
			bool checkAlly = false;
			for (auto& toExamine : DATACENTRE.RefObjects(ObjType::AllyTiles))
			{
				if (PointCompare(toExamine.second->GetIndex(), draggingStarted->GetIndex()))
				{
					checkAlly = true;
					break;
				}
			}
			if(!checkAlly) 
			{draggingIndexPrev = draggingIndex;}
		}
		if (draggingStarted == nullptr)
		{
			draggingStarted = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::FoeTiles, TwoDimentionArrayToOneString(draggingIndex, TILECOLX)));
			for (auto& toFind : DATACENTRE.RefObjects(ObjType::EnemyArmy))
			{
				if (PointCompare(toFind.second->GetIndex(), draggingStarted->GetIndex()))
				{
					cursor->SetIsOtherUnitOn(toFind.first, OwnedBy::Enemy);
					break;
				}
			}
			assert(cursor->GetIsOtherUnit().name != "" && "Ÿ�� ���� �� �־����..");
		}
		assert(draggingStarted!=nullptr);

		auto MoveContainer = DATACENTRE.RefObjects(ObjType::BlueTiles);
		for (auto& arrowTiles : toMove)
		{
			arrowTiles->SetArrowT("");
		}
		toMove.clear();
		
		//�屸�� 
		while (!PointCompare(draggingStarted->GetIndex(), this->index))
		{
			toMove.push_back(draggingStarted);
			int toFind = draggingStarted->GetRouteNum();
			for (auto& availableTile : MoveContainer)
			{
				assert((dynamic_cast<Tiles*>(availableTile.second)) != nullptr);
				if ((dynamic_cast<Tiles*>(availableTile.second)->GetRouteNum() == toFind + 1)
					&& (((availableTile.second->GetIndex().x == draggingStarted->GetIndex().x+1) && (availableTile.second->GetIndex().y == draggingStarted->GetIndex().y))
					|| ((availableTile.second->GetIndex().x == draggingStarted->GetIndex().x-1) && (availableTile.second->GetIndex().y == draggingStarted->GetIndex().y))
					|| ((availableTile.second->GetIndex().x == draggingStarted->GetIndex().x) && (availableTile.second->GetIndex().y-1 == draggingStarted->GetIndex().y))
					|| ((availableTile.second->GetIndex().x == draggingStarted->GetIndex().x) && (availableTile.second->GetIndex().y+1 == draggingStarted->GetIndex().y))))
				{
					draggingStarted = dynamic_cast<Tiles*>(availableTile.second);
					break;
				}
			}
		}
		toMove.push_back(dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::BlueTiles, TwoDimentionArrayToOneString(index, TILECOLX))));

		//��巡�� �� �������� ȭ��ǥ �׸��� ���
		if (toMove.size() > 1)
		{
			for (UINT i = 0; i < toMove.size(); i++)
			{
				toMove[i]->SetArrowT("MoveArrow");
				if (i == 0 && (i + 1 < toMove.size()))
				{
					if (toMove[i + 1]->GetIndex().x > toMove[i]->GetIndex().x)
					{
						toMove[i]->SetArrowtFrame({ 0,2 });
						draggindDirection = DraggingDirection::LEFT;
					}
					else if (toMove[i + 1]->GetIndex().x < toMove[i]->GetIndex().x)
					{
						toMove[i]->SetArrowtFrame({ 1,2 });
						draggindDirection = DraggingDirection::RIGHT;
					}
					else if (toMove[i + 1]->GetIndex().y > toMove[i]->GetIndex().y)
					{
						toMove[i]->SetArrowtFrame({ 3,2 });
						draggindDirection = DraggingDirection::UP;
					}
					else
					{
						toMove[i]->SetArrowtFrame({ 2,2 });
						draggindDirection = DraggingDirection::DOWN;
					}
				}
				else if ((i - 1 >= 0) && i == toMove.size() - 1)
				{
					if (toMove[i - 1]->GetIndex().x > toMove[i]->GetIndex().x)
						toMove[i]->SetArrowtFrame({ 0,3 });
					else if (toMove[i - 1]->GetIndex().x < toMove[i]->GetIndex().x)
						toMove[i]->SetArrowtFrame({ 1,3 });
					else if (toMove[i - 1]->GetIndex().y > toMove[i]->GetIndex().y)
						toMove[i]->SetArrowtFrame({ 3,3 });
					else
						toMove[i]->SetArrowtFrame({ 2,3 });
				}
				else
				{
					if (toMove[i - 1]->GetIndex().x == toMove[i]->GetIndex().x && toMove[i + 1]->GetIndex().x== toMove[i]->GetIndex().x)
						toMove[i]->SetArrowtFrame({ 3,1 });
					else if (toMove[i - 1]->GetIndex().y == toMove[i]->GetIndex().y && toMove[i + 1]->GetIndex().y == toMove[i]->GetIndex().y)
						toMove[i]->SetArrowtFrame({ 0,1 });

					if (index.y > cursor->GetIndex().y)
					{
						if (toMove[i - 1]->GetIndex().x > toMove[i]->GetIndex().x
						&& toMove[i - 1]->GetIndex().y == toMove[i]->GetIndex().y
						&& toMove[i + 1]->GetIndex().x == toMove[i]->GetIndex().x
						&& toMove[i + 1]->GetIndex().y > toMove[i]->GetIndex().y)

						toMove[i]->SetArrowtFrame({ 0,0 });

						else if (toMove[i - 1]->GetIndex().x < toMove[i]->GetIndex().x
						&& toMove[i - 1]->GetIndex().y == toMove[i]->GetIndex().y
						&& toMove[i + 1]->GetIndex().x == toMove[i]->GetIndex().x
						&& toMove[i + 1]->GetIndex().y > toMove[i]->GetIndex().y)
					
						toMove[i]->SetArrowtFrame({ 1,0 });

						else if (toMove[i - 1]->GetIndex().x == toMove[i]->GetIndex().x
							&& toMove[i - 1]->GetIndex().y < toMove[i]->GetIndex().y
							&& toMove[i + 1]->GetIndex().x > toMove[i]->GetIndex().x
							&& toMove[i + 1]->GetIndex().y == toMove[i]->GetIndex().y)

							toMove[i]->SetArrowtFrame({ 3,0 });

						else if (toMove[i - 1]->GetIndex().x == toMove[i]->GetIndex().x
							&& toMove[i - 1]->GetIndex().y < toMove[i]->GetIndex().y
							&& toMove[i + 1]->GetIndex().x < toMove[i]->GetIndex().x
							&& toMove[i + 1]->GetIndex().y == toMove[i]->GetIndex().y)

							toMove[i]->SetArrowtFrame({ 2,0 });
						//TODO:���� �߰�
					}
					else
					{
						if (toMove[i - 1]->GetIndex().x < toMove[i]->GetIndex().x
							&& toMove[i - 1]->GetIndex().y == toMove[i]->GetIndex().y
							&& toMove[i + 1]->GetIndex().x == toMove[i]->GetIndex().x
							&& toMove[i + 1]->GetIndex().y < toMove[i]->GetIndex().y)

							toMove[i]->SetArrowtFrame({ 2,0 });

						else if (toMove[i - 1]->GetIndex().x > toMove[i]->GetIndex().x
							&& toMove[i - 1]->GetIndex().y == toMove[i]->GetIndex().y
							&& toMove[i + 1]->GetIndex().x == toMove[i]->GetIndex().x
							&& toMove[i + 1]->GetIndex().y < toMove[i]->GetIndex().y)

							toMove[i]->SetArrowtFrame({ 3,0 });

						else if (toMove[i - 1]->GetIndex().x == toMove[i]->GetIndex().x
							&& toMove[i - 1]->GetIndex().y > toMove[i]->GetIndex().y
							&& toMove[i + 1]->GetIndex().x > toMove[i]->GetIndex().x
							&& toMove[i + 1]->GetIndex().y == toMove[i]->GetIndex().y)

							toMove[i]->SetArrowtFrame({ 0,0 });

						else if (toMove[i - 1]->GetIndex().x == toMove[i]->GetIndex().x
							&& toMove[i - 1]->GetIndex().y > toMove[i]->GetIndex().y
							&& toMove[i + 1]->GetIndex().x < toMove[i]->GetIndex().x
							&& toMove[i + 1]->GetIndex().y == toMove[i]->GetIndex().y)

							toMove[i]->SetArrowtFrame({ 1,0 });
						else if (toMove[i - 1]->GetIndex().x == toMove[i]->GetIndex().x
							&& toMove[i - 1]->GetIndex().y < toMove[i]->GetIndex().y
							&& toMove[i + 1]->GetIndex().x > toMove[i]->GetIndex().x
							&& toMove[i + 1]->GetIndex().y == toMove[i]->GetIndex().y)

							toMove[i]->SetArrowtFrame({ 3,0 });
						//TODO:���� �߰�
					}
				}
			}
		}

		//��巡�� ���� AŰ�� ������ �̵����� Ȯ������
		if (KEYMANAGER->IsOnceKeyDown('A'))
		{
			//������ ���� ���°���, �� ��Ÿ�Ͽ� Ŀ���� �ΰ� A�� �����ٸ�
			if ((cursor->GetIsOtherUnit().name==""))
			{
				this->charStatus = CharStatus::IsMoving;
				dynamic_cast<SelectionUI*>(DATACENTRE.GetCertainObject(ObjType::UI, "SelectionUI"))->SetToShow(SelectionUI::ToShow::SelectionBox);
				moveStartLocation = index; //����ϱ� ���� ���� ���
			}

			else
			{

				cursor->SetCursorTurn(IngameStatus::SelectionUI);
				SelectionUI* toSelect = dynamic_cast<SelectionUI*>(DATACENTRE.GetCertainObject(ObjType::UI, "SelectionUI"));
				toSelect->SetToShow(SelectionUI::ToShow::BattlePredict);
				toSelect->SetBattlePredict(this, this);
				toSelect->SetPhotoFrameAlphaZero();
				cursor->SetCursorTurnPrev(IngameStatus::PlayerTurn);
				SelectionUI;
			}
			
		}

		break;
	}
	case CharStatus::Disable:

		break;
	case CharStatus::IsMoving:
		//���̵� ���߿��� ȭ��ǥ�� ��������
		for (auto& arrowTiles : toMove)
		{
			arrowTiles->SetArrowT("");
		}
		//���̵��ϰ��� �ϴ� �����̳ʰ� �ϳ��� �����Ҷ��� ����
		if (toMove.size() > 0)
		{
			//���̵��ϰ��� �ϴ� Ÿ�Ͽ� ���� ������, �̵����� �ʰ� �ٶ󺸴� ���⸸ �ٲ۴�
			if (DATACENTRE.CheckObjectExistance(ObjType::FoeTiles, TwoDimentionArrayToOneString(toMove.back()->GetIndex(), TILECOLX))|| DATACENTRE.CheckObjectExistance(ObjType::AllyTiles, TwoDimentionArrayToOneString(toMove.back()->GetIndex(), TILECOLX)))
			{
				float directionAngle = floor(GetAngleDegree(RectLeftTopOnly(position), RectLeftTopOnly(toMove.back()->GetPosition())) + 0.5f);
				if (directionAngle == 0)
					{movingDirection = MovingDirection::RIGHT;}
				else if (directionAngle == 90)
					{movingDirection = MovingDirection::LEFT;}
				else if (directionAngle == 180)
					{movingDirection = MovingDirection::UP;}
				else if (directionAngle == 270)
					{movingDirection = MovingDirection::DOWN;}
				this->toMove.pop_back();
			}
			//�常�� ���� ���ڶ��� ��� ��ǥ�� �̵� �Ϸᰡ �Ǿ�����
			else if (PointCompare(RectLeftTopOnly(position), RectLeftTopOnly(toMove.back()->GetPosition())))
			{
				index = toMove.back()->GetIndex(); //�� ĳ������ �ε��� ���� �������ְ�
				this->toMove.pop_back(); //������ ��带 ����
			}
			else
			{
				assert(TILESIZE % moveSpeed == 0); //�̵��ӵ��� 48�� ����� �ƴϸ� �Ͷ߸�
				float directionAngle = floor(GetAngleDegree(RectLeftTopOnly(position), RectLeftTopOnly(toMove.back()->GetPosition())) + 0.5f);
				if(directionAngle==0)
				{
					position.left += moveSpeed;
					position.right += moveSpeed;
					movingDirection = MovingDirection::RIGHT;
				}
				else if(directionAngle ==90)
				{ 
					position.top -= moveSpeed;
					position.bottom -= moveSpeed;
					movingDirection = MovingDirection::UP;
				}
				else if (directionAngle == 180)
				{
					position.left -= moveSpeed;
					position.right -= moveSpeed;
					movingDirection = MovingDirection::LEFT;
				}
				else if(directionAngle==270)
				{
					position.top += moveSpeed;
					position.bottom += moveSpeed;
					movingDirection = MovingDirection::DOWN;
				}
				else
				{
					assert(false && "wrong angle");
				}
			}
		}
		//���� ĳ���Ͱ� ����������
		else
		{
			cursor->SetCursorTurn(IngameStatus::SelectionUI);
			dynamic_cast<SelectionUI*>(DATACENTRE.GetCertainObject(ObjType::UI, "SelectionUI"))->SetPhotoFrameAlphaZero();
			cursor->SetCursorTurnPrev(IngameStatus::PlayerTurn);
		}

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

	//TODO
	//GameObject::SetPositionViaIndex();
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
	initTile->SetRouteNum(RangeCalculator);//�˻��� �Ʊ� ����ص� ���� �̵��� �־��
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
		//��ù° ������ ���̶��
		if (UpValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string((toExamen->GetIndex().y - 1) * TILECOLX + toExamen->GetIndex().x)));
			UpValidity &= !checkTarget->GetCheckedNum();	//üũ�ѹ� ������
			UpValidity &= checkTarget->GetObjT() == "";		//��ֹ� �ִ��� ������
			//���̹����� ���̶��
			if (UpValidity)
			{
				//��Ʊ��� ĳ���Ϳ��ٸ�
				if (whosChar == OwnedBy::Player)
				{
					//������� ����������, ���� ����Ʈ�� ����
					for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
						{
							redTiles.insert(checkTarget);
							foeTiles.insert(checkTarget);
							checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
							UpValidity &= false;
						}
					}
					for (auto& isAlly : DATACENTRE.RefObjects(ObjType::PlayerArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isAlly.second->GetIndex()))
						{
							allyTiles.insert(checkTarget);
						}
					}
				}
				//��ĳ���Ͱ� ���� �����ϋ�
				else if (whosChar == OwnedBy::Enemy)
				{
					for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::PlayerArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
						{
							redTiles.insert(checkTarget);
							foeTiles.insert(checkTarget);
							checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
							UpValidity &= false;
						}
					}
					for (auto& isAlly : DATACENTRE.RefObjects(ObjType::EnemyArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isAlly.second->GetIndex()))
						{
							allyTiles.insert(checkTarget);
						}
					}
				}
			}
			//���� ������ ������, �˻� ��� �߰�
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
				if (whosChar == OwnedBy::Player)
				{
					for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
						{
							redTiles.insert(checkTarget);
							foeTiles.insert(checkTarget);
							checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
							LeftValidity &= false;
						}
					}
					for (auto& isAlly : DATACENTRE.RefObjects(ObjType::PlayerArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isAlly.second->GetIndex()))
						{
							allyTiles.insert(checkTarget);
						}
					}

				}
				//��ĳ���Ͱ� ���� �����ϋ�
				else if (whosChar == OwnedBy::Enemy)
				{
					for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::PlayerArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
						{
							redTiles.insert(checkTarget);
							foeTiles.insert(checkTarget);
							checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
							LeftValidity &= false;
						}
					}
					for (auto& isAlly : DATACENTRE.RefObjects(ObjType::EnemyArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isAlly.second->GetIndex()))
						{
							allyTiles.insert(checkTarget);
						}
					}
				}
			}
			//���� ������ ������, �˻� ��� �߰�
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
				if (whosChar == OwnedBy::Player)
				{
					for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
						{
							redTiles.insert(checkTarget);
							foeTiles.insert(checkTarget);
							checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
							DownValidity &= false;
						}
					}
					for (auto& isAlly : DATACENTRE.RefObjects(ObjType::PlayerArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isAlly.second->GetIndex()))
						{
							allyTiles.insert(checkTarget);
						}
					}

				}
				//��ĳ���Ͱ� ���� �����ϋ�
				else if (whosChar == OwnedBy::Enemy)
				{
					for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::PlayerArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
						{
							redTiles.insert(checkTarget);
							foeTiles.insert(checkTarget);
							checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
							DownValidity &= false;
						}
					}
					for (auto& isAlly : DATACENTRE.RefObjects(ObjType::EnemyArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isAlly.second->GetIndex()))
						{
							allyTiles.insert(checkTarget);
						}
					}
				}
			}
			//���� ������ ������, �˻� ��� �߰�
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
				if (whosChar == OwnedBy::Player)
				{
					for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
						{
							redTiles.insert(checkTarget);
							foeTiles.insert(checkTarget);
							checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
							RightValidity &= false;
						}
					}
					for (auto& isAlly : DATACENTRE.RefObjects(ObjType::PlayerArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isAlly.second->GetIndex()))
						{
							allyTiles.insert(checkTarget);
						}
					}

				}
				//��ĳ���Ͱ� ���� �����ϋ�
				else if (whosChar == OwnedBy::Enemy)
				{
					for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::PlayerArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
						{
							redTiles.insert(checkTarget);
							foeTiles.insert(checkTarget);
							checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
							RightValidity &= false;
						}
					}
					for (auto& isAlly : DATACENTRE.RefObjects(ObjType::EnemyArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isAlly.second->GetIndex()))
						{
							allyTiles.insert(checkTarget);
						}
					}
				}
			}
			//���� ������ ������, �˻� ��� �߰�
			if (RightValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
				BfsFloodFill.push(checkTarget);
			}
		}
	}
	(this->*colourShow)(ActionRange);
}
//���̵� �� ���ݹ��� �Ķ��� �Ⱥ��̰� ó��
void Character::DisableMoveRange()
{
	//���ش� ����� �ѹ��̶� ����ǥ�� ������ �־������� �۵�
	if (isMoveCalculated)
	{
		for (auto& redTile : redTiles)
		{
			redTile->DecreaseRedNum(); //���� �������� ���̱�
		}

		for (auto& greenTile : greenTiles)
		{
			greenTile->DecreaseGreenNum();//�ʷ� �������� ���̱�
		}

		for (auto& blueTile : blueTiles)
		{

			blueTile->DecreaseBlueNum();//�Ķ� �������� ���̱�
		}
		blueTiles.clear();
		DATACENTRE.ClearObjects(ObjType::BlueTiles);
		DATACENTRE.ClearObjects(ObjType::FoeTiles);
		DATACENTRE.ClearObjects(ObjType::AllyTiles);
		redTiles.clear();
		greenTiles.clear();

		AvailableTiles.clear();
		isMoveCalculated = false;
	}
}

//��(��+�̵�) ����� �ൿ���� ǥ���Ѱ� �� �� ���
void Character::DisableActionRange()
{
	//���ش� ����� �ѹ��̶� ����ǥ�� ������ �־������� �۵�
	if (isRangeCalculated)
	{
		for (auto& purpleTile : purpleTiles)
		{
			purpleTile->DecreasePurpleNum();
		}

		isRangeCalculated = false;
	}
	purpleTiles.clear();
	blueTiles.clear();
	redTiles.clear();
	greenTiles.clear();

	AvailableTiles.clear();
}

//���+�̵� ���� ����ǥ��
void Character::ShowActionRange()
{
	//���Լ������͸� �Ѱܼ� ���� �ٸ� ��� ����
	ShowMoveRange(&Character::PurpleShow);
	isRangeCalculated = true; //����ߴٰ� �˸�
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

//�� ���ݹ����� ������ �̵����� �Ķ����� �� �� �����ִ� ���
void Character::BlueRedShow(INT _actionRange)
{
	//�忬�� ���� ��� �ൿ������ ���� �׼�
	for (auto& toCal : AvailableTiles)
	{
		if (toCal->GetCheckedNum() <= _actionRange)
		{
			if (occupation == Occupation::Cleric || occupation == Occupation::WarCleric)
			{
				//��׸� �������� ����
				if (toCal->GetGreenAlpha() < 0.5f) toCal->SetGreenAlpha(0.4f);
				toCal->IncreaseGreenNum();	
				greenTiles.insert(toCal);
				toCal->SetRouteNum(toCal->GetCheckedNum());
				toCal->SetCheckedNum(0);
			}
			else
			{
				//�廡�� �������� ����
				redTiles.insert(toCal);
				toCal->SetRouteNum(toCal->GetCheckedNum());
				toCal->SetCheckedNum(0);
			}
		}
		else
		{
			//���� ���� ���� ����
			if (toCal->GetBlueAlpha() < 0.5f) toCal->SetBlueAlpha(0.4f);
			toCal->IncreaseBlueNum();	
			toCal->SetRouteNum(toCal->GetCheckedNum());
			toCal->SetCheckedNum(0);
			blueTiles.insert(toCal);
		}
	}
	//���߰������� ���� ����Ÿ�Ͽ� ���� Ư��ó��
	for (auto& redTile : redTiles)
	{
		if (redTile->GetRedAlpha() < 0.5f) redTile->SetRedAlpha(0.4f);
		redTile->IncreaseRedNum();	
		redTile->SetRouteNum(redTile->GetCheckedNum());
		redTile->SetCheckedNum(0);
	}
}

//���+�̵����� ������ ������� �����ִ� ���
void Character::PurpleShow(INT _actionRange)
{
	//�忬�� ���� ��� �ൿ������ ���� �׼�
	for (auto& purpleTile : AvailableTiles)
	{
		//����� ����Ÿ��
		if (purpleTile->GetPurpleAlpha() < 0.5f) purpleTile->SetPurpleAlpha(0.4f);
		purpleTile->IncreasePurpleNum();
		purpleTile->SetRouteNum(purpleTile->GetCheckedNum());
		purpleTile->SetCheckedNum(0);
		purpleTiles.insert(purpleTile);
	}
	for (auto& purpleEnemyTile : foeTiles)
	{
		//���� Ÿ���ϰ�� 
		if (purpleEnemyTile->GetPurpleAlpha() < 0.5f) purpleEnemyTile->SetPurpleAlpha(0.4f);
		purpleEnemyTile->IncreasePurpleNum();
		purpleEnemyTile->SetRouteNum(purpleEnemyTile->GetCheckedNum());
		purpleEnemyTile->SetCheckedNum(0);
		purpleTiles.insert(purpleEnemyTile);
	}
}

//������ ���ÿ�. ���� �� �ش��ϴ� ������ �־���. �������� �ʴ� ������ ���� �� �Ͷ߸�
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
		occupationData.Move = 5;
		break;
	case Occupation::Knight:
		occupationData.Range = 1;
		occupationData.Health = 25;
		occupationData.Attack = 5;
		occupationData.Defence = 5;
		occupationData.Luck = 90;
		occupationData.Move = 7;
		break;
	case Occupation::Mage:
		occupationData.Range = 2;
		occupationData.Health = 20;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 80;
		occupationData.Move = 5;
		break;
	case Occupation::Assassin:
		occupationData.Range = 1;
		occupationData.Health = 20;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 95;
		occupationData.Move = 5;
		break;
	case Occupation::Archer:
		occupationData.Range = 2;
		occupationData.Health = 15;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 70;
		occupationData.Move = 5;
		break;
	case Occupation::Cleric:
		occupationData.Range = 1;
		occupationData.Health = 15;
		occupationData.Attack = 2;
		occupationData.Defence = 1;
		occupationData.Luck = 100;
		occupationData.Move = 5;
		break;
	case Occupation::Sniper:
		occupationData.Range = 3;
		occupationData.Health = 15;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 70;
		occupationData.Move = 5;
		break;
	default:
		assert(false && "Occupation does not exist"); //�������� �ʴ� ���� ������ �Ͷ߸�
		break;
	}
}

//������ֿ� ������ ���� �˾Ƽ� �־���
void Character::SetFrameAuto(Occupation _job, OwnedBy _whos)
{
	frame.x = static_cast<UINT>(_whos);
	frame.y = static_cast<UINT>(_job);
}

//���־��� �̹��� �̸����� ����
void Character::SetImgAuto(std::string _CharName)
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
	SetImgAuto(_charName);
	SetIndex(_index);
	SetPositionViaIndex();
}

//��׸���
void Character::Render()
{
	//��ī�޶� �ӿ� �������� �����Ѵ�
	if (CheckInCamera())
	{
		AdjustFrame(); //������ ������ �κ��� ���� �ƴҋ��� ���ư����ϴ� ������ ���� 

		frameImg->SetSize({ TILESIZE, TILESIZE }); //�����ӷ����� ������ ����
		if (charStatus == CharStatus::IsMoving) 
			{ frameImg->RelativeFrameRender(position.left, position.top, frame.x + frameLoop, frame.y + (INT)movingDirection); } //ī�޶� ��� ����
		else 
			{ frameImg->RelativeFrameRender(position.left, position.top, frame.x + frameLoop, frame.y); } //ī�޶� ��� ����

		if (isActionTaken) //�ൿ�� ���߾��ٸ�
		{
			IMAGEMANAGER->FindImage("ActionTaken")->SetAlpha(0.5f);
			IMAGEMANAGER->FindImage("ActionTaken")->SetSize({ TILESIZE, TILESIZE });
			IMAGEMANAGER->FindImage("ActionTaken")->RelativeRender(position.left, position.top); //��ο� �׸��ڷ� �������
		}

		//������ UI�� �E �κ�
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
	//��ĳ���� ������ ǥ�� �� �巡�����϶� ���� �̵���ο� ĳ���͸� �׸�
	if (charStatus == CharStatus::IsDragging)
	{
		frameImg->SetSize({ TILESIZE, TILESIZE });
		frameImg->SetAlpha(0.5f);
		frameImg->RelativeFrameRender(draggingIndexPrev.x * TILESIZE, draggingIndexPrev.y * TILESIZE, frame.x + frameLoop, frame.y + (UINT)draggindDirection);
	
	}
}