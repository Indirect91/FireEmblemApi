#pragma once
#include "GameObject.h"
class Player final : public GameObject //�÷��̾� �� ��ӹ��� ���ϰ� ���̳� �ھƳ���
{
private:

	//�÷��̾ �����Ѵٸ� �־�� �� ��?
	
	//�÷��̾ �������� ĳ���� ���
	//�÷��̾� �κ��丮
	//���� ���
	//���丮 �����Ȳ

public:

	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	Player() {};
	~Player() {};
};

