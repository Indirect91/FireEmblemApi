#pragma once
#include "GameNode.h"
#include "Tiles.h"

class MapToolScene : public GameNode
{
private:
	struct moveDir //����Ű ��������
	{
		moveDir() { pos = { 0 }; pressed = false; }
		POINT pos;
		BOOL pressed;
	};

	struct MapToolCursor
	{
		std::string terrain;//Ÿ���� ����ִ� ����
		POINT terrainFrame;	//Ÿ������ ���� �����ӷ�����

		std::string objectT; //Ÿ���� ����ִ� ������Ʈ
		POINT objTFrame;		//Ÿ������ ������Ʈ �����ӷ�����

		std::string movingT;//Ÿ���� ����ִ� �����̴� Ÿ��
		POINT movingtFrame;	//�����̴� Ÿ�� �����ӷ�����

		INT occupation;
		std::string name = "General";
		POINT enemyPos;

	};

	enum class WhichSheet
	{
		MovingTile,
		rock,
		grass,
		object,
		Enemy,
	};

private:
	float moveWindowRateX;	  //������ �̵���ų ��ġ
	float moveWindowRateAccX; //������ �̵���ų ��ġ ����
	BOOL isMovingDone;		  //������ �̵� ��������
	void moveWindowFrame();	  //������ ������ �Լ�. �ܺμ� ȣ���� �� �����״� �����̺�
	UINT paperScrollX;		  //������ �̵��� ���� ������ ��ũ��

	//std::vector<class Tiles> vecField;    //
	float moveAngle;
	class Tiles *field;					//Ÿ�ϵ� ������ ������
	Tiles toSave[TILECOLX * TILEROWY];
	void moveCamera();
	RECT reLocate(RECT _rc);
	
	moveDir RightLeft;
	moveDir UpDown;




	MapToolCursor maptoolCursor;
	WhichSheet whichSheet;
	std::vector<std::wstring> sheetV;
	RECT exitRc = {};
	RECT changePalletPrev = {1228,167, 1320,207};
	RECT changePalletNext = { 1336,167, 1428,207};
	std::vector<RECT> pallet;

	RECT saveButton = { 1220,610,1460,680 };
	RECT exitButton = {1220,720,1460,780};
	RECT selectArea = { 156,138, 970, 828};
	INT movingTilex = 0;
	INT counter = 0;

public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	MapToolScene();
	~MapToolScene() {};
};

