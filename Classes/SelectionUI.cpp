#include "../stdafx.h"
#include "SelectionUI.h"
#include "Cursor.h"
#include "Character.h"
#include "ExecuteBattle.h"

SelectionUI::SelectionUI()
{
	
	selectionBox.selectionBoxList = { 500,200,600,300 };
	selectionBox.selectionBoxTomove = {};
	//battlePredict.thisCharacterRenderPortrait = {100,100};
	//battlePredict.enemyRenderPortrait = { 500,100 };
	cursor = nullptr;
	photoFrameAlpha = 0;
	toShow = ToShow::Idle;
}

void SelectionUI::Init()
{
	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
	assert(cursor != nullptr); //Ŀ�� ����� �ȵ������� �Ͷ߸�
}

void SelectionUI::SetToShow(ToShow _whatToShow)
{
	toShow = _whatToShow;
	this->SetPhotoFrameAlphaZero();
}

void SelectionUI::Release()
{
	//���⼭ ���Ҵ������ ����. ������ Ȥ�� ������ü��
}

void SelectionUI::Update()
{
	//if (photoFrameAlpha < 1.f) photoFrameAlpha += 0.1f;




	switch (toShow)
	
	{
	case SelectionUI::ToShow::Idle: //���� ��� �ϳ��� �ƴҶ� Ȥ�ó� ���Ë��� ������ ������ġ
		assert(!"���� ���� ���̵� ���� ���µ�??");
		break;

	case SelectionUI::ToShow::SelectionBox:
		//����¹ڽ� �޴��� �������� �̵�Ű�� ���� �ڽ� �̵�
		if (KEYMANAGER->IsOnceKeyDown(VK_UP))
		{
			selectionBox.selectionBoxList.top -= 150;
			selectionBox.selectionBoxList.bottom -= 150;
		}
		//��ٿ�
		else if (KEYMANAGER->IsOnceKeyDown(VK_DOWN))
		{
			selectionBox.selectionBoxList.top += 150;
			selectionBox.selectionBoxList.bottom += 150;
		}

		else if (KEYMANAGER->IsOnceKeyDown('A'))
		{

			dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::PlayerArmy, cursor->GetCursorOccupied()))->DisableMoveRange();
			dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::PlayerArmy, cursor->GetCursorOccupied()))->SetActionTaken(true);

			//DATACENTRE.ClearObjects(ObjType::FoeTiles);
			//DATACENTRE.ClearObjects(ObjType::AllyTiles);
			cursor->SetCursorOccupied("");
			cursor->SetCursorTurn(cursor->GetCursorTurnPrev());
			cursor->SetCursorTurnPrev(IngameStatus::SelectionUI);
		}

		//����� �̵� ���
		else if (KEYMANAGER->IsOnceKeyDown('S'))
		{
			Character* toExamine = dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::PlayerArmy, cursor->GetCursorOccupied()));
			toExamine->SetStatus(Character::CharStatus::IsDragging);

			toExamine->SetIndex(toExamine->GetMoveStartLocation());
			toExamine->SetPositionViaIndex();

			cursor->SetCursorTurn(cursor->GetCursorTurnPrev());
			cursor->SetCursorTurnPrev(IngameStatus::SelectionUI);
		}


		break;

	//������ ���� UI �����ֶ�� ��û ����������
	case SelectionUI::ToShow::BattlePredict:
	{
		//���������� UI ������ ���¿� ���� �޸� ����
		switch (battlePredict.battleUIState)
		{
		//���ʻ�ȭ ������ �̵��϶�� ���� ���ð��
		case SelectionUI::BattleUIState::IsMovingOutwards:

			if (photoFrameAlpha > 0.f) photoFrameAlpha -= 0.2f;

			if (!PointCompare(battlePredict.thisCharacterRenderPortrait, battlePredict.thisCharacterRenderPortraitOriginal))
			{
				battlePredict.thisCharacterRenderPortrait.x -= 43;
			}
			if (!PointCompare(battlePredict.enemyRenderPortrait, battlePredict.enemyRenderPortraitOriginal))
			{
				battlePredict.enemyRenderPortrait.x += 43;
			}
			if (battlePredict.thisCharacterRenderPortrait.x == battlePredict.thisCharacterRenderPortraitOriginal.x && battlePredict.enemyRenderPortrait.x == battlePredict.enemyRenderPortraitOriginal.x)
			{
				
				cursor->SetCursorTurn(cursor->GetCursorTurnPrev());
				cursor->SetCursorTurnPrev(IngameStatus::SelectionUI);
			}

			break;

		//���ʻ�ȭ ������ �̵��϶�� ���� ���ð��
		case SelectionUI::BattleUIState::IsMovingInwards:

			//����� ������ �̵��϶�� ���� ��������
			if (photoFrameAlpha < 1.f) photoFrameAlpha += 0.2f;
			//���÷��̾�ĳ���� �̹����� ���� �̹��� �����ִ� ������ �������� ���Ͽ��ٸ�
			if (!PointCompare(battlePredict.thisCharacterRenderPortrait, battlePredict.thisCharacterRenderPortraitToMove))
			{
				battlePredict.thisCharacterRenderPortrait.x += 43;
			}
			//���� ĳ���� �̹����� ���� �̹��� �����ִ� ������ �������� ���Ͽ��ٸ�
			if (!PointCompare(battlePredict.enemyRenderPortrait, battlePredict.enemyRenderPortraitToMove))
			{
				battlePredict.enemyRenderPortrait.x -= 43;
			}
			//���÷��̾�ĳ����, �� ĳ���� ��� ����ġ�� �����Ͽ��ٸ�
			if (battlePredict.thisCharacterRenderPortrait.x == battlePredict.thisCharacterRenderPortraitToMove.x && battlePredict.enemyRenderPortrait.x == battlePredict.enemyRenderPortraitToMove.x)
			{
				battlePredict.battleUIState = BattleUIState::IsWaitingForInput;
			}
			break;

		//������ ������ ������ ���� �϶�� ���� Ȯ���� ����������
		case SelectionUI::BattleUIState::IsBattleReady:
			if (photoFrameAlpha > 0.f) photoFrameAlpha -= 0.2f;

			if (!PointCompare(battlePredict.thisCharacterRenderPortrait, battlePredict.thisCharacterRenderPortraitOriginal))
			{
				battlePredict.thisCharacterRenderPortrait.x -= 43;
			}
			if (!PointCompare(battlePredict.enemyRenderPortrait, battlePredict.enemyRenderPortraitOriginal))
			{
				battlePredict.enemyRenderPortrait.x += 43;
			}
			if (battlePredict.thisCharacterRenderPortrait.x == battlePredict.thisCharacterRenderPortraitOriginal.x && battlePredict.enemyRenderPortrait.x == battlePredict.enemyRenderPortraitOriginal.x)
			{
				//cursor->SetCursorTurnPrev(cursor->GetCursorTurnPrev());
				cursor->SetCursorTurn(IngameStatus::ExecutingBattle);
				dynamic_cast<ExecuteBattle*> (DATACENTRE.GetCertainObject(ObjType::Battle, "BattleManager"))->SetWhoBattles(battlePredict.thisCharacterPtr, battlePredict.enemyPtr);
			}
			


			break;
		case SelectionUI::BattleUIState::IsWaitingForInput:

			//�忹��ȭ�鼭 ��� ������ �ʱ�ȭ
			if (KEYMANAGER->IsOnceKeyDown('S'))
				{battlePredict.battleUIState = BattleUIState::IsMovingOutwards;}

			//��A�� ������ ���� Ȯ������
			else if (KEYMANAGER->IsOnceKeyDown('A'))
				{battlePredict.battleUIState = BattleUIState::IsBattleReady;}

			break;

		default:
			assert(!"��ƲUI�ȸ���� ����");
			break;
		}

		break;
	}
	case SelectionUI::ToShow::HealPredict:
	{
		//�忹��ȭ�鼭 ��� ������ �ʱ�ȭ
		if (KEYMANAGER->IsOnceKeyDown('S'))
		{
			cursor->SetCursorTurn(cursor->GetCursorTurnPrev());
			cursor->SetCursorTurnPrev(IngameStatus::SelectionUI);
		}

		break;
	}
	default:
		assert(false && "SelectionUI not made yet");
		break;
	}
}

