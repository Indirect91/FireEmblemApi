#pragma once
#include "GameObject.h"

//������
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

//��ĳ���� �巡�� ����
enum class DraggingDirection : UINT
{
	LEFT = 2,RIGHT,DOWN,UP
};

class Character : public GameObject
{
public:
	//��ĳ���� ����
	enum class CharStatus : UINT
	{
		Idle,
		IsActed,
		IsCursorOn,
		IsClicked,
		IsMoving,
		IsAttacking,
		IsDragging,
		IsCheckingOut,
		Disable,
	};

	//��ĳ���� ������
	struct CharData
	{
		INT Range = 0;		//�ൿ ����
		INT Attack = 0;		//���ݷ�
		INT Defence = 0;	//����
		INT Health = 0;		//ü��
		FLOAT Luck = 0;		//���, ���߷� �� ũ����
		INT Move = 0;		//�̵���
	};

	//�������
	enum class OwnedBy
	{
		Enemy = 0,
		Player = 4,
		Ally = 8,
	};

private:

	//�����
	OwnedBy whosChar;
	CharStatus charStatus;					//ĳ���� ����
	Occupation occupation;					//����
	class Item *item = nullptr;				//�������� ������
	Image* frameImg;						//������ �̹���
	BOOL isInCamera;						//ī�޶� �ȿ� �־�߸� ������Ʈ/������
	class Cursor* cursor;

	//���̹����� ����
	Image * portraitImg;		//�ʻ�ȭ �̹���
	FLOAT portraitAlpha;		//�ʻ�ȭ ���İ�
	BOOL isActionTaken;			//��� ĳ���ʹ� �ϴ� �ൿ�� ������
	UINT frameCounter;			//������Ʈ�� ������ ������
	INT frameInterval;			//������ ���� ��
	UINT frameLoop;				//������ �߰���ġ
	POINT frame;				//��� ĳ���ʹ� �������� ������

	//�彺�ȵ������� ����
	CharData occupationData;
	CharData additionalData;
	CharData baseData;

	//���̵����� ����
	std::set<class Tiles*> AvailableTiles;	//�ൿ���� Ÿ�ϵ� ��Ƶ�
	std::set<class Tiles*> purpleTiles;		//�� �ൿ����
	std::set<class Tiles*> greenTiles;		//������ ǥ�õ� Ÿ�ϵ� ��Ƶ�
	std::set<class Tiles*> blueTiles;		//�̵����� ǥ�õ� Ÿ�ϵ� ��Ƶ�
	std::set<class Tiles*> allyTiles;		//�Ʊ� ���� �����̳�
	std::set<class Tiles*> foeTiles;		//���� ���� �����̳�
	std::set<class Tiles*> redTiles;		//���ݹ��� ǥ�õ� Ÿ�ϵ� ��Ƶ�

	//��Ÿ�������
	BOOL isRangeCalculated;						//�̵����� ��� �ѹ� �ߴ��� üũ��
	BOOL isMoveCalculated;						//�̵����� ��� �ѹ� �ߴ��� üũ��
	INT RangeCalculator;					//�̵����� ����

	//��巡�� ����
	DraggingDirection draggindDirection;	//�巡�� ����
	std::set<class Tiles*> toMove;		//�̵� ȭ��ǥ ǥ�ÿ�
	POINT draggingIndex;					//�巡�� ��ġ
	UINT dragValidity;						//�̵� ��������

private:
	BOOL CheckInCamera();										//ī�޶� �ӿ� ĳ���� �ִ��� üũ
	void AdjustFrame();



public:
	void Release()override;
	void Render()override;
	void Update()override;
	void Init() override;

	void SetInitialChar(Occupation _job, std::string _charName, POINT _index, OwnedBy _whos);
	void SetActionTaken(BOOL _actionTaken) { this->isActionTaken = _actionTaken; }
	void SetOccupation(Occupation _job);
	void SetFrame(POINT _frame) { this->frame = _frame; }
	void SetFrameAuto(Occupation _job, OwnedBy _whos);
	void SetImg(std::string _CharName);

	void SetAdditionalAttack(INT _attack) { this->additionalData.Attack = _attack; }
	void SetAdditionalDefence(INT _defence) { this->additionalData.Defence = _defence; }
	void SetAdditionalLuck(FLOAT _luck) { this->additionalData.Luck = _luck; }
	void SetAdditionalMove(INT _move) { this->additionalData.Move = _move; }

	const BOOL & GetIsActionTaken() const { return this->isActionTaken; }
	const Occupation &GetOccupation() const { return this->occupation; }
	const POINT &GetFrame() const { return this->frame; }
	const BOOL GetIsClicked() const { return charStatus == CharStatus::IsClicked; }
	const CharStatus& GetStatus() const { return charStatus; }

	//�����ǥ�ÿ�û����
	void ShowMoveRange(void (Character::* colourShow)(INT));	//�Լ������ͷ� ��� �������� Ȯ������
	void DisableMoveRange();									//���� �ֺ� ���� ����
	void ShowActionRange();
	void DisableActionRange();
	void BlueRedShow(INT _actionRange);
	void PurpleShow(INT _actionRange);

	Character();
	Character(Occupation _job, std::string _charName, POINT _index, OwnedBy _whos);
	~Character() {};
};

