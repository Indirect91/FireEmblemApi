#include "../stdafx.h"
#include "SelectionUI.h"
#include "Cursor.h"
#include "Character.h"
#include "ExecuteBattle.h"

SelectionUI::SelectionUI()
{
	
	selectionBox.selectionBoxList = { 500,200,600,300 };
	selectionBox.selectionBoxTomove = { 500,200,600,300 };
	cursor = nullptr;
	photoFrameAlpha = 0;
	toShow = ToShow::Idle;

	battlePredict.enemyRender.charPtr = nullptr;
	battlePredict.enemyRender.RenderPortrait = { 0 };									//�� �ʻ�ȭ �׸� ��ġ
	battlePredict.enemyRender.RenderPortraitOriginal = { 758,100 };						//�� �ʻ�ȭ ����ġ
	battlePredict.enemyRender.RenderPortraitToMove = { 500,100 };						//�� �ʻ�ȭ �̵��ϰ��� �ϴ� ��ġ
	battlePredict.enemyRender.RenderName = { 0 };										//�� �̸� �׸� ��ġ
	battlePredict.enemyRender.RenderHealthPredict = { 0 };								//�� ü�� ���� �׸� ��ġ
	battlePredict.enemyRender.RenderHitRatePredict = { 0 };								//�� ���� ���� �׸� ��ġ
	battlePredict.enemyRender.RenderWeaponAdvantage = { 0 };							//�� ���� �� �׸� ��ġ

	battlePredict.thisCharRender.charPtr = nullptr;										//���� ĳ���� ������
	battlePredict.thisCharRender.RenderPortrait = { 0 };								//���� ĳ���� �ʻ�ȭ �׸� ��ġ
	battlePredict.thisCharRender.RenderPortraitOriginal = { -158,100 };					//���� ĳ���� ����ġ
	battlePredict.thisCharRender.RenderPortraitToMove = { 100,100 };					//���� ĳ���� �̵��ϰ��� �ϴ� ��ġ
	battlePredict.thisCharRender.RenderName = { 0 };									//���� ĳ���� �̸� �׸� ��ġ
	battlePredict.thisCharRender.RenderHealthPredict = { 0 };							//���� ĳ���� ü�� ���� �׸� ��ġ
	battlePredict.thisCharRender.RenderHitRatePredict = { 0 };							//���� ĳ���� ���� ���� �׸� ��ġ
	battlePredict.thisCharRender.RenderWeaponAdvantage = { 0 };							//���� ĳ���� ���� �� �׸� ��ġ
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
			dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::PlayerArmy, cursor->GetCursorOccupied()))->SetStatus(Character::CharStatus::IsActed);

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

			if (!PointCompare(battlePredict.thisCharRender.RenderPortrait, battlePredict.thisCharRender.RenderPortraitOriginal))
			{
				battlePredict.thisCharRender.RenderPortrait.x -= 43;
			}
			if (!PointCompare(battlePredict.enemyRender.RenderPortrait, battlePredict.enemyRender.RenderPortraitOriginal))
			{
				battlePredict.enemyRender.RenderPortrait.x += 43;
			}
			if (battlePredict.thisCharRender.RenderPortrait.x == battlePredict.thisCharRender.RenderPortraitOriginal.x && battlePredict.enemyRender.RenderPortrait.x == battlePredict.enemyRender.RenderPortraitOriginal.x)
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
			if (!PointCompare(battlePredict.thisCharRender.RenderPortrait, battlePredict.thisCharRender.RenderPortraitToMove))
			{
				battlePredict.thisCharRender.RenderPortrait.x += 43;
			}
			//���� ĳ���� �̹����� ���� �̹��� �����ִ� ������ �������� ���Ͽ��ٸ�
			if (!PointCompare(battlePredict.enemyRender.RenderPortrait, battlePredict.enemyRender.RenderPortraitToMove))
			{
				battlePredict.enemyRender.RenderPortrait.x -= 43;
			}
			//���÷��̾�ĳ����, �� ĳ���� ��� ����ġ�� �����Ͽ��ٸ�
			if (battlePredict.thisCharRender.RenderPortrait.x == battlePredict.thisCharRender.RenderPortraitToMove.x && battlePredict.enemyRender.RenderPortrait.x == battlePredict.enemyRender.RenderPortraitToMove.x)
			{
				battlePredict.battleUIState = BattleUIState::IsWaitingForInput;
			}
			break;

		//������ ������ ������ ���� �϶�� ���� Ȯ���� ����������
		case SelectionUI::BattleUIState::IsBattleReady:
			if (photoFrameAlpha > 0.f) photoFrameAlpha -= 0.2f;

			if (!PointCompare(battlePredict.thisCharRender.RenderPortrait, battlePredict.thisCharRender.RenderPortraitOriginal))
			{
				battlePredict.thisCharRender.RenderPortrait.x -= 43;
			}
			if (!PointCompare(battlePredict.enemyRender.RenderPortrait, battlePredict.enemyRender.RenderPortraitOriginal))
			{
				battlePredict.enemyRender.RenderPortrait.x += 43;
			}
			if (battlePredict.thisCharRender.RenderPortrait.x == battlePredict.thisCharRender.RenderPortraitOriginal.x && battlePredict.enemyRender.RenderPortrait.x == battlePredict.enemyRender.RenderPortraitOriginal.x)
			{
				//cursor->SetCursorTurnPrev(cursor->GetCursorTurnPrev());
				cursor->SetCursorTurn(IngameStatus::ExecutingBattle);
				dynamic_cast<ExecuteBattle*> (DATACENTRE.GetCertainObject(ObjType::Battle, "BattleManager"))->SetWhoBattles(battlePredict.thisCharRender.charPtr, battlePredict.enemyRender.charPtr);
				
				//battlePredict.thisCharRender.charPtr->SetStatus(Character::CharStatus::IsAttacking);
				
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
	case SelectionUI::ToShow::AllyDead:
	{

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
	battlePredict.enemyRender.RenderPortrait = battlePredict.enemyRender.RenderPortraitOriginal;					//���� ���� �ܰ������� ���
	battlePredict.thisCharRender.RenderPortrait = battlePredict.thisCharRender.RenderPortraitOriginal;	//�Ʊ� �̹����� ���� �ܰ������� ���
}

void SelectionUI::SetBattlePredictOutwards()
{
	battlePredict.battleUIState = BattleUIState::IsMovingOutwards;
	battlePredict.enemyRender.RenderPortrait = battlePredict.enemyRender.RenderPortraitToMove;					//�� �̹����� 100������������
	battlePredict.thisCharRender.RenderPortrait = battlePredict.thisCharRender.RenderPortraitToMove;	//�Ʊ� �̹����� 500������������
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
		IMAGEMANAGER->FindImage("�ʻ�ȭ" + battlePredict.thisCharRender.charPtr->GetName())->Render(battlePredict.thisCharRender.RenderPortrait.x, battlePredict.thisCharRender.RenderPortrait.y);
		IMAGEMANAGER->FindImage("�ʻ�ȭ" + battlePredict.enemyRender.charPtr->GetName())->Render(battlePredict.enemyRender.RenderPortrait.x, battlePredict.enemyRender.RenderPortrait.y);

		break;
	case SelectionUI::ToShow::AllyDead:
		//IMAGEMANAGER->FindImage(charDead.deadCharacter->GetName()

	break;

	default:
		assert(false && "�����ִ� UI�� ��������");
		break;
	}


	IMAGEMANAGER->FindImage("PhotoFrame")->SetSize({ 800,640 });
	IMAGEMANAGER->FindImage("PhotoFrame")->SetAlpha(photoFrameAlpha);
	IMAGEMANAGER->FindImage("PhotoFrame")->Render(0, 0);
}

