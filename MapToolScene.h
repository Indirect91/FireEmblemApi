#pragma once
#include "GameNode.h"

class MapToolScene : public GameNode
{

private:
	float moveWindowRateX;	  //������ �̵���ų ��ġ
	float moveWindowRateAccX; //������ �̵���ų ��ġ ����
	BOOL isMovingDone;		  //������ �̵� ��������
	void moveWindowFrame();	  //������ ������ �Լ�. �ܺμ� ȣ���� �� �����״� �����̺�
	UINT paperScrollX;		  //������ �̵��� ���� ������ ��ũ��

	UINT fieldX_Column = 30;				//Ÿ�ϵ� ���� ����
	UINT fieldY_Row = 30;					//Ÿ�ϵ� ���� ����
	UINT tileSize = 32;						//Ÿ�� ���μ���
	//std::vector<class Tiles> vecField;    //
	class Tiles *arrField;					//Ÿ�ϵ� ������ �迭. ���忡 ���
	RECT showArea;							//Ÿ�ϵ� ������ ����(ī�޶�)
	RECT tempC;
	void moveCamera();

public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	MapToolScene() {};
	~MapToolScene() {};
};

