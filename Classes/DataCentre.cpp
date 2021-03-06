#include "../stdafx.h"
#include "DataCentre.h"
#include "GameObject.h"
#include "Character.h"
#include "Gold.h"
#include "Tiles.h"


DataCentre::DataCentre()
{
	//▼오브젝트의 종류만큼 오브젝트 컨테이너 만듦
	for (UINT i = 0; i < static_cast<UINT>(ObjType::ObjSize); i++)
	{
		objContainer.insert(std::make_pair((ObjType)i, std::map<std::string, GameObject*>()));
	}
	AddObj(ObjType::Gold, "playerGold", new Gold);
}

//▼오브젝트 컨테이너 속에 모든걸 이닛시킴
void DataCentre::Init()
{
	//개선된 auto문 사용. 참조자를 박아뒀기에 원본을 받아 사용
	for (auto &allObj : objContainer) //오브젝트 컨테이너를 전부 돈다. allObj는 현재 ObjCtr형식
	{
		for (auto &certainObj : allObj.second) //certainObj는 현재 map<std::string, class GameObject*> 형식
		{
			certainObj.second->Init(); //certainObj의 second는 gameObject포인터임. 그것들 이닛
		}
	}
}

//▼오브젝트 컨테이너 속에 모든걸 업뎃시킴
void DataCentre::Update()
{
	for (auto& allObj : objContainer) //오브젝트 컨테이너를 전부 돈다. allObj는 현재 ObjCtr형식
	{
		for (auto& certainObj : allObj.second) //certainObj는 현재 map<std::string, class GameObject*> 형식
		{
			certainObj.second->Update(); //certainObj의 second는 gameObject포인터임. 그것들 업뎃
		}
	}
}

//▼오브젝트 컨테이너 속에 모든걸 릴리즈시킴
void DataCentre::Release()
{
	//for (auto& allObj : objContainer) //오브젝트 컨테이너를 전부 돈다. allObj는 현재 ObjCtr형식
	//{
	//	for (auto& certainObj : allObj.second) //certainObj는 현재 map<std::string, class GameObject*> 형식
	//	{
	//		certainObj.second->Release(); //certainObj의 second는 gameObject포인터임. 그것들 릴리즈
	//		SAFE_DELETE(certainObj.second); //포인터는 안전하게 버리고 널처리
	//	}
	//	allObj.second.clear(); //방금 포인터들 정리한 백터 비워버림
	//}
}

//▼오브젝트 컨테이너 속에 모든걸 랜더 돌림
void DataCentre::Render()
{
	for (const auto& allObj : objContainer) //오브젝트 컨테이너를 전부 돈다. allObj는 현재 ObjCtr형식
	{
		if (allObj.first == ObjType::Tile)
		{
			for (const auto& certainObj : allObj.second) //certainObj는 현재 map<std::string, class GameObject*> 형식
			{
				dynamic_cast<Tiles*>(certainObj.second)->Render(); //certainObj의 second는 gameObject포인터임. 그것들 랜더
			}
		}
	}
}

//▼특정 오브젝트 컨테이너 클리어
void DataCentre::ClearObjects(ObjType _toClear)
{
	objContainer[_toClear].clear();
}

//▼오브젝트 추가
void DataCentre::AddObj(ObjType _type, std::string _name, GameObject* _obj)
{	//▼assert는 거짓조건이 들어오면 터지다는것과, 맵에는 키값당 하나만 존재한다는 사실을 이용한 안전검사
	assert(objContainer[_type].count(_name) < 1);
	objContainer[_type].insert(std::make_pair(_name, _obj)); //방금 받아온거 넣어줌
}

