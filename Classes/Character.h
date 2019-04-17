#pragma once
#include "GameObject.h"

enum class Occupation
{
	Infantary,
	Knight,
	mage,
	Rogue,

};

class Character : public GameObject
{
private:
	Occupation classes; //��� ĳ���ʹ� ������ �ִ�
	Image * portaitImg; //��� ĳ���ʹ� �ʻ�ȭ �̹����� ������
	Image* frameImg;	//��� ĳ���ʹ� ������ �̹����� ������
	BOOL isActionTaken;	//��� ĳ���ʹ� �ϴ� �ൿ�� ������
	UINT frameX;		//������ �̹����� X
	UINT frameY;		//������ �̹����� Y

	UINT attack;		//���ݷ�
	UINT defence;		//����
	UINT luck;			//����. ȸ���� �� ũ��Ƽ��
	UINT move;			//�̵���. �ϴ� �̵� ������ Ÿ�� ��

	  
public:
	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	Character();
	~Character() {};
};

