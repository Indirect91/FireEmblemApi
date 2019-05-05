#pragma once
#include "GameObject.h"



class Character : public GameObject
{
public:
	//��ĳ���� ����
	enum class CharStatus : UINT
	{
		Idle,
		IsDead,
		IsDying,
		IsActed,
		Disable,
		IsMoving,
		IsClicked,
		IsDragging,
		IsCursorOn,
		IsAttacking,
		IsDeadTalking,
		IsCheckingOut,
	};

	//��ĳ���� ������
	struct CharData
	{
		CharData() { Weapon = WeaponType::Axe; }
		INT Move = 0;		//�̵���
		INT Range = 0;		//�ൿ ����
		INT Speed = 0;		//�ӵ�. ����� 5���̰� ���� �ι� �����Ѵ�
		INT Attack = 0;		//���ݷ�
		FLOAT Luck = 0;		//���, ���߷� �� ũ����
		INT Health = 0;		//ü��
		INT Defence = 0;	//����
		WeaponType Weapon;	//��������
	};

private:

	//�����
	class Item *item = nullptr;				//�������� ������
	CharStatus charStatus;					//ĳ���� ����
	Occupation occupation;					//����
	class Cursor* cursor;					//Ŀ���� �ʹ� Ŀ��
	std::string name;						//ĳ���͸�
	OwnedBy whosChar;						//������ ĳ��������
	
	Image* frameImg;						//������ �̹���
	BOOL isInCamera;						//ī�޶� �ȿ� �־�߸� ������Ʈ/������
	BOOL isInBattle;

	//���̹����� ����
	RECT healthBarBackground;	//��� ĳ���ʹ� ü�¹ٸ� ������
	FLOAT characterAlpha;		//ĳ���� ���İ�
	RECT healthBarFront;		//��� ĳ���ʹ� �� ü���� ������
	Image * portraitImg;		//�ʻ�ȭ �̹���
	FLOAT portraitAlpha;		//�ʻ�ȭ ���İ�
	//BOOL isActionTaken;			//��� ĳ���ʹ� �ϴ� �ൿ�� ������
	UINT frameCounter;			//������Ʈ�� ������ ������
	INT frameInterval;			//������ ���� ��
	UINT frameLoop;				//������ �߰���ġ
	POINT frame;				//��� ĳ���ʹ� �������� ������

	//�彺�ȵ������� ����
	CharData occupationData;
	CharData additionalData;
	CharData baseData;
	INT currentHealth;
	BOOL deadTalkInit = false;
	BOOL deadTalkEnd = false;

	//���̵����� ����
	std::set<class Tiles*> AvailableTiles;	//�ൿ���� Ÿ�ϵ� ��Ƶ�
	std::set<class Tiles*> purpleTiles;		//�� �ൿ����
	std::set<class Tiles*> greenTiles;		//������ ǥ�õ� Ÿ�ϵ� ��Ƶ�
	std::set<class Tiles*> blueTiles;		//�̵����� ǥ�õ� Ÿ�ϵ� ��Ƶ�
	std::set<class Tiles*> allyTiles;		//�Ʊ� ���� �����̳�
	std::set<class Tiles*> foeTiles;		//���� ���� �����̳�
	std::set<class Tiles*> redTiles;		//���ݹ��� ǥ�õ� Ÿ�ϵ� ��Ƶ�

	//��Ÿ�������
	BOOL isRangeCalculated;					//�̵����� ��� �ѹ� �ߴ��� üũ��
	BOOL isMoveCalculated;					//�̵����� ��� �ѹ� �ߴ��� üũ��
	INT RangeCalculator;					//�̵����� ����

