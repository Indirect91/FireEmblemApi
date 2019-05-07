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
	//���� ������ ���Ѱ� ����
}

void UserInteface::Update()
{
	toShow = nullptr; //�� ������Ʈ �����Ҷ� �ϴ��� �������� ����

	//���ϴ� �÷��̾� ������� ã�ƺ�
	for (auto& toFind : DATACENTRE.RefObjects(ObjType::PlayerArmy))
	{
		if (PointCompare(cursor->GetIndex(), toFind.second->GetIndex()))
		{
			toShow = dynamic_cast<Character*>(toFind.second);
		}
	}

	//��� ������ �� �δ븦 ������
	for (auto& toFind : DATACENTRE.RefObjects(ObjType::EnemyArmy))
	{
		if (PointCompare(cursor->GetIndex(), toFind.second->GetIndex()))
		{
			toShow = dynamic_cast<Character*>(toFind.second);
		}
	}

	//�幺���� ã�Ƴ±⿡ �������Ͱ� �ƴϰ� �Ǿ����� ���İ��� �÷��� ������ ���̰� ��
	if (toShow!=nullptr)
	{
		toShowName = toShow->GetName();
		imageAlpha += 0.1f;
		if (imageAlpha >= 1) imageAlpha = 1;

	}
	//����������϶� �ٽ� �ʻ�ȭ�� �Ⱥ��̰� �ؼ� �ٴ����� �ǵ���
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
		IMAGEMANAGER->FindImage("�ʻ�ȭ" + toShowName)->SetAlpha(imageAlpha);
		IMAGEMANAGER->FindImage("�ʻ�ȭ" + toShowName)->Render(950, 180);
	}
}