#include "../stdafx.h"
#include "Player.h"
#include "Character.h"
#include "Cursor.h"
#include "Gold.h"

//▼일단 비워서 출고
Player::Player()
{
	playerGold = nullptr;
	cursor = nullptr;
	enemyRangeDetector = false;
	enemyRangeCalculated = "";
	enemyRangeCalculatedPrev = "";
}

//▼이닛시 데이터센터와의 연결점을 만든다
void Player::Init()
{
	//플레이어의 부대,아이템,골드는 데이터센터서 가져와 넣는다
	playerGold = dynamic_cast<Gold*>(DATACENTRE.GetCertainObject(ObjType::Gold, "playerGold"));
	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
	for (auto& characters : playerTroop) { dynamic_cast<Character*>(characters.second)->Init(); }
	enemyRangeDetector = false;
	enemyRangeCalculated = "";
	enemyRangeCalculatedPrev = "";
}

//▼릴리즈. 뉴할당 받은거 여기선 없다
void Player::Release()
{
	//플레이어에서 들고있는 부대,아이템은 외부 데이터를 참조자로 들고있기에 뉴할당을 하지 않았음
}

//▼업데이트
void Player::Update()
{
	//▼플레이어의 업데이트가 돌때 현재 커서를 점령중인 캐릭터가 존재할때 키 제어권을 해당 캐릭터에게 넘김
	if (cursor->GetCursorOccupied() != "")
	{
		//TODO:추후 큐런트플레이어아미로 대체 필요
		dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::PlayerArmy, cursor->GetCursorOccupied()))->Update();

	}
	else
	{
		for (auto& character : playerTroop) //추후 배틀트룹으로 바꿀것
		{
			if (dynamic_cast<Character*>(character.second)->GetIsClicked())
			{
				cursor->SetCursorOccupied(character.first);
				break;
			}
			dynamic_cast<Character*>(character.second)->Update();
		}
	}
	//▼토글키인 M이 눌려있는지 여부를 커서에게 알림
	if (KEYMANAGER->IsToggleKey('M')) 
	{
		cursor->SetCursorStatus(Cursor::CursorState::cursorActivated);
	}
	else
	{
		cursor->SetCursorStatus(Cursor::CursorState::cursorDeactivated);
	}

	//▼키값에 따른 커서 이동. 통상
	if (KEYMANAGER->IsOnceKeyDown(VK_RIGHT))
	{
		cursor->MoveRight();
	}
	else if (KEYMANAGER->IsOnceKeyDown(VK_LEFT))
	{
		cursor->MoveLeft();
	}
	if (KEYMANAGER->IsOnceKeyDown(VK_UP))
	{
		cursor->MoveUp();
	}
	else if (KEYMANAGER->IsOnceKeyDown(VK_DOWN))
	{
		cursor->MoveDown();
	}

	if (KEYMANAGER->IsOnceKeyDown('D'))
	{
		if (!enemyRangeDetector)
		{
			for (auto& eachEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
			{
				dynamic_cast<Character*>(eachEnemy.second)->ShowActionRange();
			}
			enemyRangeDetector = true;
		}
		else
		{
			enemyRangeDetector = false;
			for (auto& eachEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
			{
				dynamic_cast<Character*>(eachEnemy.second)->DisableActionRange();
			}
		}

	}

	enemyRangeCalculated = "";
	for (auto& enemyEncounter : DATACENTRE.RefObjects(ObjType::EnemyArmy))
	{
		if (PointCompare(cursor->GetIndex(), enemyEncounter.second->GetIndex()))
		{
			if (enemyRangeCalculated == "")
			{
				enemyRangeCalculated = enemyEncounter.first;
				enemyRangeCalculatedPrev = enemyRangeCalculated;
				break;
			}
		}
	}
	
	if (enemyRangeCalculated != "")
	{
		if (cursor->GetCursorOccupied() == "")
		{
			dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::EnemyArmy, enemyRangeCalculated))->Update();
		}
	}
	else if (enemyRangeCalculated=="" && enemyRangeCalculatedPrev!="")
	{
		dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::EnemyArmy, enemyRangeCalculatedPrev))->DisableMoveRange();
		enemyRangeCalculatedPrev = "";
	}


	CAMERA.Follow(cursor->GetPosition());
}

void Player::Render()
{
	for (auto &character : playerTroop)
	{
		dynamic_cast<Character*>(character.second)->Render();
	}
}

