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
	assert(cursor != nullptr); //커서 제대로 안들어왔으면 터뜨림
}

void SelectionUI::SetToShow(ToShow _whatToShow)
{
	toShow = _whatToShow;
	this->SetPhotoFrameAlphaZero();
}

void SelectionUI::Release()
{
	//여기서 뉴할당받은거 없음. 포인터 혹은 정적개체들
}

void SelectionUI::Update()
{
	//if (photoFrameAlpha < 1.f) photoFrameAlpha += 0.1f;




	switch (toShow)
	
	{
	case SelectionUI::ToShow::Idle: //둘중 어느 하나도 아닐때 혹시나 들어올떄를 방지한 안전장치
		assert(!"머지 아직 아이들 준적 없는데??");
		break;

	case SelectionUI::ToShow::SelectionBox:
		//▼고르는박스 메뉴가 나왔을때 이동키에 따른 박스 이동
		if (KEYMANAGER->IsOnceKeyDown(VK_UP))
		{
			selectionBox.selectionBoxList.top -= 150;
			selectionBox.selectionBoxList.bottom -= 150;
		}
		//▼다운
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

		//▼모의 이동 취소
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

	//▼전투 예측 UI 보여주라고 요청 들어와있을시
	case SelectionUI::ToShow::BattlePredict:
	{
		//▼전투예측 UI 내에서 상태에 따라 달리 실행
		switch (battlePredict.battleUIState)
		{
		//▼초상화 밖으로 이동하라고 지시 들어올경우
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

		//▼초상화 안으로 이동하라고 지시 들어올경우
		case SelectionUI::BattleUIState::IsMovingInwards:

			//▼사진 안으로 이동하라는 지시 들어왔을시
			if (photoFrameAlpha < 1.f) photoFrameAlpha += 0.2f;
			//▼플레이어캐릭터 이미지가 아직 이미지 보여주는 지점에 도달하지 못하였다면
			if (!PointCompare(battlePredict.thisCharacterRenderPortrait, battlePredict.thisCharacterRenderPortraitToMove))
			{
				battlePredict.thisCharacterRenderPortrait.x += 43;
			}
			//▼적 캐릭터 이미지가 아직 이미지 보여주는 지점에 도달하지 못하였다면
			if (!PointCompare(battlePredict.enemyRenderPortrait, battlePredict.enemyRenderPortraitToMove))
			{
				battlePredict.enemyRenderPortrait.x -= 43;
			}
			//▼플레이어캐릭터, 적 캐릭터 모두 정위치에 도달하였다면
			if (battlePredict.thisCharacterRenderPortrait.x == battlePredict.thisCharacterRenderPortraitToMove.x && battlePredict.enemyRenderPortrait.x == battlePredict.enemyRenderPortraitToMove.x)
			{
				battlePredict.battleUIState = BattleUIState::IsWaitingForInput;
			}
			break;

		//▼전투 예측이 끝나고 전투 하라고 최종 확인이 떨어졌을때
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

			//▼예측화면서 취소 누르면 초기화
			if (KEYMANAGER->IsOnceKeyDown('S'))
				{battlePredict.battleUIState = BattleUIState::IsMovingOutwards;}

			//▼A를 누르면 공격 확정지음
			else if (KEYMANAGER->IsOnceKeyDown('A'))
				{battlePredict.battleUIState = BattleUIState::IsBattleReady;}

			break;

		default:
			assert(!"배틀UI안만든거 있음");
			break;
		}

		break;
	}
	case SelectionUI::ToShow::HealPredict:
	{
		//▼예측화면서 취소 누르면 초기화
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
	battlePredict.enemyRenderPortrait = battlePredict.enemyRenderPortraitOriginal;					//적은 우측 외각서부터 출발
	battlePredict.thisCharacterRenderPortrait = battlePredict.thisCharacterRenderPortraitOriginal;	//아군 이미지는 좌측 외각서부터 출발
}

void SelectionUI::SetBattlePredictOutwards()
{
	battlePredict.battleUIState = BattleUIState::IsMovingOutwards;
	battlePredict.enemyRenderPortrait = battlePredict.enemyRenderPortraitToMove;					//적 이미지는 100지점에서부터
	battlePredict.thisCharacterRenderPortrait = battlePredict.thisCharacterRenderPortraitToMove;	//아군 이미지는 500지점에서부터
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
		IMAGEMANAGER->FindImage("초상화" + battlePredict.thisCharacterPtr->GetName())->Render(battlePredict.thisCharacterRenderPortrait.x, battlePredict.thisCharacterRenderPortrait.y);
		IMAGEMANAGER->FindImage("초상화" + battlePredict.enemyPtr->GetName())->Render(battlePredict.enemyRenderPortrait.x, battlePredict.enemyRenderPortrait.y);

		break;
	default:
		assert(false && "보여주는 UI에 에러있음");
		break;
	}


	IMAGEMANAGER->FindImage("PhotoFrame")->SetSize({ 800,640 });
	IMAGEMANAGER->FindImage("PhotoFrame")->SetAlpha(photoFrameAlpha);
	IMAGEMANAGER->FindImage("PhotoFrame")->Render(0, 0);
}

