#pragma once
#include "Lords.h"
class Player final : public Lords //�÷��̾� �� ��ӹ��� ���ϰ� ���̳� �ھƳ���
{
private:

	GameObject * playerGold = DATACENTRE.GetCertainObject(ObjType::Gold, "playerGold"); //���� ���� ���� �÷��̾ ����ִ´�
	std::map<std::string, GameObject*> &inven = DATACENTRE.GetObjects(ObjType::PlayerItem); //�κ��丮 ����� ���� ���� �÷��̾ ������ �ִ´�
	std::map<std::string, GameObject*> &playerTroop = DATACENTRE.GetObjects(ObjType::PlayerArmy); //�÷��̾� �δ�
	std::map<std::string, GameObject*> &battleTroop = DATACENTRE.GetObjects(ObjType::CurrentPlayerArmy); //�� �������� ����� �÷��̾� �δ�
public:

	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	Player();
	~Player() {};
};

