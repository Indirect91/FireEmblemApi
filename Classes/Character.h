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
	Occupation classes;			//��� ĳ���ʹ� ������ �ִ�
	Image * portaitImg;			//��� ĳ���ʹ� �ʻ�ȭ �̹����� ������
	Image* frameImg;			//��� ĳ���ʹ� ������ �̹����� ������
	BOOL isActionTaken;			//��� ĳ���ʹ� �ϴ� �ൿ�� ������
	POINT frame;				//��� ĳ���ʹ� �������� ������

	INT classHp;				//������ ü��
	UINT classAttack;			//������ ���ݷ�
	UINT classDefence;			//������ ����
	UINT classLuck;				//������ ����. ȸ���� �� ũ��Ƽ��
	UINT classMove;				//������ �̵���. �ϴ� �̵� ������ Ÿ�� ��

	UINT baseHp;				//ü��
	UINT baseAttack;			//���ݷ�
	UINT baseDefence;			//����
	UINT baseLuck;				//����. ȸ���� �� ũ��Ƽ��
	UINT baseMove;				//�̵���. �ϴ� �̵� ������ Ÿ�� ��

	UINT AdditionalHp;			//�߰� ü��
	UINT AdditionalAttack;		//�߰� ���ݷ�
	UINT AdditionalDefence;		//�߰� ����
	UINT AdditionalLuck;		//�߰� ����. ȸ���� �� ũ��Ƽ��
	UINT AdditionalMove;		//�߰� �̵���. �ϴ� �̵� ������ Ÿ�� ��

	class Item *item;					//�������� ������
	BOOL mouseOn;
	void ���(POINT _index);
	  
public:
	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

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

