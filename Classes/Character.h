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
	Occupation classes; //모든 캐릭터는 직업이 있다
	Image * portaitImg; //모든 캐릭터는 초상화 이미지를 가진다
	Image* frameImg;	//모든 캐릭터는 프레임 이미지를 가진다
	BOOL isActionTaken;	//모든 캐릭터는 턴당 행동을 가진다
	POINT frame;		//모든 캐릭터는 프레임을 가진다

	UINT attack;		//공격력
	UINT defence;		//방어력
	UINT luck;		//행운력. 회피율 및 크리티컬
	UINT move;		//이동력. 턴당 이동 가능한 타일 수

	UINT AdditionalAttack;		//공격력
	UINT AdditionalDefence;		//방어력
	UINT AdditionalLuck;		//행운력. 회피율 및 크리티컬
	UINT AdditionalMove;		//이동력. 턴당 이동 가능한 타일 수

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

