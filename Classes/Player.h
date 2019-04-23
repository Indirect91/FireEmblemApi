#pragma once
#include "Lords.h"
class Player final : public Lords //�÷��̾� �� ��ӹ��� ���ϰ� ���̳� �ھƳ���
{
private:

	GameObject * playerGold = DATACENTRE.GetCertainObject(ObjType::Gold, "playerGold"); //���� ���� ���� �÷��̾ ����ִ´�
	std::map<std::string, GameObject*> &inven = DATACENTRE.RefObjects(ObjType::PlayerItem); //�κ��丮 ����� ���� ���� �÷��̾ ������ �ִ´�
	std::map<std::string, GameObject*> &playerTroop = DATACENTRE.RefObjects(ObjType::PlayerArmy); //�÷��̾� �δ�
	std::map<std::string, GameObject*> &battleTroop = DATACENTRE.RefObjects(ObjType::CurrentPlayerArmy); //�� �������� ����� �÷��̾� �δ�

	RECT toMove = { 0,0,800,WINSIZEY };
public:

	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	Player();
	~Player() {};
};

