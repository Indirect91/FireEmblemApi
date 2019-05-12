#include "../stdafx.h"
#include "GameObject.h"
#include "Character.h"
#include "Item.h"
#include "Tiles.h"
#include "Cursor.h"
#include "SelectionUI.h"
#include "ExecuteBattle.h"


//�������
Character::Character()
{
	deadTalkEnd = false;
	deadTalkInit = false;
	isInBattle = false;
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
	characterAlpha = 1.f;
	frameImg = nullptr;
	item = nullptr;
	draggingStarted = nullptr;
	frame = { 0 };
	frameLoop = 0;
	frameCounter = 0;
	frameInterval = 5;
	name = "";
	ZeroMemory(&occupationData, sizeof(occupationData)); ;
	ZeroMemory(&additionalData, sizeof(additionalData));
	memset(&baseData, 0, sizeof(baseData));
	draggingIndexPrev = { 0 };
	RangeCalculator = 0;
	isMoveCalculated = false;
	isRangeCalculated = false;
	cursor = nullptr;
	dragValidity = false;
	healthBarBackground = { 0,0,TILESIZE,TILESIZE / 5 };
	healthBarFront = { 0,0,TILESIZE * 5 / 6,TILESIZE / 6 };
	currentHealth = GetHealth();
}

//������ ĳ�� ������ ���� ������ �����ε�
Character::Character(Occupation _job, std::string _charName, POINT _index, OwnedBy _whos) : whosChar(_whos)
{
	deadTalkEnd = false;
	deadTalkInit = false;
	isInBattle = false;
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
	characterAlpha = 1.f;
	frameImg = nullptr;
	item = nullptr;
	frame.x = static_cast<UINT>(_whos);
	frame.y = static_cast<UINT>(_job);
	frameLoop = 0;
	frameCounter = 0;
	frameInterval = 5;

	ZeroMemory(&occupationData, sizeof(occupationData)); ;
	ZeroMemory(&additionalData, sizeof(additionalData));
	memset(&baseData, 0, sizeof(baseData));

	RangeCalculator = 0;
	isMoveCalculated = false;
	isRangeCalculated = false;
	cursor = nullptr;
	dragValidity = false;

	SetOccupation(_job);
	SetImgAuto(_charName);
	SetPositionViaIndex();
	healthBarBackground = { 0,0,TILESIZE, TILESIZE / 5 };
	healthBarFront = { 0,0,TILESIZE * 5 / 6, TILESIZE / 6 };
	currentHealth = GetHealth();
}