void SelectionUI::SetBattlePredictInwards()
{
	battlePredict.battleUIState = BattleUIState::IsMovingInwards;
	battlePredict.enemyRenderPortrait = battlePredict.enemyRenderPortraitOriginal;					//���� ���� �ܰ������� ���
	battlePredict.thisCharacterRenderPortrait = battlePredict.thisCharacterRenderPortraitOriginal;	//�Ʊ� �̹����� ���� �ܰ������� ���
}

void SelectionUI::SetBattlePredictOutwards()
{
	battlePredict.battleUIState = BattleUIState::IsMovingOutwards;
	battlePredict.enemyRenderPortrait = battlePredict.enemyRenderPortraitToMove;					//�� �̹����� 100������������
	battlePredict.thisCharacterRenderPortrait = battlePredict.thisCharacterRenderPortraitToMove;	//�Ʊ� �̹����� 500������������
}


void SelectionUI::Render()
{

	switch (toShow)
	{
	case SelectionUI::ToShow::SelectionBox:
		IMAGEMANAGER->FindImage("Blue")->SetSize({ 300, 100 });
		IMAGEMANAGER->FindImage("Blue")->Render(selectionBox.selectionBoxList.left, selectionBox.selectionBoxList.top);

		break;
	case SelectionUI::ToShow::BattlePredict:
		IMAGEMANAGER->FindImage("�ʻ�ȭ" + battlePredict.thisCharacterPtr->GetName())->Render(battlePredict.thisCharacterRenderPortrait.x, battlePredict.thisCharacterRenderPortrait.y);
		IMAGEMANAGER->FindImage("�ʻ�ȭ" + battlePredict.enemyPtr->GetName())->Render(battlePredict.enemyRenderPortrait.x, battlePredict.enemyRenderPortrait.y);

		break;
	default:
		assert(false && "�����ִ� UI�� ��������");
		break;
	}


	IMAGEMANAGER->FindImage("PhotoFrame")->SetSize({ 800,640 });
	IMAGEMANAGER->FindImage("PhotoFrame")->SetAlpha(photoFrameAlpha);
	IMAGEMANAGER->FindImage("PhotoFrame")->Render(0, 0);
}

