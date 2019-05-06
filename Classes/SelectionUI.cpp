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
	DeadDialogs.clear();
	DeadDialogs.emplace_back(L"����.. ������ �� ������ �ִµ�..");
	DeadDialogs.emplace_back(L"�� �׾ ��������..�ٸ� ����� �ȵſ�..");
	DeadDialogs.emplace_back(L"����..��.. �Ⱥ�����.. ���ݸ� ���Կ�..");
	DeadDialogs.emplace_back(L"���ư���..��ȥ�ϰڴٴ� ���..�̾���..");
	DeadDialogs.emplace_back(L"��������ŭ��..����;��µ�..������ ����..");
	DeadDialogs.emplace_back(L"��..�̹��� �� ��������..������?..");
	DeadDialogs.emplace_back(L"�˼�������..������..�����Ű��ƿ�..");
	DeadDialogs.emplace_back(L"�̹���..��..���� ������..");
	DeadDialogs.emplace_back(L"�̹�..����..��..�����̾��µ�..");
	DeadDialogs.emplace_back(L"������..�η���..�ʴٴ°�..���� �������̾�");
	DeadDialogs.emplace_back(L"����.. ���ϼ�����..��� ������..");
	DeadDialogs.emplace_back(L"�ָ�..����Ʈ ����� �־��µ�..");
	DeadDialogs.emplace_back(L"���Ⱑ..��..������ ������..���̱���..");
	//DeadDialogs.emplace_back(L"��..ũ��..�..");
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
	if (photoFrameAlpha < 1.f) photoFrameAlpha += 0.1f;




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
			{
				battlePredict.battleUIState = BattleUIState::IsMovingOutwards;
			}

			//��A�� ������ ���� Ȯ������
			else if (KEYMANAGER->IsOnceKeyDown('A'))
			{
				battlePredict.battleUIState = BattleUIState::IsBattleReady;
			}

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
				SOUNDMANAGER->play("�ΰ���BGM");

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
	{
		std::wstring charName = L"";
		charName.assign(charDead.deadCharacter->GetName().begin(), charDead.deadCharacter->GetName().end());

		IMAGEMANAGER->FindImage("���Ǯ������" + charDead.deadCharacter->GetName())->SetAlpha(photoFrameAlpha);
		IMAGEMANAGER->FindImage("���Ǯ������" + charDead.deadCharacter->GetName())->SetScale(0.6f);
		IMAGEMANAGER->FindImage("���Ǯ������" + charDead.deadCharacter->GetName())->Render(charDead.deadPortraitPosition.x, charDead.deadPortraitPosition.y);
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
		assert(false && "�����ִ� UI�� ��������");
		break;
	}


	IMAGEMANAGER->FindImage("PhotoFrame")->SetSize({ 800,640 });
	IMAGEMANAGER->FindImage("PhotoFrame")->SetAlpha(photoFrameAlpha);
	IMAGEMANAGER->FindImage("PhotoFrame")->Render(0, 0);
}

