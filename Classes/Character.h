#pragma once
#include "GameObject.h"

//������
enum class Occupation : UINT
{
	//���� ��Ÿ� 1¥����
	Infantary = 0,
	Rogue,
	Knight,

	RangeSeparater, //��Ÿ� ���Ұ�. 
	
	//���� ��Ÿ� 2¥����
	Archer,
	Mage,
};

//��ĳ���� ����
enum class CharStatus
{
	Idle,
	IsCursorOn,
	IsClicked,
	IsMoving,
	IsAttacking,
	IsDragging,
	Disable,
};

//��ĳ���� �巡�� ����
enum class DraggingDirection
{
	LEFT = 2,RIGHT,DOWN,UP
};

class Character : public GameObject
{
private:

	//�����
	POINT draggingIndex;					//�巡�� ��ġ
	DraggingDirection draggindDirection;	//�巡�� ����
	CharStatus charStatus;					//ĳ���� ����
	Occupation occupation;						//����
	class Item *item;						//�������� ������
	Image* frameImg;						//������ �̹���
	BOOL isInCamera;						//ī�޶� �ȿ� �־�߸� ������Ʈ/������
	Cursor* tmpCursor;

	//���̹����� ����
	Image * portraitImg;		//�ʻ�ȭ �̹���
	FLOAT portraitAlpha;		//�ʻ�ȭ ���İ�
	BOOL isActionTaken;			//��� ĳ���ʹ� �ϴ� �ൿ�� ������
	UINT frameCounter;			//������Ʈ�� ������ ������
	INT frameInterval;			//������ ���� ��
	UINT frameLoop;				//������ �߰���ġ
	POINT frame;				//��� ĳ���ʹ� �������� ������

	//�彺���� ����1
	INT classDefence;			//������ ����
	INT classAttack;			//������ ���ݷ�
	UINT classLuck;				//������ ����. ȸ���� �� ũ��Ƽ��
	UINT classMove;				//������ �̵���. �ϴ� �̵� ������ Ÿ�� ��
	INT classHp;				//������ ü��

	//�彺���� ����2
	INT baseDefence;			//����
	INT baseAttack;				//���ݷ�
	UINT baseLuck;				//����. ȸ���� �� ũ��Ƽ��
	UINT baseMove;				//�̵���. �ϴ� �̵� ������ Ÿ�� ��
	INT baseHp;					//ü��

	//�彺���� ����3
	INT AdditionalDefence;		//����, �����, Ȥ�� �������� ������ �޴� ����
	INT AdditionalAttack;		//����, �����, Ȥ�� �������� ������ �޴� ����
	UINT AdditionalLuck;		//����, �����, Ȥ�� �������� ������ �޴� ����
	INT AdditionalMove;			//����, �����, Ȥ�� �������� ������ �޴� ����
	INT AdditionalHp;			//����, �����, Ȥ�� �������� ������ �޴� ����

	//���̵����� ����
	std::set<class Tiles*> AvailableTiles;	//�ൿ���� Ÿ�ϵ� ��Ƶ�
	std::set<class Tiles*> greenTiles;		//������ ǥ�õ� Ÿ�ϵ� ��Ƶ�
	std::set<class Tiles*> blueTiles;		//�̵����� ǥ�õ� Ÿ�ϵ� ��Ƶ�
	std::vector<class Tiles*> toMove;		//�̵� ȭ��ǥ ǥ�ÿ�
	std::set<class Tiles*> redTiles;		//���ݹ��� ǥ�õ� Ÿ�ϵ� ��Ƶ�
	INT RangeCalculator;					//�̵����� ����
	UINT dragValidity;						//�̵� ��������
	BOOL isCalculated;						//�̵����� ��� �ѹ� �ߴ��� üũ��

private:
	void MakeItBlue(POINT _pos, UINT _move);
	void DisableMoveRange();				//���� �ֺ� ���� ����
	BOOL CheckInCamera();					//ī�޶� �ӿ� ĳ���� �ִ��� üũ
	void ShowMoveRange();					//���� �ֺ� �̵����� ǥ��
	void AdjustFrame();
	  
public:
	void Release()override;
	void Render()override;
	void Update()override;
	void Init() override;

	void SetInitialChar(Occupation _job, POINT _frame, std::string _charName, POINT _index);
	void SetActionTaken(BOOL _actionTaken) { this->isActionTaken = _actionTaken; }
	void SetOccupation(Occupation _job);
	void SetFrame(POINT _frame) { this->frame = _frame; }
	void SetImg(std::string _CharName);

	void SetAdditionalAttack(UINT _attack) { this->AdditionalAttack = _attack; }
	void SetAdditionalDefence(UINT _defence) { this->AdditionalDefence = _defence; }
	void SetAdditionalLuck(UINT _luck) { this->AdditionalLuck = _luck; }
	void SetAdditionalMove(UINT _move) { this->AdditionalMove = _move; }

	const BOOL & GetIsActionTaken() const { return this->isActionTaken; }
	const Occupation &GetOccupation() const { return this->occupation; }
	const POINT &GetFrame() const { return this->frame; }
	const BOOL GetIsClicked() const { return charStatus == CharStatus::IsClicked; }


	Character();
	~Character() {};
};

