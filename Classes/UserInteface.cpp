#include "../stdafx.h"
#include "UserInteface.h"
#include "Cursor.h"
#include "Character.h"

UserInteface::UserInteface()
{
	toShow = nullptr;
}

void UserInteface::Init()
{
	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
}

void UserInteface::Release()
{
	//현재 씬에서 뉴한거 없음
}

void UserInteface::Update()
{
	toShow = nullptr; //매 업데이트 시작할때 일단은 널포인터 박음

	//▼일단 플레이어 군대부터 찾아봄
	for (auto& toFind : DATACENTRE.RefObjects(ObjType::PlayerArmy))
	{
		if (PointCompare(cursor->GetIndex(), toFind.second->GetIndex()))
		{
			toShow = dynamic_cast<Character*>(toFind.second);
		}
	}

	//▼그 다음은 적 부대를 뒤져봄
	for (auto& toFind : DATACENTRE.RefObjects(ObjType::EnemyArmy))
	{
		if (PointCompare(cursor->GetIndex(), toFind.second->GetIndex()))
		{
			toShow = dynamic_cast<Character*>(toFind.second);
		}
	}

	//▼뭔가를 찾아냈기에 널포인터가 아니게 되었을때 알파값을 올려서 서서히 보이게 함
	if (toShow!=nullptr)
	{
		toShowName = toShow->GetName();
		imageAlpha += 0.1f;
		if (imageAlpha >= 1) imageAlpha = 1;

	}
	//▼널포인터일때 다시 초상화값 안보이게 해서 바닥으로 되돌림
	else
	{
		imageAlpha -= 0.1f;
		if (imageAlpha <= 0) imageAlpha = 0;
	}	
}

void UserInteface::Render()
{
	IMAGEMANAGER->FindImage("BattleUIBg")->Render(0, 0);
	if (toShowName != "")
	{
		IMAGEMANAGER->FindImage("초상화" + toShowName)->SetAlpha(imageAlpha);
		IMAGEMANAGER->FindImage("초상화" + toShowName)->Render(950, 180);
	}
}