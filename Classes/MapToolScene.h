#pragma once
#include "GameNode.h"

class MapToolScene : public GameNode
{
private:
	struct moveDir //����Ű ��������
	{
		POINT pos;
		BOOL pressed;
	};
private:
	float moveWindowRateX;	  //������ �̵���ų ��ġ
	float moveWindowRateAccX; //������ �̵���ų ��ġ ����
	BOOL isMovingDone;		  //������ �̵� ��������
	void moveWindowFrame();	  //������ ������ �Լ�. �ܺμ� ȣ���� �� �����״� �����̺�
	UINT paperScrollX;		  //������ �̵��� ���� ������ ��ũ��

	//std::vector<class Tiles> vecField;    //
	float moveAngle;
	class Tiles *field;					//Ÿ�ϵ� ������ �迭. ���忡 ���
	RECT tempC;								//�浹�� ����� �ӽ� ��Ʈ �ʱ�ȭ
	void moveCamera();
	RECT reLocate(RECT _rc);
	RECT palletes[9];
	
	moveDir RightLeft;
	moveDir UpDown;

public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	MapToolScene() {};
	~MapToolScene() {};
};