//����� Ŀ���� Ȱ��ȭ ��Ű�� �̴��� ���� ����
void Character::Init()
{
	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
	assert(cursor != nullptr); //Ŀ�� ����� �ȵ������� �Ͷ߸�
	//isActionTaken = false;
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
		if (charStatus == CharStatus::IsActed) //�ൿ�� �������� �ִ� ĳ���Ͷ��
		{
			charStatus = CharStatus::IsActed;
		} //�ൿ�� ���·� ����
		else if (charStatus == CharStatus::IsDeadTalking) //�ൿ�� �������� �ִ� ĳ���Ͷ��
		{
			charStatus = CharStatus::IsDeadTalking;
		}

		else if (charStatus == CharStatus::IsDying)
		{
			charStatus = CharStatus::IsDying;
		}
		else if (charStatus == CharStatus::IsMoving)
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
	else if (cursor->GetCursorTurn() == IngameStatus::PlayerTurn && whosChar == OwnedBy::Enemy && (this->charStatus != CharStatus::IsDying) && this->charStatus != CharStatus::IsDead)
	{
		charStatus = CharStatus::IsCheckingOut;
	}
	else if (cursor->GetCursorTurn() == IngameStatus::PlayerTurn && (charStatus == CharStatus::IsDying || this->charStatus == CharStatus::IsDead)) {}
	else if (cursor->GetCursorTurn() == IngameStatus::EnemyTurn)
	{
		//enemyPhase
		//if(((charStatus != CharStatus::IsMoving) || (charStatus != CharStatus::IsAttacking) || (charStatus != CharStatus::IsActed)))
		charStatus = CharStatus::EnemyPhase;
	}
	else
	{
		assert(false && "Turn Error");
	}

	//����¿� ���� �ൿ
	switch (charStatus)
	{
	case CharStatus::IsDying:
		break;

	case CharStatus::IsDead:
		break;
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

			INT random = (rand() % 3) + 1;	//���� ������Ŵ
			SOUNDMANAGER->play(name + std::to_string(random));//���� ���� �������� ���

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
		//��巡�� ���� ��Ŭ���̳� S�� ������ �Ȼ��� ����
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
			if (!checkAlly)
			{
				draggingIndexPrev = draggingIndex;
			}
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
		assert(draggingStarted != nullptr);

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
					&& (((availableTile.second->GetIndex().x == draggingStarted->GetIndex().x + 1) && (availableTile.second->GetIndex().y == draggingStarted->GetIndex().y))
						|| ((availableTile.second->GetIndex().x == draggingStarted->GetIndex().x - 1) && (availableTile.second->GetIndex().y == draggingStarted->GetIndex().y))
						|| ((availableTile.second->GetIndex().x == draggingStarted->GetIndex().x) && (availableTile.second->GetIndex().y - 1 == draggingStarted->GetIndex().y))
						|| ((availableTile.second->GetIndex().x == draggingStarted->GetIndex().x) && (availableTile.second->GetIndex().y + 1 == draggingStarted->GetIndex().y))))
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
						draggindDirection = DraggingDirection::TOP;
					}
					else
					{
						toMove[i]->SetArrowtFrame({ 2,2 });
						draggindDirection = DraggingDirection::BOTTOM;
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
					if (toMove[i - 1]->GetIndex().x == toMove[i]->GetIndex().x && toMove[i + 1]->GetIndex().x == toMove[i]->GetIndex().x)
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
						//TODO:���� ���¹��� �߰�
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

		if (DATACENTRE.CheckObjectExistance(ObjType::FoeTiles, TwoDimentionArrayToOneString(toMove[0]->GetIndex(), TILECOLX)) || DATACENTRE.CheckObjectExistance(ObjType::AllyTiles, TwoDimentionArrayToOneString(toMove[0]->GetIndex(), TILECOLX)))
		{
			draggingIndexPrev = toMove[1]->GetIndex();
			//toMove[1]->SetArrowtFrame(toMove[0]->GetArrowFrame());
			//toMove[0]->SetArrowT("");
		}
		else
		{
			draggingIndexPrev = draggingIndex;
		}


		//��巡�� ���� AŰ�� ������ �̵����� Ȯ������
		if (KEYMANAGER->IsOnceKeyDown('A'))
		{
			//������ ���� ���°���, �� ��Ÿ�Ͽ� Ŀ���� �ΰ� A�� �����ٸ�
			if ((cursor->GetIsOtherUnit().name == ""))
			{
				this->charStatus = CharStatus::IsMoving;
				dynamic_cast<SelectionUI*>(DATACENTRE.GetCertainObject(ObjType::UI, "SelectionUI"))->SetToShow(SelectionUI::ToShow::SelectionBox);
				moveStartLocation = index; //����ϱ� ���� ���� ���
			}
			//���� Ȥ�� �Ʊ��� �ִ°��� Ŀ���� �ΰ� A�� ��������
			else
			{
				if ((cursor->GetIsOtherUnit().whos == OwnedBy::Enemy))
				{
					cursor->SetCursorTurn(IngameStatus::SelectionUI);		//UI���� ���� �ѱ�
					cursor->SetCursorTurnPrev(IngameStatus::PlayerTurn);	//UI���� ���� ���� �÷��̾����� ����

					//�弱��UI�� ��������
					SelectionUI* toSelect = dynamic_cast<SelectionUI*>(DATACENTRE.GetCertainObject(ObjType::UI, "SelectionUI"));
					toSelect->SetToShow(SelectionUI::ToShow::BattlePredict); //�������� ȭ���� �����ֶ�� ����
					toSelect->SetBattlePredict(this, dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::EnemyArmy, cursor->GetIsOtherUnit().name)));
					toSelect->SetBattlePredictInwards();
				}
				else if ((cursor->GetIsOtherUnit().whos == OwnedBy::Player))
				{
					//TODO:�Ʊ� �� ����
				}
			}
		}
		break;
	}
	//�������� �ʰ�, �ൿ�� ���ϰ�, �׸����� �ʰ�..
	case CharStatus::Disable:

		break;
	case CharStatus::EnemyPhase:
	{
		switch (enemyPhaseProc)
		{
			//�� Ŀ�� �Űܵ�
		case 0:
		{
			attackable = false;
			cursor->SetIndex(this->index);
			cursor->GameObject::SetPositionViaIndex();
			foeTiles.clear();
			allyTiles.clear();
			enemyToPlayer.clear();
			enemyPhaseProc++;
			break;
		}
		//�庻�� �ֺ����� �˻�
		case 1:
		{
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
					UpValidity &= checkTarget->GetMovingT() == "";	//��ֹ�2 �ִ��� ������
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
					LeftValidity &= checkTarget->GetMovingT() == "";
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
					DownValidity &= checkTarget->GetMovingT() == "";
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
					RightValidity &= checkTarget->GetMovingT() == "";
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

			//����� ���� ������ ���ٸ� A��Ÿ�� �̵�
			if (foeTiles.size() == 0)
			{
				//�˻��Ѱ� ��ó��
				for (auto& toClean : AvailableTiles)
				{
					toClean->SetCheckedNum(0);
				}
				purpleTiles.clear();
				blueTiles.clear();
				redTiles.clear();
				greenTiles.clear();
				AvailableTiles.clear();


				enemyPhaseProc++;
			}
			else
			{
				//�忬�� ���� ��� �ൿ������ ���� �׼�
				for (auto& toCal : AvailableTiles)
				{
					if (toCal->GetCheckedNum() <= ActionRange)
					{
						if (occupation == Occupation::Cleric || occupation == Occupation::WarCleric)
						{
							//��׸� �������� ����
							toCal->SetRouteNum(toCal->GetCheckedNum());
							toCal->SetCheckedNum(0);
							greenTiles.insert(toCal);
						}
						else
						{
							//�廡�� �������� ����
							redTiles.insert(toCal);
						}
					}
					else
					{
						//���� ���� ���� ����
						toCal->SetRouteNum(toCal->GetCheckedNum());
						toCal->SetCheckedNum(0);
						blueTiles.insert(toCal);
					}
				}
				//���߰������� ���� ����Ÿ�Ͽ� ���� Ư��ó��
				for (auto& redTile : redTiles)
				{
					redTile->SetRouteNum(redTile->GetCheckedNum());
					redTile->SetCheckedNum(0);
				}

				auto& MoveContainer = AvailableTiles;
				enemyToPlayer.clear(); //�÷��̾� ���ֱ��� ��� �ϴ� ����
				searchStarted = (*(foeTiles.begin())); //���� ���� ���� �߰ߵ� �ָ� Ÿ������ ����
				assert(searchStarted != nullptr); //�����˻�

				//Ÿ�� �����ſ� �ش��ϴ� �÷��̾� ĳ���� ã�Ƴ�
				for (auto& toFind : DATACENTRE.RefObjects(ObjType::PlayerArmy))
				{
					if (PointCompare(toFind.second->GetIndex(), searchStarted->GetIndex()))
					{
						cursor->SetIsOtherUnitOn(toFind.first, OwnedBy::Player);
						break;
					}
				}
				assert(cursor->GetIsOtherUnit().name != ""); //��ã���� ����� �����˻�

				//�屸����
				while (!PointCompare(searchStarted->GetIndex(), this->index))
				{
					if (dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, TwoDimentionArrayToOneString(searchStarted->GetIndex(), TILECOLX)))->GetRouteNum() > ActionRange)
						enemyToPlayer.push_back(searchStarted);

					int toFind = searchStarted->GetRouteNum();
					for (auto& availableTile : MoveContainer)
					{

						assert((dynamic_cast<Tiles*>(availableTile)) != nullptr);
						if ((dynamic_cast<Tiles*>(availableTile)->GetRouteNum() == toFind + 1)
							&& (((availableTile->GetIndex().x == searchStarted->GetIndex().x + 1) && (availableTile->GetIndex().y == searchStarted->GetIndex().y))
								|| ((availableTile->GetIndex().x == searchStarted->GetIndex().x - 1) && (availableTile->GetIndex().y == searchStarted->GetIndex().y))
								|| ((availableTile->GetIndex().x == searchStarted->GetIndex().x) && (availableTile->GetIndex().y - 1 == searchStarted->GetIndex().y))
								|| ((availableTile->GetIndex().x == searchStarted->GetIndex().x) && (availableTile->GetIndex().y + 1 == searchStarted->GetIndex().y))))
						{
							searchStarted = dynamic_cast<Tiles*>(availableTile);
							break;
						}
					}
				}

				attackable = true;
				enemyToPlayer.push_back(dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, TwoDimentionArrayToOneString(index, TILECOLX))));

				std::reverse(enemyToPlayer.begin(), enemyToPlayer.end());

				enemyPhaseProc += 2;
			}
			break;
		}
		//���밡 ���� ���� ��� �ָ� ã�ư��� �� ��
		case 2:
		{
			//��þ� �۱����� Ÿ�ϵ��� ����ؾ� �ϴ� �̹��� ��� Ÿ���� �����ͼ� �ӽ÷� ��Ƶ�
			auto& allTiles = DATACENTRE.RefObjects(ObjType::Tile);
			Tiles* start = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, TwoDimentionArrayToOneString(index, TILECOLX)));

			//����� ���޹��� ���� ���� ����� ������ ã�Ƽ� Ÿ������ ����
			FLOAT shortest = INFINITY;
			std::string shortestChar = "";
			for (auto& toExamineDistance : DATACENTRE.RefObjects(ObjType::PlayerArmy))
			{
				if (dynamic_cast<Character*>(toExamineDistance.second)->GetStatus() == Character::CharStatus::IsDying) continue;
				FLOAT calculated = GetDistance(this->index, toExamineDistance.second->GetIndex());
				if (shortest > calculated)
				{
					shortest = calculated;
					shortestChar = toExamineDistance.first;
				}
			}

			Tiles* reach = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, TwoDimentionArrayToOneString(DATACENTRE.GetCertainObject(ObjType::PlayerArmy, shortestChar)->GetIndex(), TILECOLX)));;

			//��Ÿ�ٱ����� �� ã�� ���� ���� �ʱ�ȭ
			for (auto& eachTile : allTiles)
			{
				dynamic_cast<Tiles*>(eachTile.second)->RefIsVisited() = false; //�˻� ���۽� �湮���� ���� ����
				dynamic_cast<Tiles*>(eachTile.second)->RefGlobalGoal() = INFINITY; //�����Ҷ� ��� Ÿ�ϵ��� ������������ �Ÿ��� ū �� ��Ƶ�
				dynamic_cast<Tiles*>(eachTile.second)->RefLocalGoal() = INFINITY; //�����Ҷ� ��� Ÿ�ϵ��� ������������ �Ÿ��� ū �� ��Ƶ�
				dynamic_cast<Tiles*>(eachTile.second)->SetParentPtr(nullptr); //�θ� ������ �����
			}

			//��Ÿ� ���ϴ� ���ٽ�
			auto distance = [](Tiles * a, Tiles * b)
			{
				//return (float)sqrtf((a->GetIndex().x - b->GetIndex().x)*(a->GetIndex().x - b->GetIndex().x) + (a->GetIndex().y - b->GetIndex().y)*(a->GetIndex().y - b->GetIndex().y)); //�Ǵٰ�󽺷� �޸���ƽ ���
				return (float)abs(a->GetIndex().x - b->GetIndex().x) + abs(a->GetIndex().y - b->GetIndex().y); //����ư ������� �޸���ƽ ���
			};
			//���޸���ƽ ���ϴ� ���ٽ�
			auto heuristic = [distance](Tiles * a, Tiles * b)//�޸���ƽ ������ ���. ��� �׳� ���Ͻ� ������
			{
				return (float)distance(a, b);
			};

			//������ ��带 ���۳��� ����
			Tiles * nodeCurrent = start;
			start->fLocalGoal = 0.0f;
			start->fGlobalGoal = heuristic(start, reach); //�͸��Լ� ���ٽ� ���

			//��Ÿ�� �����͵��� ���� �ӽ� ����Ʈ ����
			std::list<Tiles*> listNotTestedNodes;
			listNotTestedNodes.push_back(start); //�������� �־ �˻� ����

			//�忩�⼭���� ����
			while (!listNotTestedNodes.empty() && nodeCurrent != reach) //Ÿ�������͵��� ����Ʈ�� ������� �ʰ�, �������� �������� �ʾ������
			{
				//������ �˻����� �������� �̿�Ÿ�ϵ��� ���� �˻� �����Ѵ�
				for (auto& nodeNeighbour : nodeCurrent->RefNeighbours())
				{
					//������ ��� �ִ� �̿�Ÿ���� �̹� �湮�� Ÿ���� �ƴϰų� ��ֹ��� �ƴ϶��, 
					if (!(nodeNeighbour->RefIsVisited() || nodeNeighbour->GetObjT() != ""))
						listNotTestedNodes.push_back(nodeNeighbour); //���� �˻����� �̿�Ÿ���� �̰˻� ����Ʈ�� �������

					//��(���� �˻����� �̿�Ÿ��)�� (�̿�Ÿ���� �Ļ��� ���� Ÿ��)���� �Ÿ� + ���������κ����� �Ÿ� ����ؼ� ��� �����ص�
					float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

					//�������� ����� ����, �̿�Ÿ���� �����ϴ������� ������ ��ü. �� ÷�� fLocal�� ���Ѵ� ����־����� ����ϼ�!
					if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
					{
						nodeNeighbour->parent = nodeCurrent; //�̿� Ÿ���� �θ��尡 ���� ���� �˻����� Ÿ����
						nodeNeighbour->fLocalGoal = fPossiblyLowerGoal; //���� ��갪 ����
						nodeNeighbour->fGlobalGoal = (float)nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, reach); //�۷ι� �ԽѰ� ����
					}
				}
				//���ǥ���� ���� ����� ��带 �����ϱ� ���� �޸���ƽ ���� ���� ������ ����
				listNotTestedNodes.sort([](const Tiles * lhs, const Tiles * rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

				//���ǥ���� �޸���ƽ�� ������ ���� ������ �����Ƿ� �� �� ����
				while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->RefIsVisited())
					listNotTestedNodes.pop_front(); //������� �޸���ƽ�� {8,8,8,9,9,9,9} �̷����̸� {8,9,9,9,9} �̷��� ����ġ �ϳ��� ���ΰ�

				//����̻� � ��尡 ������� ���� ����
				if (listNotTestedNodes.empty())
					break;

				//���޸���ƽ ���������� �����ѰŴ�, ���� �� �տ��ִ�, �� �޸���ƽ �� ���� ���� �̵� �� �ٽ� �ݺ��� ���� ����
				nodeCurrent = listNotTestedNodes.front();
				nodeCurrent->RefIsVisited() = true;

			}
			listNotTestedNodes.clear(); //�̹� �˻�� �� ������ ���




			//����������� ���ϴºκ�
			if (reach != nullptr) //�������� �����Ѵٸ�
			{
				Tiles* inspect = reach; //�ӽ÷� �� ��带 ��Ƶΰ�
				while (inspect->GetParentPtr() != nullptr) //�� ��弭 �θ� �����Ѵٸ�
				{
					enemyToPlayer.push_back(inspect);
					inspect = inspect->parent; //�� ��� �θ� ���� �����͸� �ٲ�
				}
			}
			std::reverse(enemyToPlayer.begin(), enemyToPlayer.end());
			int toPop = enemyToPlayer.size()+1;
			for (UINT i = 0; i < toPop - this->GetMoveRange(); i++)
			{
				enemyToPlayer.pop_back();
			}
			
			enemyPhaseProc++;
			
			break;
		}
		case 3:
			
			//���̵��ϰ��� �ϴ� �����̳ʰ� �ϳ��� �����Ҷ��� ����
			if (enemyToPlayer.size() > 0)
			{
				//���̵��ϰ��� �ϴ� Ÿ�Ͽ� ���� ������, �̵����� �ʰ� �ٶ󺸴� ���⸸ �ٲ۴�
				if (foeTiles.size()!=0&& PointCompare((*(foeTiles.begin()))->GetIndex(), enemyToPlayer.front()->GetIndex()) && enemyToPlayer.size() == 1)
				{
					float directionAngle = GetAngleDegree(RectLeftTopOnly(position), RectLeftTopOnly(enemyToPlayer.front()->GetPosition()));
					if (directionAngle == 0)
					{
						movingDirection = MovingDirection::RIGHT;
					}
					else if (directionAngle == 90)
					{
						movingDirection = MovingDirection::LEFT;
					}
					else if (directionAngle == 180)
					{
						movingDirection = MovingDirection::TOP;
					}
					else if (directionAngle == 270)
					{
						movingDirection = MovingDirection::BOTTOM;
					}
					assert(!enemyToPlayer.empty());
					enemyToPlayer.erase(enemyToPlayer.begin());
				}
				//�常�� ���� ���ڶ��� ��� ��ǥ�� �̵� �Ϸᰡ �Ǿ�����
				else if (PointCompare(RectLeftTopOnly(position), RectLeftTopOnly(enemyToPlayer.front()->GetPosition())))
				{
					index = enemyToPlayer.front()->GetIndex(); //�� ĳ������ �ε��� ���� �������ְ�
					assert(!enemyToPlayer.empty());
					enemyToPlayer.erase(enemyToPlayer.begin());
				}
				else
				{
					assert(TILESIZE % moveSpeed == 0); //�̵��ӵ��� 48�� ����� �ƴϸ� �Ͷ߸�
					float directionAngle = GetAngleDegree(RectLeftTopOnly(position), RectLeftTopOnly(enemyToPlayer.front()->GetPosition()));

					if (directionAngle >= 337.5 && directionAngle <= 360.f || directionAngle >= 0 && directionAngle < 22.5f)
					{
						position.left += moveSpeed;
						position.right += moveSpeed;
						movingDirection = MovingDirection::RIGHT;
					}
					else if (directionAngle >= 22.5f && directionAngle < 67.5f)
					{
						movingDirection = MovingDirection::RIGHTTOP;
						assert(!"���⼭ �� ������ ������ �ȵǴµ� ����");
					}
					else if (directionAngle >= 67.5f && directionAngle < 112.5f)
					{
						position.top -= moveSpeed;
						position.bottom -= moveSpeed;
						movingDirection = MovingDirection::TOP;
					}
					else if (directionAngle >= 112.5f && directionAngle < 157.5f)
					{
						movingDirection = MovingDirection::RIGHTBOTTOM;
						assert(!"���⼭ �� ������ ������ �ȵǴµ� ����");
					}
					else if (directionAngle >= 157.5f && directionAngle < 202.5)
					{
						position.left -= moveSpeed;
						position.right -= moveSpeed;
						movingDirection = MovingDirection::LEFT;
					}
					else if (directionAngle >= 202.5 && directionAngle < 247.5)
					{
						movingDirection = MovingDirection::LEFTBOTTOM;
						assert(!"���⼭ �� ������ ������ �ȵǴµ� ����");
					}
					else if (directionAngle >= 247.5 && directionAngle < 292.5)
					{
						position.top += moveSpeed;
						position.bottom += moveSpeed;
						movingDirection = MovingDirection::BOTTOM;
					}
					else if (directionAngle >= 292.5 && directionAngle < 337.5)
					{
						movingDirection = MovingDirection::LEFTTOP;
						assert(!"���⼭ �� ������ ������ �ȵǴµ� ����");
					}
					else
					{
						assert(!"Wrong Angle");
					}

					cursor->SetPosition(position);
				}
			}
			//���� ĳ���Ͱ� ����������
			else
			{
				for (auto& toClean : AvailableTiles)
				{
					toClean->SetCheckedNum(0);
				}

				blueTiles.clear();
				redTiles.clear();
				greenTiles.clear();
				AvailableTiles.clear();
				foeTiles.clear();
				allyTiles.clear();

				DATACENTRE.ClearObjects(ObjType::BlueTiles);
				DATACENTRE.ClearObjects(ObjType::FoeTiles);
				DATACENTRE.ClearObjects(ObjType::AllyTiles);

				if (attackable)
				{
					cursor->SetIndex(index);
					cursor->SetPositionViaIndex();
					enemyPhaseProc = 0;
					cursor->SetCursorTurn(IngameStatus::ExecutingBattle);
					cursor->SetCursorTurnPrev(IngameStatus::EnemyTurn);
					charStatus = CharStatus::IsAttacking;
					dynamic_cast<ExecuteBattle*> (DATACENTRE.GetCertainObject(ObjType::Battle, "BattleManager"))->SetBattleState(ExecuteBattle::BattleState::AttackerAttacking);
					dynamic_cast<ExecuteBattle*> (DATACENTRE.GetCertainObject(ObjType::Battle, "BattleManager"))->SetWhoBattles(this ,dynamic_cast<Character *> (DATACENTRE.GetCertainObject(ObjType::PlayerArmy, cursor->GetIsOtherUnit().name)));
				}
				else
				{
					cursor->SetIndex(index);
					cursor->SetPositionViaIndex();
					enemyPhaseProc = 0;
					charStatus = CharStatus::IsActed;
					cursor->SetCursorOccupied("");
				}
			}

			break;

		default:
			assert(!"���ʹ� AI ��������");
			break;
		}





		break;
	}
	case CharStatus::IsDeadTalking:
		cursor->SetCursorTurn(IngameStatus::SelectionUI);
		cursor->SetCursorTurnPrev(IngameStatus::PlayerTurn);
		dynamic_cast<SelectionUI*>(DATACENTRE.GetCertainObject(ObjType::UI, "SelectionUI"))->SetToShow(SelectionUI::ToShow::AllyDead);
		dynamic_cast<SelectionUI*>(DATACENTRE.GetCertainObject(ObjType::UI, "SelectionUI"))->SetPhotoFrameAlphaZero();
		dynamic_cast<SelectionUI*>(DATACENTRE.GetCertainObject(ObjType::UI, "SelectionUI"))->SetDeadChar(this);

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
			if (DATACENTRE.CheckObjectExistance(ObjType::FoeTiles, TwoDimentionArrayToOneString(toMove.back()->GetIndex(), TILECOLX)) || DATACENTRE.CheckObjectExistance(ObjType::AllyTiles, TwoDimentionArrayToOneString(toMove.back()->GetIndex(), TILECOLX)) && toMove.size() == 1)
			{
				float directionAngle = GetAngleDegree(RectLeftTopOnly(position), RectLeftTopOnly(toMove.back()->GetPosition()));
				if (directionAngle == 0)
				{
					movingDirection = MovingDirection::RIGHT;
				}
				else if (directionAngle == 90)
				{
					movingDirection = MovingDirection::LEFT;
				}
				else if (directionAngle == 180)
				{
					movingDirection = MovingDirection::TOP;
				}
				else if (directionAngle == 270)
				{
					movingDirection = MovingDirection::BOTTOM;
				}
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
				float directionAngle = GetAngleDegree(RectLeftTopOnly(position), RectLeftTopOnly(toMove.back()->GetPosition()));

				if (directionAngle >= 337.5 && directionAngle <= 360.f || directionAngle >= 0 && directionAngle < 22.5f)
				{
					position.left += moveSpeed;
					position.right += moveSpeed;
					movingDirection = MovingDirection::RIGHT;
				}
				else if (directionAngle >= 22.5f && directionAngle < 67.5f)
				{
					movingDirection = MovingDirection::RIGHTTOP;
					assert(!"���⼭ �� ������ ������ �ȵǴµ� ����");
				}
				else if (directionAngle >= 67.5f && directionAngle < 112.5f)
				{
					position.top -= moveSpeed;
					position.bottom -= moveSpeed;
					movingDirection = MovingDirection::TOP;
				}
				else if (directionAngle >= 112.5f && directionAngle < 157.5f)
				{
					movingDirection = MovingDirection::RIGHTBOTTOM;
					assert(!"���⼭ �� ������ ������ �ȵǴµ� ����");
				}
				else if (directionAngle >= 157.5f && directionAngle < 202.5)
				{
					position.left -= moveSpeed;
					position.right -= moveSpeed;
					movingDirection = MovingDirection::LEFT;
				}
				else if (directionAngle >= 202.5 && directionAngle < 247.5)
				{
					movingDirection = MovingDirection::LEFTBOTTOM;
					assert(!"���⼭ �� ������ ������ �ȵǴµ� ����");
				}
				else if (directionAngle >= 247.5 && directionAngle < 292.5)
				{
					position.top += moveSpeed;
					position.bottom += moveSpeed;
					movingDirection = MovingDirection::BOTTOM;
				}
				else if (directionAngle >= 292.5 && directionAngle < 337.5)
				{
					movingDirection = MovingDirection::LEFTTOP;
					assert(!"���⼭ �� ������ ������ �ȵǴµ� ����");
				}
				else
				{
					assert(!"Wrong Angle");
				}
			}
		}
		//���� ĳ���Ͱ� ����������
		else
		{
			if (isInBattle)
			{
				DisableMoveRange();
				cursor->SetCursorTurn(IngameStatus::ExecutingBattle);
				cursor->SetCursorTurnPrev(IngameStatus::PlayerTurn);
				dynamic_cast<ExecuteBattle*> (DATACENTRE.GetCertainObject(ObjType::Battle, "BattleManager"))->SetBattleState(ExecuteBattle::BattleState::AttackerAttacking);

				charStatus = CharStatus::IsAttacking;
			}
			else
			{
				cursor->SetCursorTurn(IngameStatus::SelectionUI);
				cursor->SetCursorTurnPrev(IngameStatus::PlayerTurn);
				dynamic_cast<SelectionUI*>(DATACENTRE.GetCertainObject(ObjType::UI, "SelectionUI"))->SetToShow(SelectionUI::ToShow::SelectionBox);
				dynamic_cast<SelectionUI*>(DATACENTRE.GetCertainObject(ObjType::UI, "SelectionUI"))->SetPhotoFrameAlphaZero();
			}

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
			UpValidity &= checkTarget->GetMovingT() == "";	//��ֹ�2 �ִ��� ������
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
			LeftValidity &= checkTarget->GetMovingT() == "";
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
			DownValidity &= checkTarget->GetMovingT() == "";
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
			RightValidity &= checkTarget->GetMovingT() == "";
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
	else if (charStatus == CharStatus::IsActed)
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

	switch (occupation)
	{
	case GameObject::Occupation::Assassin:
		occupationData.Speed = 15;
		occupationData.Range = 1;
		occupationData.Health = 20;
		occupationData.Attack = 18;
		occupationData.Defence = 2;
		occupationData.Luck = 95;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Sword;
		break;
	case GameObject::Occupation::Swordsman:
		occupationData.Range = 1;
		occupationData.Speed = 5;
		occupationData.Health = 25;
		occupationData.Attack = 5;
		occupationData.Defence = 5;
		occupationData.Luck = 90;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Sword;
		break;
	case GameObject::Occupation::Myrmidon:
		occupationData.Range = 1;
		occupationData.Speed = 7;
		occupationData.Health = 25;
		occupationData.Attack = 10;
		occupationData.Defence = 4;
		occupationData.Luck = 80;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Sword;
		break;
	case GameObject::Occupation::Mercenary:
		occupationData.Range = 1;
		occupationData.Speed = 7;
		occupationData.Health = 30;
		occupationData.Attack = 5;
		occupationData.Defence = 14;
		occupationData.Luck = 80;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Sword;
		break;
	case GameObject::Occupation::SwordMaster:
		occupationData.Range = 1;
		occupationData.Speed = 10;
		occupationData.Health = 28;
		occupationData.Attack = 18;
		occupationData.Defence = 1;
		occupationData.Luck = 80;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Sword;
		break;
	case GameObject::Occupation::Archer:
		occupationData.Speed = 5;
		occupationData.Range = 2;
		occupationData.Health = 15;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 70;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Bow;
		break;
	case GameObject::Occupation::Sniper:
		occupationData.Speed = 1;
		occupationData.Range = 3;
		occupationData.Health = 15;
		occupationData.Attack = 10;
		occupationData.Defence = 2;
		occupationData.Luck = 70;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Bow;
		break;
	case GameObject::Occupation::GreatLord:
		occupationData.Range = 1;
		occupationData.Speed = 10;
		occupationData.Health = 30;
		occupationData.Attack = 10;
		occupationData.Defence = 10;
		occupationData.Luck = 80;
		occupationData.Move = 6;
		occupationData.Weapon = WeaponType::Sword;
		break;
	case GameObject::Occupation::Knight:
		occupationData.Range = 1;
		occupationData.Speed = 15;
		occupationData.Health = 25;
		occupationData.Attack = 5;
		occupationData.Defence = 5;
		occupationData.Luck = 90;
		occupationData.Move = 3;
		occupationData.Weapon = WeaponType::Lance;
		break;
	case GameObject::Occupation::WarCleric:
		occupationData.Speed = 7;
		occupationData.Range = 2;
		occupationData.Health = 25;
		occupationData.Attack = 2;
		occupationData.Defence = 1;
		occupationData.Luck = 100;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Staff;
		break;
	case GameObject::Occupation::Hero:
		occupationData.Range = 1;
		occupationData.Speed = 5;
		occupationData.Health = 30;
		occupationData.Attack = 3;
		occupationData.Defence = 20;
		occupationData.Luck = 90;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Sword;
		break;
	case GameObject::Occupation::Sage:
		occupationData.Speed = 5;
		occupationData.Range = 2;
		occupationData.Health = 20;
		occupationData.Attack = 12;
		occupationData.Defence = 2;
		occupationData.Luck = 80;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Tomb;
		break;
	case GameObject::Occupation::Mage:
		occupationData.Speed = 5;
		occupationData.Range = 2;
		occupationData.Health = 20;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 80;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Tomb;
		break;
	case GameObject::Occupation::Cleric:
		occupationData.Speed = 5;
		occupationData.Range = 1;
		occupationData.Health = 15;
		occupationData.Attack = 2;
		occupationData.Defence = 1;
		occupationData.Luck = 100;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Staff;
		break;
	case GameObject::Occupation::Grandmaster:
		occupationData.Speed = 5;
		occupationData.Range = 2;
		occupationData.Health = 20;
		occupationData.Attack = 12;
		occupationData.Defence = 2;
		occupationData.Luck = 80;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Tomb;
		break;
	case GameObject::Occupation::Tactician:
		occupationData.Speed = 5;
		occupationData.Range = 2;
		occupationData.Health = 20;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 80;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Tomb;
		break;
	case GameObject::Occupation::BowKnight:
		occupationData.Speed = 1;
		occupationData.Range = 2;
		occupationData.Health = 15;
		occupationData.Attack = 10;
		occupationData.Defence = 3;
		occupationData.Luck = 60;
		occupationData.Move = 7;
		occupationData.Weapon = WeaponType::Bow;
		break;
	case GameObject::Occupation::DarkKnight:
		occupationData.Range = 1;
		occupationData.Speed = 10;
		occupationData.Health = 28;
		occupationData.Attack = 18;
		occupationData.Defence = 1;
		occupationData.Luck = 80;
		occupationData.Move = 7;
		occupationData.Weapon = WeaponType::Sword;
		break;
	case GameObject::Occupation::GreatKnight:
		occupationData.Speed = 15;
		occupationData.Range = 1;
		occupationData.Health = 20;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 80;
		occupationData.Move = 7;
		occupationData.Weapon = WeaponType::Axe;
		break;
	case GameObject::Occupation::Cavalier:
		occupationData.Range = 1;
		occupationData.Speed = 10;
		occupationData.Health = 28;
		occupationData.Attack = 8;
		occupationData.Defence = 1;
		occupationData.Luck = 80;
		occupationData.Move = 7;
		occupationData.Weapon = WeaponType::Lance;
		break;
	case GameObject::Occupation::Paladin:
		occupationData.Range = 1;
		occupationData.Speed = 10;
		occupationData.Health = 28;
		occupationData.Attack = 10;
		occupationData.Defence = 5;
		occupationData.Luck = 80;
		occupationData.Move = 7;
		occupationData.Weapon = WeaponType::Lance;
		break;
	case GameObject::Occupation::Troubadour:
		occupationData.Speed = 5;
		occupationData.Range = 1;
		occupationData.Health = 20;
		occupationData.Attack = 2;
		occupationData.Defence = 2;
		occupationData.Luck = 100;
		occupationData.Move = 7;
		occupationData.Weapon = WeaponType::Staff;
		break;
	case GameObject::Occupation::Valkyrie:
		occupationData.Speed = 5;
		occupationData.Range = 1;
		occupationData.Health = 25;
		occupationData.Attack = 2;
		occupationData.Defence = 5;
		occupationData.Luck = 100;
		occupationData.Move = 7;
		occupationData.Weapon = WeaponType::Staff;
		break;
	case GameObject::Occupation::Dancer:
		occupationData.Range = 1;
		occupationData.Speed = 5;
		occupationData.Health = 20;
		occupationData.Attack = 5;
		occupationData.Defence = 3;
		occupationData.Luck = 90;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Sword;
		break;
	case GameObject::Occupation::DarkMage:
		occupationData.Speed = 5;
		occupationData.Range = 2;
		occupationData.Health = 20;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 80;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Tomb;
		break;
	case GameObject::Occupation::Sorcerer:
		occupationData.Speed = 5;
		occupationData.Range = 2;
		occupationData.Health = 20;
		occupationData.Attack = 12;
		occupationData.Defence = 2;
		occupationData.Luck = 80;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Tomb;
		break;
	case GameObject::Occupation::Theif:
		occupationData.Speed = 15;
		occupationData.Range = 1;
		occupationData.Health = 20;
		occupationData.Attack = 10;
		occupationData.Defence = 2;
		occupationData.Luck = 95;
		occupationData.Move = 5;
		occupationData.Weapon = WeaponType::Sword;
		break;
	default:
		assert(false && "Occupation does not exist"); //�������� �ʴ� ���� ������ �Ͷ߸�
		break;
	}
}

