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
	POINT frame;		//��� ĳ���ʹ� �������� ������

	UINT attack;		//���ݷ�
	UINT defence;		//����
	UINT luck;		//����. ȸ���� �� ũ��Ƽ��
	UINT move;		//�̵���. �ϴ� �̵� ������ Ÿ�� ��

	UINT AdditionalAttack;		//���ݷ�
	UINT AdditionalDefence;		//����
	UINT AdditionalLuck;		//����. ȸ���� �� ũ��Ƽ��
	UINT AdditionalMove;		//�̵���. �ϴ� �̵� ������ Ÿ�� ��

	//Item item;
	  
public:
	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	void SetActionTaken(BOOL _actionTaken) { isActionTaken = _actionTaken; }
	void SetOccupation(Occupation _job);
	void SetFrame(POINT _frame);
	void SetImg(std::string _CharName);

	void SetAdditionalAttack(UINT _attack) { AdditionalAttack = _attack; }
	void SetAdditionalDefence(UINT _defence) { AdditionalDefence = _defence; }
	void SetAdditionalLuck(UINT _luck) { AdditionalLuck = _luck; }
	void SetAdditionalMove(UINT _move) { AdditionalMove = _move; }

	const BOOL &GetIsActionTaken() const { return isActionTaken; }
	const Occupation &GetOccupation() const { return classes; }
	const POINT &GetFrame() const { return frame; }


	Character();
	~Character() {};
};

