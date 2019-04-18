#pragma once
#include "GameObject.h"

class Gold : public GameObject
{
private:
	UINT money; //���� ��

public:
	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	//�嵷 ��������
	const UINT &GetMoney() const {return money; }
	//�嵷 �����ϱ�
	void SetMoney(UINT _money) { money = _money; }
	//����� ������� ���� ������
	UINT& RefMoney() { return money; }

	Gold() { money = 10000; }; //�ʱ�ݾ� �������� ���, ���� ���� ����
	~Gold() {};
};

