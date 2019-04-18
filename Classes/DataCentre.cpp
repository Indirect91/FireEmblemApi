#include "../stdafx.h"
#include "DataCentre.h"
#include "GameObject.h"
#include "Character.h"
#include "Gold.h"

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
	for (auto& allObj : objContainer) //오브젝트 컨테이너를 전부 돈다. allObj는 현재 ObjCtr형식
	{
		for (auto& certainObj : allObj.second) //certainObj는 현재 map<std::string, class GameObject*> 형식
		{
			certainObj.second->Release(); //certainObj의 second는 gameObject포인터임. 그것들 릴리즈
			SAFE_DELETE(certainObj.second); //포인터는 안전하게 버리고 널처리
		}
		allObj.second.clear(); //방금 포인터들 정리한 백터 비워버림
	}
}

//▼오브젝트 컨테이너 속에 모든걸 랜더 돌림
void DataCentre::Render()
{
	for (const auto& allObj : objContainer) //오브젝트 컨테이너를 전부 돈다. allObj는 현재 ObjCtr형식
	{
		for (const auto& certainObj : allObj.second) //certainObj는 현재 map<std::string, class GameObject*> 형식
		{
			certainObj.second->Render(); //certainObj의 second는 gameObject포인터임. 그것들 랜더
		}
	}
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

GameObject* DataCentre::GetCertainObject(ObjType _type, std::string _name)
{
	assert(objContainer[_type].count(_name) > 0); //삭제하고자 하는 대상이 존재해야만 하니,안전검사 일단 실행
	return objContainer[_type][_name]; //타입,이름이 일치하는 오브젝트 반환
}

std::map<std::string, GameObject*>& DataCentre::GetObjects(ObjType _type)
{
	return objContainer[_type]; //타입이 일치하는 맵 반환
}

BOOL DataCentre::SavetoFile()
{

	return false;
}

BOOL DataCentre::LoadFromFile()
{
	std::string stringImgTest; //임시 스트링



	stringImgTest = "Chrome"; //거기에 크롬 대입
	GameObject* addTest = new Character; //임시 객체 생성
	
	dynamic_cast<Character*> (addTest)->SetImg(stringImgTest); //캐릭터 생성 테스트
	dynamic_cast<Character*> (addTest)->SetPosition(RectMake(48 * 6, 48 * 6, 48, 48));
	AddObj(ObjType::PlayerArmy, stringImgTest , addTest); //방금 만들어진 정보를 추가함


	
	return false;
}

