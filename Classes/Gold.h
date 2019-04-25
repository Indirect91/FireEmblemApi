#pragma once
#include "GameObject.h"

class Gold : public GameObject
{
private:
	UINT money; //���� ��

public:
	void Init() override {}; //�̴��Ұ� ����..
	void Release()override{}; //������ �Ұ� ����..
	void Update()override {}; //�����Ұ� ����..
	void Render()override {}; //�׸��� ����..

	//�嵷 ��������
	const UINT &GetMoney() const {return money; }
	//�嵷 �����ϱ�
	void SetMoney(UINT _money) { money = _money; }
	//����� ������� ���� ������
	UINT& RefMoney() { return money; }

	Gold() { money = 10000; }; //�ʱ�ݾ� �������� ���, ���� ���̺����Ͽ� ���� �������
	~Gold() {};
};