//▼오브젝트 제거
void DataCentre::RemoveObj(ObjType _type, std::string _name, GameObject* _obj)
{
	assert(objContainer[_type].count(_name) > 0); //삭제하고자 하는 대상이 존재해야만 하니,안전검사 일단 실행
	for (auto &toRemove : objContainer[_type]) //toRemove는 현재 주어진 타입의 std::map<std::string, GameObject*>));
	{
		if (toRemove.first == _name) //toRemove의 first는 내가 준 오브젝트타입의 컨테이너 돌면서 나올 키값들
		{
			toRemove.second->Release(); //toRemove의 second는 키값하고 일치하는 오브젝트니, 그거 릴리즈
			SAFE_DELETE(toRemove.second); //찾은거 삭제
			break; //지웠으니 이젠 포문 더 돌 필요 없음
		}
	}
}

//▼특정 오브젝트 가져오기
GameObject* DataCentre::GetCertainObject(ObjType _type, std::string _name)
{
	assert(objContainer[_type].count(_name) > 0 && "찾는거 없어용"); 
	return objContainer[_type][_name]; //타입,이름이 일치하는 오브젝트 반환
}

BOOL DataCentre::CheckObjectExistance(ObjType _type, std::string _name)
{
	//해당 컨테이너 뒤져서 이름이 있는가 세본다. 있으면 참 반환, 없으면 거짓
	return objContainer[_type].count(_name) > 0;

}

std::map<std::string, GameObject*>& DataCentre::RefObjects(ObjType _type)
{
	return objContainer[_type]; //타입이 일치하는 맵 반환
}

HRESULT DataCentre::SavetoFile()
{

	return S_OK;
}

HRESULT DataCentre::LoadFromFile()
{
	HANDLE file;
	DWORD read;

	EnemyData toLoadEnemy[100];

	file = CreateFile(L"EnemyFile.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	assert(ReadFile(file, toLoadEnemy, sizeof(EnemyData)*100, &read, NULL));
	CloseHandle(file);



	AddObj(ObjType::PlayerArmy, "Chrome", new Character(GameObject::Occupation::Swordsman, "Chrome", { 1,2 }, OwnedBy::Player));
	AddObj(ObjType::PlayerArmy, "Anna", new Character(GameObject::Occupation::Swordsman, "Anna", { 3,2 }, OwnedBy::Player));
	AddObj(ObjType::PlayerArmy, "Lucina2", new Character(GameObject::Occupation::Mage, "Lucina", { 5,3 }, OwnedBy::Player));
	AddObj(ObjType::PlayerArmy, "Virion", new Character(GameObject::Occupation::Archer, "Virion", { 4,4 }, OwnedBy::Player));

	//AddObj(ObjType::EnemyArmy, "Virion", new Character(GameObject::Occupation::Sniper, "Virion", { 1,6 }, OwnedBy::Enemy));
	//AddObj(ObjType::EnemyArmy, "Lucina3", new Character(GameObject::Occupation::GreatKnight, "Lucina", { 8,8 }, OwnedBy::Enemy));
	//AddObj(ObjType::EnemyArmy, "Olivia", new Character(GameObject::Occupation::Assassin, "Olivia", { 3,6 }, OwnedBy::Enemy));
	//AddObj(ObjType::EnemyArmy, "Lucina", new Character(GameObject::Occupation::Knight, "Lucina", { 7,9 }, OwnedBy::Enemy));

	for (UINT i = 0; i < 100; i++)
	{
		if (!(toLoadEnemy[i].occupation == 0 && PointCompare(toLoadEnemy[i].realPos, { 0,0 })))
		{
			int dupCounter = 0;
			for (auto& duplicate : objContainer[ObjType::EnemyArmy])
			{
				if (PointCompare(duplicate.second->GetIndex(), toLoadEnemy[i].realPos))
				{
					dupCounter++;
				}
			}
			if(dupCounter==0)
				AddObj(ObjType::EnemyArmy, "General" + std::to_string(i), new Character((GameObject::Occupation)toLoadEnemy[i].occupation,"General", toLoadEnemy[i].realPos, OwnedBy::Enemy));
		}
		else if (toLoadEnemy[i].occupation == 0 && PointCompare(toLoadEnemy[i].realPos, { 0,0 }))
		{
			continue;
		}
	}

	return S_OK;
}

