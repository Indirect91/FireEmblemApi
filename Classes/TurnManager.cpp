#include "../stdafx.h"
#include "TurnManager.h"
#include "Cursor.h"

TurnManager::TurnManager()
{
	changingStatus = TurnManager::ChangingStatus::toPlayer;
	moveSpeedFast = 25;
	moveSpeedMid = 15;
	moveSpeedSlow = 5;
}

void TurnManager::Init()
{
	//this->changeToPlayer();
	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
}

void TurnManager::Release()
{
}

void TurnManager::Revert()
{

}


void TurnManager::Update()
{
	switch (changingStatus)
	{
	case TurnManager::ChangingStatus::Idle:
		break;
	case TurnManager::ChangingStatus::toPlayer:

		switch (toPlayer.phase)
		{
		case 0 :
			SOUNDMANAGER->play("PlayerTurn");
			toPlayer.rings.x = -IMAGEMANAGER->FindImage("PlayerTurnRing")->GetSize().x;
			toPlayer.texts.x = -IMAGEMANAGER->FindImage("PlayerTurnText")->GetSize().x;
			toPlayer.back.x = -IMAGEMANAGER->FindImage("PlayerTurnBack")->GetSize().x*2;

			toPlayer.phase++;
			break;
		case 1 :
			toPlayer.rings.x += moveSpeedFast;

			if (toPlayer.rings.x >= 400)
			{
				toPlayer.phase++;
			}

			break;
		case 2:
			toPlayer.rings.x += moveSpeedFast;
			toPlayer.texts.x += moveSpeedFast;
			toPlayer.back.x += moveSpeedFast;

			if (toPlayer.texts.x >= 0)
			{
				toPlayer.phase++;
			}

			break;
		case 3:
			toPlayer.rings.x += moveSpeedFast;
			toPlayer.texts.x += moveSpeedSlow;
			toPlayer.back.x += moveSpeedMid;

			if (toPlayer.texts.x >= 250)
			{
				toPlayer.phase++;
			}

			break;
		case 4:
			toPlayer.rings.x += moveSpeedFast;
			toPlayer.texts.x += moveSpeedFast;
			toPlayer.back.x += moveSpeedMid;

			if (toPlayer.texts.x >= WINSIZEX+ IMAGEMANAGER->FindImage("PlayerTurnText")->GetSize().x / 2)
			{
				toPlayer.phase++;
			}

			break;
		case 5:
			cursor->SetCursorTurn(IngameStatus::PlayerTurn);
			cursor->SetCursorTurnPrev(IngameStatus::EnemyTurn);
			changingStatus = ChangingStatus::Idle;
			toPlayer.phase = 0;
			break;

		default:
			break;
		}

		break;
	case TurnManager::ChangingStatus::toEnemy:

		switch (toEnemy.phase)
		{
		case 0:
			SOUNDMANAGER->play("EnemyTurn");
			toEnemy.rings.x = 800 + IMAGEMANAGER->FindImage("EnemyTurnRing")->GetSize().x;
			toEnemy.texts.x = 800;// +IMAGEMANAGER->FindImage("EnemyTurnText")->GetSize().x;
			toEnemy.back.x = 800 + IMAGEMANAGER->FindImage("EnemyTurnBack")->GetSize().x;// *2;

			toEnemy.phase++;
			break;
		case 1:
			toEnemy.rings.x -= moveSpeedFast;

			if (toEnemy.rings.x <= 400)
			{
				toEnemy.phase++;
			}

			break;
		case 2:
			toEnemy.rings.x -= moveSpeedFast;
			toEnemy.texts.x -= moveSpeedFast;
			toEnemy.back.x -= moveSpeedFast;

			if (toEnemy.texts.x <= 400)
			{
				toEnemy.phase++;
			}
		case 3:
			toEnemy.rings.x -= moveSpeedFast;
			toEnemy.texts.x -= moveSpeedSlow;
			toEnemy.back.x -= moveSpeedMid;

			if (toEnemy.texts.x <= 0)
			{
				toEnemy.phase++;
			}

			break;
		case 4:
			toEnemy.rings.x -= moveSpeedFast;
			toEnemy.texts.x -= moveSpeedFast;
			toEnemy.back.x -= moveSpeedMid;

			if (toEnemy.texts.x <= 0 - IMAGEMANAGER->FindImage("EnemyTurnText")->GetSize().x-200)
			{
				toEnemy.phase++;
			}

			break;
		case 5:
			cursor->SetCursorTurn(IngameStatus::EnemyTurn);
			cursor->SetCursorTurnPrev(IngameStatus::PlayerTurn);
			changingStatus = ChangingStatus::Idle;
			toEnemy.phase = 0;
			break;

		default:
			break;
		}
	}
}



void TurnManager::changeToPlayer()
{
	toPlayer.phase = 0;
	changingStatus = ChangingStatus::toPlayer;

}

void TurnManager::changeToEnemy()
{
	toEnemy.phase = 0;
	changingStatus = ChangingStatus::toEnemy;
}

void TurnManager::Render()
{
	switch (changingStatus)
	{
	case TurnManager::ChangingStatus::Idle:
		break;
	case TurnManager::ChangingStatus::toPlayer:

		IMAGEMANAGER->FindImage("PlayerTurnBack")->Render(toPlayer.back.x, toPlayer.back.y);
		IMAGEMANAGER->FindImage("PlayerTurnText")->Render(toPlayer.texts.x, toPlayer.texts.y);
		for (UINT i = 0; i < 35; i++)
		{
			IMAGEMANAGER->FindImage("PlayerTurnRing")->Render(toPlayer.rings.x - i * 70, toPlayer.rings.y,Pivot::Centre);
			IMAGEMANAGER->FindImage("PlayerTurnRing")->Render(toPlayer.rings.x - i * 70, toPlayer.rings.y + 300, Pivot::Centre);
		}
		


		break;
	case TurnManager::ChangingStatus::toEnemy:
		IMAGEMANAGER->FindImage("EnemyTurnBack")->Render(toEnemy.back.x, toEnemy.back.y);
		IMAGEMANAGER->FindImage("EnemyTurnText")->Render(toEnemy.texts.x, toEnemy.texts.y);
		for (UINT i = 0; i < 35; i++)
		{
			IMAGEMANAGER->FindImage("EnemyTurnRing")->Render(toEnemy.rings.x + i * 70, toEnemy.rings.y, Pivot::Centre);
			IMAGEMANAGER->FindImage("EnemyTurnRing")->Render(toEnemy.rings.x + i * 70, toEnemy.rings.y + 300, Pivot::Centre);
		}



		break;
	default:
		break;
	}
}
