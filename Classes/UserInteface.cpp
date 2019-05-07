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
		showData.toShowName = toShow->GetName(); //�̸� �� ����������(rating) �����ص�
		showData.currentHealth = toShow->GetCurrentHealth();
		showData.maxHealth = toShow->GetHealth();
		showData.rating = toShow->GetDefence() + toShow->GetDamage() + toShow->GetHealth() + toShow->GetSpeed() + toShow->GetMoveRange();
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
	if (showData.toShowName != "")
	{
		IMAGEMANAGER->FindImage("�ʻ�ȭ" + showData.toShowName)->SetAlpha(imageAlpha);
		IMAGEMANAGER->FindImage("�ʻ�ȭ" + showData.toShowName)->Render(950, 150);
		IMAGEMANAGER->FindImage("StatusUI")->SetAlpha(imageAlpha);
		IMAGEMANAGER->FindImage("StatusUI")->Render(900, 350);
		
		
		D2DRENDERER->RenderText(1028, 372, StringToWString(showData.toShowName), RGB( 255,255,255 ), imageAlpha, 20,DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
		D2DRENDERER->RenderText(970, 445, std::to_wstring(showData.rating), RGB(255, 255, 255), imageAlpha, 30, DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
		D2DRENDERER->RenderText(1000, 445, std::to_wstring(showData.currentHealth) + L" / " + std::to_wstring(showData.maxHealth), RGB(0,0,0), imageAlpha, 32, DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	}
}