	//��巡�� ����
	DraggingDirection draggindDirection;	//�巡�� ����
	std::vector<class Tiles*> toMove;		//�̵� ȭ��ǥ ǥ�ÿ�
	POINT moveStartLocation;				//�̵����� ��ġ
	POINT draggingIndexPrev;				//�巡�� ������ġ
	Tiles* draggingStarted;					//�巡�� ���� ���
	POINT draggingIndex;					//�巡�� ��ġ
	UINT dragValidity;						//�̵� ��������
	UINT moveSpeed;							//�̵� �ӵ�

	//���̵� ����
	MovingDirection movingDirection;

private:
	BOOL CheckInCamera();					//ī�޶� �ӿ� ĳ���� �ִ��� üũ
	void AdjustFrame();



public:
	void Release()override;
	void Render()override;
	void Update()override;
	void Init() override;

	void SetInitialChar(Occupation _job, std::string _charName, POINT _index, OwnedBy _whos);
	void SetFrame(POINT _frame) { this->frame = _frame; }
	void SetFrameAuto(Occupation _job, OwnedBy _whos);
	void SetImgAuto(std::string _CharName);
	void SetOccupation(Occupation _job);

	void SetAdditionalDefence(INT _defence) { this->additionalData.Defence = _defence; }
	void SetAdditionalAttack(INT _attack) { this->additionalData.Attack = _attack; }
	void SetAdditionalLuck(FLOAT _luck) { this->additionalData.Luck = _luck; }
	void SetAdditionalMove(INT _move) { this->additionalData.Move = _move; }

	void SetMovingDirection(MovingDirection _movingDirection) { movingDirection = _movingDirection; }
	void SetStatus(CharStatus _toChange) { this->charStatus = _toChange; }
	void SetDeadTalkEnd(BOOL _deadTalkEnd) { deadTalkEnd = _deadTalkEnd; }
	void SetCurrentHpSubtractByValue(INT _changedValue);
	void SetCurrentHpAddByValue(INT _changedValue);
	void SetCurrentHealth(INT _currentHealth);

	const BOOL GetIsClicked() const { return charStatus == CharStatus::IsClicked; }
	const MovingDirection & GetMovingDirection() const { return movingDirection; }
	const POINT& GetMoveStartLocation() const { return moveStartLocation; }
	const Occupation &GetOccupation() const { return this->occupation; }
	const CharStatus& GetStatus() const { return charStatus; }
	const std::string& GetName() const { return this->name; }
	const POINT &GetFrame() const { return this->frame; }

	//�彺�� ����������
	const INT GetDefence() const { return additionalData.Defence + occupationData.Defence + baseData.Defence; }
	const INT GetAttackRange() const {return additionalData.Range + occupationData.Range + baseData.Range; }
	const INT GetHealth() const { return additionalData.Health + occupationData.Health + baseData.Health; }
	const INT GetDamage() const { return additionalData.Attack + occupationData.Attack + baseData.Attack; }
	const INT GetMoveRange() const { return additionalData.Move + occupationData.Move + baseData.Move;}
	const INT GetSpeed() const { return additionalData.Speed + occupationData.Speed + baseData.Speed; }
	const FLOAT GetLuck() const { return additionalData.Luck + occupationData.Luck + baseData.Luck; }
	const WeaponType &GetWeaponType() const { return occupationData.Weapon; }
	const INT &GetCurrentHealth() const { return currentHealth; }
	const Item* GetItemPtr() const { return item; }

	//����������
	void SetIsInBattle(BOOL _isInBattle) { isInBattle = _isInBattle; }
	const BOOL GetIsInBattle()const { return isInBattle; }

	//�����ǥ�ÿ�û����
	void ShowMoveRange(void (Character::* colourShow)(INT));	//�Լ������ͷ� ��� �������� Ȯ������
	void BlueRedShow(INT _actionRange);
	void PurpleShow(INT _actionRange);
	void DisableActionRange();
	void DisableMoveRange();									//���� �ֺ� ���� ����
	void ShowActionRange();

	Character();
	Character(Occupation _job, std::string _charName, POINT _index, OwnedBy _whos);
	~Character() {};
};

