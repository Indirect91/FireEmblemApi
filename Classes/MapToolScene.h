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

	struct EnemyData
	{
		INT occupation;
		POINT realPos;
	};

	struct MapToolCursor
	{
		std::string terrain;//Ÿ���� ����ִ� ����
		POINT terrainFrame;	//Ÿ������ ���� �����ӷ�����

		std::string objectT; //Ÿ���� ����ִ� ������Ʈ
		POINT objTFrame;		//Ÿ������ ������Ʈ �����ӷ�����

		std::string movingT;//Ÿ���� ����ִ� �����̴� Ÿ��
		POINT movingtFrame;	//�����̴� Ÿ�� �����ӷ�����

		std::string EnemyName = "";
		INT occupation = 0;
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

	enum class Occupation : UINT
	{
		Assassin = 0,		//�ܰ�				--						���	A
		Swordsman = 10,		//���				--						���	A
		Myrmidon = 32,		//�ذ�				����:SwordMaster			�߱�	B				
		Mercenary = 42,		//����+��			����:Hero				�߱�	B
		SwordMaster = 52,	//�ְ�				--						���	A
		Archer = 62,		//���� �ü�			����:Sniper				�ϱ�	C
		Sniper = 72,		//ūȰ �ü�			����:BowNight			�߱�	B
		GreatLord = 82,		//����				--						�ֻ�	S
		Knight = 92,		//���尩				����:General(�̱���)		�߱�	B
		WarCleric = 102,	//�ٹ��Ͽ� ����		--						���	A
		Hero = 114,			//ū���� + ��			--						���	A
		Sage = 124,			//�κ�				--						���	A
		Mage = 136,			//���� + �κ�			����:Sage				�߱�	B
		Cleric = 146,		//�Ϲ� ����			����:War Cleric			�߱�	B
		Grandmaster = 158,	//���������			--						���	A
		Tactician = 168,	//������				����:GrandMaster			�߱�	B
		BowKnight = 178,	//��ź �ü�			--						���	A
		DarkKnight = 188,	//�� ���			--						���	A
		GreatKnight = 198,	//���� ���			--						���	A
		Cavalier = 208,		//â ���			����:Paladin				�߱�	B
		Paladin = 218,		//â ��� ����		--						���	A
		Troubadour = 228,	//�⸶����			����:Valkyrie			�߱�	B
		Valkyrie = 240,		//�⸶��������		--						���	A
		Dancer = 252,		//����				����						�ֻ�	S
		DarkMage = 263,		//���渶��			����:Sorcerer			�߱�	B
		Sorcerer = 273,		//���渶������		--						���	A
		Theif = 283,		//����				����:Assassin			�ϱ�	C
	};

private:
	float moveWindowRateX;	  //������ �̵���ų ��ġ
	float moveWindowRateAccX; //������ �̵���ų ��ġ ����
	BOOL isMovingDone;		  //������ �̵� ��������
	void moveWindowFrame();	  //������ ������ �Լ�. �ܺμ� ȣ���� �� �����״� �����̺�
	UINT paperScrollX;		  //������ �̵��� ���� ������ ��ũ��

	float moveAngle;
	class Tiles *field;					//Ÿ�ϵ� ������ ������
	Tiles toSave[TILECOLX * TILEROWY];
	EnemyData toSaveEnemy[100];
	std::vector<EnemyData> EnemiesV;
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
	INT enemyX = 0;


public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	MapToolScene();
	~MapToolScene() {};
};

