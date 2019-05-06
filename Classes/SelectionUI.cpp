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
	battlePredict.enemyRender.RenderPortrait = { 0 };									//적 초상화 그릴 위치
	battlePredict.enemyRender.RenderPortraitOriginal = { 758,100 };						//적 초상화 원위치
	battlePredict.enemyRender.RenderPortraitToMove = { 500,100 };						//적 초상화 이동하고자 하는 위치
	battlePredict.enemyRender.RenderName = { 0 };										//적 이름 그릴 위치
	battlePredict.enemyRender.RenderHealthPredict = { 0 };								//적 체력 예상 그릴 위치
	battlePredict.enemyRender.RenderHitRatePredict = { 0 };								//적 명중 예상 그릴 위치
	battlePredict.enemyRender.RenderWeaponAdvantage = { 0 };							//적 무기 상성 그릴 위치

	battlePredict.thisCharRender.charPtr = nullptr;										//현재 캐릭터 포인터
	battlePredict.thisCharRender.RenderPortrait = { 0 };								//현재 캐릭터 초상화 그릴 위치
	battlePredict.thisCharRender.RenderPortraitOriginal = { -158,100 };					//현재 캐릭터 원위치
	battlePredict.thisCharRender.RenderPortraitToMove = { 100,100 };					//현재 캐릭터 이동하고자 하는 위치
	battlePredict.thisCharRender.RenderName = { 0 };									//현재 캐릭터 이름 그릴 위치
	battlePredict.thisCharRender.RenderHealthPredict = { 0 };							//현재 캐릭터 체력 예상 그릴 위치
	battlePredict.thisCharRender.RenderHitRatePredict = { 0 };							//현재 캐릭터 명중 예상 그릴 위치
	battlePredict.thisCharRender.RenderWeaponAdvantage = { 0 };							//현재 캐릭터 무기 상성 그릴 위치
}

