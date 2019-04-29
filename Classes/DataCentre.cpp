#include "../stdafx.h"
#include "DataCentre.h"
#include "GameObject.h"
#include "Character.h"
#include "Gold.h"
#include "Tiles.h"


DataCentre::DataCentre()
{
	//�������Ʈ�� ������ŭ ������Ʈ �����̳� ����
	for (UINT i = 0; i < static_cast<UINT>(ObjType::ObjSize); i++)
	{
		objContainer.insert(std::make_pair((ObjType)i, std::map<std::string, GameObject*>()));
	}
	AddObj(ObjType::Gold, "playerGold", new Gold);
}

//�������Ʈ �����̳� �ӿ� ���� �̴ֽ�Ŵ
void DataCentre::Init()
{
	//������ auto�� ���. �����ڸ� �ھƵױ⿡ ������ �޾� ���
	for (auto &allObj : objContainer) //������Ʈ �����̳ʸ� ���� ����. allObj�� ���� ObjCtr����
	{
		for (auto &certainObj : allObj.second) //certainObj�� ���� map<std::string, class GameObject*> ����
		{
			certainObj.second->Init(); //certainObj�� second�� gameObject��������. �װ͵� �̴�
		}
	}
}

//�������Ʈ �����̳� �ӿ� ���� ������Ŵ
void DataCentre::Update()
{
	for (auto& allObj : objContainer) //������Ʈ �����̳ʸ� ���� ����. allObj�� ���� ObjCtr����
	{
		for (auto& certainObj : allObj.second) //certainObj�� ���� map<std::string, class GameObject*> ����
		{
			certainObj.second->Update(); //certainObj�� second�� gameObject��������. �װ͵� ����
		}
	}
}

//�������Ʈ �����̳� �ӿ� ���� �������Ŵ
void DataCentre::Release()
{
	for (auto& allObj : objContainer) //������Ʈ �����̳ʸ� ���� ����. allObj�� ���� ObjCtr����
	{
		for (auto& certainObj : allObj.second) //certainObj�� ���� map<std::string, class GameObject*> ����
		{
			certainObj.second->Release(); //certainObj�� second�� gameObject��������. �װ͵� ������
			SAFE_DELETE(certainObj.second); //�����ʹ� �����ϰ� ������ ��ó��
		}
		allObj.second.clear(); //��� �����͵� ������ ���� �������
	}
}

//�������Ʈ �����̳� �ӿ� ���� ���� ����
void DataCentre::Render()
{
	for (const auto& allObj : objContainer) //������Ʈ �����̳ʸ� ���� ����. allObj�� ���� ObjCtr����
	{
		if (allObj.first == ObjType::Tile)
		{
			for (const auto& certainObj : allObj.second) //certainObj�� ���� map<std::string, class GameObject*> ����
			{
				dynamic_cast<Tiles*>(certainObj.second)->Render(); //certainObj�� second�� gameObject��������. �װ͵� ����
			}
		}
	}
}

//��Ư�� ������Ʈ �����̳� Ŭ����
void DataCentre::ClearObjects(ObjType _toClear)
{
	objContainer[_toClear].clear();
}

//�������Ʈ �߰�
void DataCentre::AddObj(ObjType _type, std::string _name, GameObject* _obj)
{	//��assert�� ���������� ������ �����ٴ°Ͱ�, �ʿ��� Ű���� �ϳ��� �����Ѵٴ� ����� �̿��� �����˻�
	assert(objContainer[_type].count(_name) < 1);
	objContainer[_type].insert(std::make_pair(_name, _obj)); //��� �޾ƿ°� �־���
}

//�������Ʈ ����
void DataCentre::RemoveObj(ObjType _type, std::string _name, GameObject* _obj)
{
	assert(objContainer[_type].count(_name) > 0); //�����ϰ��� �ϴ� ����� �����ؾ߸� �ϴ�,�����˻� �ϴ� ����
	for (auto &toRemove : objContainer[_type]) //toRemove�� ���� �־��� Ÿ���� std::map<std::string, GameObject*>));
	{
		if (toRemove.first == _name) //toRemove�� first�� ���� �� ������ƮŸ���� �����̳� ���鼭 ���� Ű����
		{
			toRemove.second->Release(); //toRemove�� second�� Ű���ϰ� ��ġ�ϴ� ������Ʈ��, �װ� ������
			SAFE_DELETE(toRemove.second); //ã���� ����
			break; //�������� ���� ���� �� �� �ʿ� ����
		}
	}
}

//��Ư�� ������Ʈ ��������
GameObject* DataCentre::GetCertainObject(ObjType _type, std::string _name)
{
	assert(objContainer[_type].count(_name) > 0 && "searched object not found"); 
	return objContainer[_type][_name]; //Ÿ��,�̸��� ��ġ�ϴ� ������Ʈ ��ȯ
}

BOOL DataCentre::CheckObjectExistance(ObjType _type, std::string _name)
{
	//�ش� �����̳� ������ �̸��� �ִ°� ������. ������ �� ��ȯ, ������ ����
	return objContainer[_type].count(_name) > 0;
}

std::map<std::string, GameObject*>& DataCentre::RefObjects(ObjType _type)
{
	return objContainer[_type]; //Ÿ���� ��ġ�ϴ� �� ��ȯ
}

HRESULT DataCentre::SavetoFile()
{

	return S_OK;
}

HRESULT DataCentre::LoadFromFile()
{
	AddObj(ObjType::PlayerArmy, "Chrome", new Character(Occupation::Swordsman, "Chrome", { 5,2 }, Character::OwnedBy::Player));
	AddObj(ObjType::PlayerArmy, "Anna", new Character(Occupation::Swordsman, "Anna", { 3,4 }, Character::OwnedBy::Player));

	AddObj(ObjType::EnemyArmy, "Olivia", new Character(Occupation::Assassin, "Olivia", { 6,6 }, Character::OwnedBy::Enemy));
	AddObj(ObjType::EnemyArmy, "Lucina", new Character(Occupation::Sniper, "Lucina", { 8,8 }, Character::OwnedBy::Enemy));

	return S_OK;
}

