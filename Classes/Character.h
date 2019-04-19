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
	Occupation classes;			//모든 캐릭터는 직업이 있다
	Image * portaitImg;			//모든 캐릭터는 초상화 이미지를 가진다
	Image* frameImg;			//모든 캐릭터는 프레임 이미지를 가진다
	BOOL isActionTaken;			//모든 캐릭터는 턴당 행동을 가진다
	POINT frame;				//모든 캐릭터는 프레임을 가진다

	INT classHp;				//직업별 체력
	UINT classAttack;			//직업별 공격력
	UINT classDefence;			//직업별 방어력
	UINT classLuck;				//직업별 행운력. 회피율 및 크리티컬
	UINT classMove;				//직업별 이동력. 턴당 이동 가능한 타일 수

	INT baseHp;					//체력
	UINT baseAttack;			//공격력
	UINT baseDefence;			//방어력
	UINT baseLuck;				//행운력. 회피율 및 크리티컬
	UINT baseMove;				//이동력. 턴당 이동 가능한 타일 수

	INT AdditionalHp;			//버프, 디버프, 혹은 아이템의 영향을 받는 스텟
	UINT AdditionalAttack;		//버프, 디버프, 혹은 아이템의 영향을 받는 스텟
	UINT AdditionalDefence;		//버프, 디버프, 혹은 아이템의 영향을 받는 스텟
	UINT AdditionalLuck;		//버프, 디버프, 혹은 아이템의 영향을 받는 스텟
	INT AdditionalMove;			//버프, 디버프, 혹은 아이템의 영향을 받는 스텟

	class Item *item;			//보유중인 아이템
	BOOL mouseOn;				//마우스가 캐릭터 위에 있을떄

	BOOL isChecking;						//이동범위 눌려있었던지
	INT moveRangeCalculator;				//이동범위 계산용
	std::vector<class Tiles*> blueTiles;	//이동범위 계산용2


private:

	void ShowMoveRange();
	void DisableRange();
	void MakeItBlue(POINT _pos, UINT _move);
	  
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

