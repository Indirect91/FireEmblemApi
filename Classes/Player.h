#pragma once
#include "Lords.h"
class Player final : public Lords //�÷��̾� �� ��ӹ��� ���ϰ� ���̳� �ھƳ���
{
private:

	GameObject* playerGold; //���� ���� ���� �÷��̾ ����ִ´�
	std::map<std::string, GameObject*> &inven = DATACENTRE.GetObjects(ObjType::PlayerItem); //�κ��丮 ����� ���� ���� �÷��̾ ������ �ִ´�
	std::map<std::string, GameObject*> &playerTroop = DATACENTRE.GetObjects(ObjType::PlayerArmy); //�÷��̾� �δ�
public:

	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	Player();
	~Player() {};
};

