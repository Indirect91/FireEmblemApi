#include "../stdafx.h"
#include "BattleScenes.h"
#include "Player.h"
#include "Enemy.h"
#include "Character.h"
#include "Cursor.h"
#include "TurnManager.h"

BattleScenes::BattleScenes()
{
	previousState = IngameStatus::StartPlacement;
	currentState = IngameStatus::PlayerTurn;

	startPlacement = nullptr;
	battleManager = nullptr;
	tileManager = nullptr;
	selectionUI = nullptr;
	turnManager = nullptr;
	ingameUI = nullptr;
	escMenu = nullptr;
	cursor = nullptr;
	player = nullptr;
	enemy = nullptr;
}

void BattleScenes::ESCManage()
{
	if (KEYMANAGER->IsOnceKeyDown(VK_ESCAPE))
	{
		if (currentState != IngameStatus::ESCMenu) //���� ESC �޴��� ���� �ִ� ���°� �ƴϾ��ٸ�
		{
			previousState = currentState;
			currentState = IngameStatus::ESCMenu;
		}
		else //ESC�� �ƴϾ��� ��� ���´� �� ���ǿ� �ɸ���, else�� ESC�޴��� �����ְ� �ִ� ��Ȳ
		{
			currentState = previousState; //�õ� �귯���� ��� ���� ������Ʈ �ٽ� ����
		}
	}
}

void BattleScenes::checkDeadUnit()
{

	for (auto playerItr = DATACENTRE.RefObjects(ObjType::PlayerArmy).begin(); playerItr != DATACENTRE.RefObjects(ObjType::PlayerArmy).end();)
	{
		if (dynamic_cast<Character*> ((*playerItr).second)->GetStatus() == Character::CharStatus::IsDead)
		{
			(*playerItr).second->Release();
			SAFE_DELETE((*playerItr).second);
			playerItr = DATACENTRE.RefObjects(ObjType::PlayerArmy).erase(playerItr);
		}
		else
		{
			++playerItr;
		}
	}

	for (auto enemyItr = DATACENTRE.RefObjects(ObjType::EnemyArmy).begin(); enemyItr != DATACENTRE.RefObjects(ObjType::EnemyArmy).end();)
	{
		if (dynamic_cast<Character*> ((*enemyItr).second)->GetStatus() == Character::CharStatus::IsDead)
		{
			if (player->RefEnemyRangeCalculatedPrev() == (*enemyItr).first) { player->RefEnemyRangeCalculatedPrev() = ""; }
			(*enemyItr).second->Release();
			SAFE_DELETE((*enemyItr).second);
			enemyItr = DATACENTRE.RefObjects(ObjType::EnemyArmy).erase(enemyItr);
		}
		else
		{
			++enemyItr;
		}
	}
}

void BattleScenes::checkTurnEnd()
{
	std::map<std::string, GameObject*>& playerTroop = DATACENTRE.RefObjects(ObjType::PlayerArmy); //�÷��̾� �δ�
	std::map<std::string, GameObject*>& enemyTroop = DATACENTRE.RefObjects(ObjType::EnemyArmy); //�� �δ�
	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));

	INT counter = 0;

	if (cursor->GetCursorTurn() == IngameStatus::PlayerTurn)
	{
		for (auto& playerEachUnit : playerTroop)
		{
			if (dynamic_cast<Character*>(playerEachUnit.second)->GetStatus() == Character::CharStatus::IsActed)
			{
				counter++;
			}
		}
		if (counter == playerTroop.size())
		{
			dynamic_cast<TurnManager*>(DATACENTRE.GetCertainObject(ObjType::UI, "TurnManager"))->changeToEnemy();
			cursor->SetCursorTurn(IngameStatus::TurnChanging);
			for (auto& playerEachUnit : playerTroop)
			{
				dynamic_cast<Character*>(playerEachUnit.second)->SetStatus(Character::CharStatus::Idle);
			}
		}
	}
	else if (cursor->GetCursorTurn() == IngameStatus::EnemyTurn)
	{
		for (auto& enemyEachUnit : enemyTroop)
		{
			if (dynamic_cast<Character*>(enemyEachUnit.second)->GetStatus() == Character::CharStatus::IsActed)
			{
				counter++;
			}
		}
		if (counter == enemyTroop.size())
		{
			dynamic_cast<TurnManager*>(DATACENTRE.GetCertainObject(ObjType::UI, "TurnManager"))->changeToPlayer();
			cursor->SetCursorTurn(IngameStatus::TurnChanging);
			cursor->SetIsOtherUnitOn("", OwnedBy::Nobody);
			for (auto& enemyEachUnit : enemyTroop)
			{
				dynamic_cast<Character*>(enemyEachUnit.second)->SetStatus(Character::CharStatus::Idle);
			}
		}
	}
}

