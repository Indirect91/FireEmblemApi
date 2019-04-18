#include "../stdafx.h"
#include "DataCentre.h"
#include "GameObject.h"
#include "Character.h"
#include "Gold.h"

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
		for (const auto& certainObj : allObj.second) //certainObj�� ���� map<std::string, class GameObject*> ����
		{
			certainObj.second->Render(); //certainObj�� second�� gameObject��������. �װ͵� ����
		}
	}
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

GameObject* DataCentre::GetCertainObject(ObjType _type, std::string _name)
{
	assert(objContainer[_type].count(_name) > 0); //�����ϰ��� �ϴ� ����� �����ؾ߸� �ϴ�,�����˻� �ϴ� ����
	return objContainer[_type][_name]; //Ÿ��,�̸��� ��ġ�ϴ� ������Ʈ ��ȯ
}

std::map<std::string, GameObject*>& DataCentre::GetObjects(ObjType _type)
{
	return objContainer[_type]; //Ÿ���� ��ġ�ϴ� �� ��ȯ
}

BOOL DataCentre::SavetoFile()
{

	return false;
}

BOOL DataCentre::LoadFromFile()
{
	std::string stringImgTest; //�ӽ� ��Ʈ��



	stringImgTest = "Chrome"; //�ű⿡ ũ�� ����
	GameObject* addTest = new Character; //�ӽ� ��ü ����
	
	dynamic_cast<Character*> (addTest)->SetImg(stringImgTest); //ĳ���� ���� �׽�Ʈ
	dynamic_cast<Character*> (addTest)->SetPosition(RectMake(48 * 6, 48 * 6, 48, 48));
	AddObj(ObjType::PlayerArmy, stringImgTest , addTest); //��� ������� ������ �߰���


	
	return false;
}