void Character::SetCurrentHpSubtractByValue(INT _changedValue)
{
	currentHealth -= _changedValue;
	if (currentHealth <= 0) currentHealth = 0;
}

void Character::SetCurrentHpAddByValue(INT _changedValue)
{
	currentHealth += _changedValue;
	if (currentHealth <= 0) currentHealth = 0;
}

void Character::SetCurrentHealth(INT _currentHealth)
{
	currentHealth = _currentHealth;
	if (currentHealth <= 0) currentHealth = 0;
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
		if (charStatus == CharStatus::IsMoving || charStatus == CharStatus::IsAttacking || charStatus == CharStatus::EnemyPhase)
		{
			frameImg->RelativeFrameRender(position.left, position.top, frame.x + frameLoop, frame.y + (INT)movingDirection); //ī�޶� ��� ����
			healthBarBackground = HealthBarBackLocater(healthBarBackground, this->position);
			D2DRENDERER->RelativeFillRectangle(healthBarBackground, D2DRenderer::DefaultBrush::White);
			float toCalculate = static_cast<FLOAT>(currentHealth) / static_cast<FLOAT>(GetHealth());
			assert(toCalculate < 1.01);
			if (toCalculate > 0.6)
			{
				D2DRENDERER->RelativeFillRectangle(HealthBarFrontLocater(healthBarBackground, GetHealth(), currentHealth), D2DRenderer::DefaultBrush::Green);
			}
			else if (toCalculate > 0.3)
			{
				D2DRENDERER->RelativeFillRectangle(HealthBarFrontLocater(healthBarBackground, GetHealth(), currentHealth), D2DRenderer::DefaultBrush::Yellow);
			}
			else
			{
				D2DRENDERER->RelativeFillRectangle(HealthBarFrontLocater(healthBarBackground, GetHealth(), currentHealth), D2DRenderer::DefaultBrush::Red);
			}
		}
		else if (charStatus == CharStatus::IsDying || charStatus == CharStatus::IsDead)
		{

			if (this->whosChar == OwnedBy::Player && !deadTalkInit)
			{
				cursor->SetCursorTurn(IngameStatus::SelectionUI);
				cursor->SetCursorTurnPrev(cursor->GetCursorTurnPrev());
				dynamic_cast<SelectionUI*>(DATACENTRE.GetCertainObject(ObjType::UI, "SelectionUI"))->SetToShow(SelectionUI::ToShow::AllyDead);
				dynamic_cast<SelectionUI*>(DATACENTRE.GetCertainObject(ObjType::UI, "SelectionUI"))->SetPhotoFrameAlphaZero();
				dynamic_cast<SelectionUI*>(DATACENTRE.GetCertainObject(ObjType::UI, "SelectionUI"))->SetDeadChar(this);
				SOUNDMANAGER->pause("�ΰ���BGM");
				SOUNDMANAGER->play("DeadBGM");
				//				charStatus = CharStatus::IsDeadTalking;
				deadTalkInit = true;
			}
			else if (this->whosChar == OwnedBy::Player && deadTalkInit && !deadTalkEnd) {}
			else
			{
				if (characterAlpha > 0)
				{
					characterAlpha -= 0.02f;
				}
				else
				{
					characterAlpha = 0;
					charStatus = CharStatus::IsDead;
				}
			}
			healthBarBackground = HealthBarBackLocater(healthBarBackground, this->position);
			healthBarBackground.right = healthBarBackground.left + ((healthBarBackground.right - healthBarBackground.left) * characterAlpha);
			D2DRENDERER->RelativeFillRectangle(healthBarBackground, D2DRenderer::DefaultBrush::White);

			frameImg->SetAlpha(characterAlpha);
			frameImg->RelativeFrameRender(position.left, position.top, frame.x, frame.y + (INT)movingDirection); //ī�޶� ��� ����

		}
		else if (charStatus == CharStatus::IsActed) //�ൿ�� ���߾��ٸ�
		{
			frameImg->RelativeFrameRender(position.left, position.top, frame.x + frameLoop, frame.y);  //ī�޶� ��� ����
			healthBarBackground = HealthBarBackLocater(healthBarBackground, this->position);
			D2DRENDERER->RelativeFillRectangle(healthBarBackground, D2DRenderer::DefaultBrush::White);
			float toCalculate = static_cast<FLOAT>(currentHealth) / static_cast<FLOAT>(GetHealth());
			assert(toCalculate < 1.01);
			if (toCalculate > 0.6)
			{
				D2DRENDERER->RelativeFillRectangle(HealthBarFrontLocater(healthBarBackground, GetHealth(), currentHealth), D2DRenderer::DefaultBrush::Green);
			}
			else if (toCalculate > 0.3)
			{
				D2DRENDERER->RelativeFillRectangle(HealthBarFrontLocater(healthBarBackground, GetHealth(), currentHealth), D2DRenderer::DefaultBrush::Yellow);
			}
			else
			{
				D2DRENDERER->RelativeFillRectangle(HealthBarFrontLocater(healthBarBackground, GetHealth(), currentHealth), D2DRenderer::DefaultBrush::Red);
			}
			IMAGEMANAGER->FindImage("ActionTaken")->SetAlpha(0.5f);
			IMAGEMANAGER->FindImage("ActionTaken")->SetSize({ TILESIZE, TILESIZE });
			IMAGEMANAGER->FindImage("ActionTaken")->RelativeRender(position.left, position.top); //��ο� �׸��ڷ� �������
		}
		else if (charStatus == CharStatus::IsDeadTalking)
		{
			cursor->SetCursorTurn(IngameStatus::SelectionUI);
			cursor->SetCursorTurnPrev(IngameStatus::PlayerTurn);
			dynamic_cast<SelectionUI*>(DATACENTRE.GetCertainObject(ObjType::UI, "SelectionUI"))->SetToShow(SelectionUI::ToShow::AllyDead);
			dynamic_cast<SelectionUI*>(DATACENTRE.GetCertainObject(ObjType::UI, "SelectionUI"))->SetPhotoFrameAlphaZero();
			dynamic_cast<SelectionUI*>(DATACENTRE.GetCertainObject(ObjType::UI, "SelectionUI"))->SetDeadChar(this);
			SOUNDMANAGER->pause("�ΰ���BGM");
			SOUNDMANAGER->play("DeadBGM");
		}
		else
		{

			frameImg->RelativeFrameRender(position.left, position.top, frame.x + frameLoop, frame.y);  //ī�޶� ��� ����
			healthBarBackground = HealthBarBackLocater(healthBarBackground, this->position);
			D2DRENDERER->RelativeFillRectangle(healthBarBackground, D2DRenderer::DefaultBrush::White);
			float toCalculate = static_cast<FLOAT>(currentHealth) / static_cast<FLOAT>(GetHealth());
			assert(toCalculate < 1.01);
			if (toCalculate > 0.6)
			{
				D2DRENDERER->RelativeFillRectangle(HealthBarFrontLocater(healthBarBackground, GetHealth(), currentHealth), D2DRenderer::DefaultBrush::Green);
			}
			else if (toCalculate > 0.3)
			{
				D2DRENDERER->RelativeFillRectangle(HealthBarFrontLocater(healthBarBackground, GetHealth(), currentHealth), D2DRenderer::DefaultBrush::Yellow);
			}
			else
			{
				D2DRENDERER->RelativeFillRectangle(HealthBarFrontLocater(healthBarBackground, GetHealth(), currentHealth), D2DRenderer::DefaultBrush::Red);
			}
		}
	}
	//��ĳ���� ������ ǥ�� �� �巡�����϶� ���� �̵���ο� ĳ���͸� �׸�
	if (charStatus == CharStatus::IsDragging)
	{
		frameImg->SetSize({ TILESIZE, TILESIZE });
		frameImg->SetAlpha(0.5f);
		frameImg->RelativeFrameRender(draggingIndexPrev.x * TILESIZE, draggingIndexPrev.y * TILESIZE, frame.x + frameLoop, frame.y + (UINT)draggindDirection);
	}
	for (auto& asd : enemyToPlayer)
		D2DRENDERER->FillRectangle(CAMERA.RelativeCameraRect(asd->GetPosition()),D2D1::ColorF::Red, 0.5);
}

