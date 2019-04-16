#pragma once
#include "GameObject.h"
class Character : public GameObject
{
private:
	Image * portaitImg; //��� ĳ���ʹ� �ʻ�ȭ �̹����� ������
	Image* frameImg;	//��� ĳ���ʹ� ������ �̹����� ������
	BOOL isActionTaken;	//��� ĳ���ʹ� �ϴ� �ൿ�� ������

	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;
	  
public:
	Character();
	~Character() {};
};