void SelectionUI::Init()
{
	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
	assert(cursor != nullptr); //커서 제대로 안들어왔으면 터뜨림
	DeadDialogs.clear();
	DeadDialogs.emplace_back(L"집에.. 돌봐야 할 가족이 있는데..");
	DeadDialogs.emplace_back(L"전 죽어도 괜찮지만..다른 사람은 안돼요..");
	DeadDialogs.emplace_back(L"앞이..잘.. 안보여요.. 조금만 쉴게요..");
	DeadDialogs.emplace_back(L"돌아가면..결혼하겠다는 약속..미안해..");
	DeadDialogs.emplace_back(L"죽을때만큼은..웃고싶었는데..눈물이 나네..");
	DeadDialogs.emplace_back(L"저..이번엔 안 도망가고..잘했죠?..");
	DeadDialogs.emplace_back(L"죄송하지만..집에는..못갈거같아요..");
	DeadDialogs.emplace_back(L"이번건..좀..많이 아프네..");
	DeadDialogs.emplace_back(L"이번..전투..후..전역이었는데..");
	DeadDialogs.emplace_back(L"죽음이..두렵지..않다는건..역시 거짓말이야");
	DeadDialogs.emplace_back(L"딸의.. 생일선물을..대신 전해줘..");
	DeadDialogs.emplace_back(L"주말..데이트 약속이 있었는데..");
	DeadDialogs.emplace_back(L"여기가..제..여정이 끝나는..곳이군요..");
	//DeadDialogs.emplace_back(L"가..크롬..어서..");
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
	if (photoFrameAlpha < 1.f) photoFrameAlpha += 0.1f;




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
			dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::PlayerArmy, cursor->GetCursorOccupied()))->SetStatus(Character::CharStatus::IsActed);

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

			//▼초상화 안으로 이동하라고 지시 들어올경우
		case SelectionUI::BattleUIState::IsMovingInwards:

			//▼사진 안으로 이동하라는 지시 들어왔을시
			if (photoFrameAlpha < 1.f) photoFrameAlpha += 0.2f;
			//▼플레이어캐릭터 이미지가 아직 이미지 보여주는 지점에 도달하지 못하였다면
			if (!PointCompare(battlePredict.thisCharRender.RenderPortrait, battlePredict.thisCharRender.RenderPortraitToMove))
			{
				battlePredict.thisCharRender.RenderPortrait.x += 43;
			}
			//▼적 캐릭터 이미지가 아직 이미지 보여주는 지점에 도달하지 못하였다면
			if (!PointCompare(battlePredict.enemyRender.RenderPortrait, battlePredict.enemyRender.RenderPortraitToMove))
			{
				battlePredict.enemyRender.RenderPortrait.x -= 43;
			}
			//▼플레이어캐릭터, 적 캐릭터 모두 정위치에 도달하였다면
			if (battlePredict.thisCharRender.RenderPortrait.x == battlePredict.thisCharRender.RenderPortraitToMove.x && battlePredict.enemyRender.RenderPortrait.x == battlePredict.enemyRender.RenderPortraitToMove.x)
			{
				battlePredict.battleUIState = BattleUIState::IsWaitingForInput;
			}
			break;

			//▼전투 예측이 끝나고 전투 하라고 최종 확인이 떨어졌을때
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

			//▼예측화면서 취소 누르면 초기화
			if (KEYMANAGER->IsOnceKeyDown('S'))
			{
				battlePredict.battleUIState = BattleUIState::IsMovingOutwards;
			}

			//▼A를 누르면 공격 확정지음
			else if (KEYMANAGER->IsOnceKeyDown('A'))
			{
				battlePredict.battleUIState = BattleUIState::IsBattleReady;
			}

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
	case SelectionUI::ToShow::AllyDead:
	{
		if (charDead.phase == 0)
		{
			charDead.randomText = DeadDialogs[rand() % DeadDialogs.size()];
			charDead.phase++;
		}
		else if (charDead.phase == 1)
		{
			if (photoFrameAlpha < 1.f)
			{
				photoFrameAlpha += 0.1f;
			}
			else
			{
				if (charDead.randomTextCounter < charDead.randomText.size())
				{
					charDead.randomTextInterval++;
					if (charDead.randomTextInterval % 5 == 0)
					{
						charDead.randomTextCounter++;
					}
				}
				charDead.toShow = charDead.randomText.substr(0, charDead.randomTextCounter);


				if (KEYMANAGER->IsOnceKeyDown('A') || KEYMANAGER->IsOnceKeyDown('S'))
				{
					SOUNDMANAGER->play("SelectionSmall");
					charDead.phase++;
				}
			}
		}
		else if (charDead.phase == 2)
		{
			if (photoFrameAlpha > 0.f)
			{
				photoFrameAlpha -= 0.2f;
			}
			else
			{
				toShow = ToShow::Idle;
				cursor->SetCursorOccupied("");
				cursor->SetCursorTurn(cursor->GetCursorTurnPrev());
				cursor->SetCursorTurnPrev(IngameStatus::SelectionUI);
				charDead.deadCharacter->SetDeadTalkEnd(true);
				SOUNDMANAGER->pause("DeadBGM");
				SOUNDMANAGER->play("인게임BGM");

				charDead.deadCharacter = nullptr;
				charDead.randomText = L"";
				charDead.toShow = L"";
				charDead.randomTextCounter = 0;
				charDead.randomTextInterval = 0;
				charDead.phase = 0;
			}
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
	battlePredict.enemyRender.RenderPortrait = battlePredict.enemyRender.RenderPortraitOriginal;					//적은 우측 외각서부터 출발
	battlePredict.thisCharRender.RenderPortrait = battlePredict.thisCharRender.RenderPortraitOriginal;	//아군 이미지는 좌측 외각서부터 출발
}

void SelectionUI::SetBattlePredictOutwards()
{
	battlePredict.battleUIState = BattleUIState::IsMovingOutwards;
	battlePredict.enemyRender.RenderPortrait = battlePredict.enemyRender.RenderPortraitToMove;					//적 이미지는 100지점에서부터
	battlePredict.thisCharRender.RenderPortrait = battlePredict.thisCharRender.RenderPortraitToMove;	//아군 이미지는 500지점에서부터
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
		IMAGEMANAGER->FindImage("초상화" + battlePredict.thisCharRender.charPtr->GetName())->Render(battlePredict.thisCharRender.RenderPortrait.x, battlePredict.thisCharRender.RenderPortrait.y);
		IMAGEMANAGER->FindImage("초상화" + battlePredict.enemyRender.charPtr->GetName())->Render(battlePredict.enemyRender.RenderPortrait.x, battlePredict.enemyRender.RenderPortrait.y);

		break;
	case SelectionUI::ToShow::AllyDead:
	{
		std::wstring charName = L"";
		charName.assign(charDead.deadCharacter->GetName().begin(), charDead.deadCharacter->GetName().end());

		IMAGEMANAGER->FindImage("사망풀사이즈" + charDead.deadCharacter->GetName())->SetAlpha(photoFrameAlpha);
		IMAGEMANAGER->FindImage("사망풀사이즈" + charDead.deadCharacter->GetName())->SetScale(0.6f);
		IMAGEMANAGER->FindImage("사망풀사이즈" + charDead.deadCharacter->GetName())->Render(charDead.deadPortraitPosition.x, charDead.deadPortraitPosition.y);
		IMAGEMANAGER->FindImage("DialogBox")->SetAlpha(photoFrameAlpha);
		IMAGEMANAGER->FindImage("DialogBox")->SetScale(1.35);
		IMAGEMANAGER->FindImage("DialogBox")->Render(charDead.deadPortraitPosition.x + 50, charDead.deadPortraitPosition.y + 450);
		D2DRENDERER->RenderText(charDead.deadPortraitPosition.x + 110, charDead.deadPortraitPosition.y + 448, charName, 30, D2DRenderer::DefaultBrush::White,DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
		D2DRENDERER->RenderText(charDead.deadPortraitPosition.x + 70, charDead.deadPortraitPosition.y + 500, charDead.toShow, 25, D2DRenderer::DefaultBrush::White);


		break;
	}
	case SelectionUI::ToShow::Idle:
	{
		break;
	}
	default:
		assert(false && "보여주는 UI에 에러있음");
		break;
	}


	IMAGEMANAGER->FindImage("PhotoFrame")->SetSize({ 800,640 });
	IMAGEMANAGER->FindImage("PhotoFrame")->SetAlpha(photoFrameAlpha);
	IMAGEMANAGER->FindImage("PhotoFrame")->Render(0, 0);
}

