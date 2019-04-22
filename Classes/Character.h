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
	Occupation classes;			//����
	Image* frameImg;			//������ �̹���
	Image * portraitImg;		//�ʻ�ȭ �̹���
	FLOAT portraitAlpha;		//�ʻ�ȭ ���İ�

	BOOL isActionTaken;			//��� ĳ���ʹ� �ϴ� �ൿ�� ������
	POINT frame;				//��� ĳ���ʹ� �������� ������
	UINT frameLoop;				//������ �߰���ġ
	UINT frameCounter;			//������Ʈ�� ������ ������
	INT frameInterval;			//������ ���� ��

	INT classHp;				//������ ü��
	UINT classAttack;			//������ ���ݷ�
	UINT classDefence;			//������ ����
	UINT classLuck;				//������ ����. ȸ���� �� ũ��Ƽ��
	UINT classMove;				//������ �̵���. �ϴ� �̵� ������ Ÿ�� ��

	INT baseHp;					//ü��
	UINT baseAttack;			//���ݷ�
	UINT baseDefence;			//����
	UINT baseLuck;				//����. ȸ���� �� ũ��Ƽ��
	UINT baseMove;				//�̵���. �ϴ� �̵� ������ Ÿ�� ��

	INT AdditionalHp;			//����, �����, Ȥ�� �������� ������ �޴� ����
	UINT AdditionalAttack;		//����, �����, Ȥ�� �������� ������ �޴� ����
	UINT AdditionalDefence;		//����, �����, Ȥ�� �������� ������ �޴� ����
	UINT AdditionalLuck;		//����, �����, Ȥ�� �������� ������ �޴� ����
	INT AdditionalMove;			//����, �����, Ȥ�� �������� ������ �޴� ����

	class Item *item;			//�������� ������
	BOOL isClicked;				//���콺�� ĳ���͸� ���� �����϶�
	BOOL isMouseOn;							//���콺�� �ö��������
	BOOL isChecking;						//�̵����� �����־�����
	BOOL isCalculated;						//�̵����� ��� �ѹ� �ߴ��� üũ��
	INT moveRangeCalculator;				//�̵����� ����
	std::vector<class Tiles*> blueTiles;	//�̵����� ����2


private:

	void ShowMoveRange();
	void DisableMoveRange();
	void MakeItBlue(POINT _pos, UINT _move);
	void AdjustFrame();
	  
public:
	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	void SetInitialChar(Occupation _job, POINT _frame, std::string _charName, POINT _index);
	void SetActionTaken(BOOL _actionTaken) { this->isActionTaken = _actionTaken; }
	void SetOccupation(Occupation _job);
	void SetFrame(POINT _frame) { this->frame = _frame; }
	void SetImg(std::string _CharName);

	void SetAdditionalAttack(UINT _attack) { this->AdditionalAttack = _attack; }
	void SetAdditionalDefence(UINT _defence) { this->AdditionalDefence = _defence; }
	void SetAdditionalLuck(UINT _luck) { this->AdditionalLuck = _luck; }
	void SetAdditionalMove(UINT _move) { this->AdditionalMove = _move; }

	const BOOL &GetIsActionTaken() const { return this->isActionTaken; }
	const Occupation &GetOccupation() const { return this->classes; }
	const POINT &GetFrame() const { return this->frame; }


	Character();
	~Character() {};
};

