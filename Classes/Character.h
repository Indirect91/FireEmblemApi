#pragma once
#include "GameObject.h"

//▼직업
enum class Occupation
{
	Infantary,
	Knight,
	Mage,
	Rogue,
};

//▼캐릭터 상태
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

//▼캐릭터 드래깅 방향
enum class DraggingDirection
{
	LEFT = 2,RIGHT,DOWN,UP
};

class Character : public GameObject
{
private:

	//▼통상
	POINT draggingIndex;					//드래깅 위치
	DraggingDirection draggindDirection;	//드래깅 방향
	CharStatus charStatus;					//캐릭터 상태
	Occupation classes;						//직업
	class Item *item;						//보유중인 아이템
	Image* frameImg;						//프레임 이미지
	BOOL isInCamera;						//카메라 안에 있어야만 업데이트/랜더됨
	Cursor* tmpCursor;

	//▼이미지쪽 변수
	Image * portraitImg;		//초상화 이미지
	FLOAT portraitAlpha;		//초상화 알파값
	BOOL isActionTaken;			//모든 캐릭터는 턴당 행동을 가진다
	UINT frameCounter;			//업데이트별 프레임 제어자
	INT frameInterval;			//프레임 도는 빈도
	UINT frameLoop;				//프레임 추가어치
	POINT frame;				//모든 캐릭터는 프레임을 가진다

	//▼스탯쪽 변수1
	INT classDefence;			//직업별 방어력
	INT classAttack;			//직업별 공격력
	UINT classLuck;				//직업별 행운력. 회피율 및 크리티컬
	UINT classMove;				//직업별 이동력. 턴당 이동 가능한 타일 수
	INT classHp;				//직업별 체력

	//▼스탯쪽 변수2
	INT baseDefence;			//방어력
	INT baseAttack;				//공격력
	UINT baseLuck;				//행운력. 회피율 및 크리티컬
	UINT baseMove;				//이동력. 턴당 이동 가능한 타일 수
	INT baseHp;					//체력

	//▼스탯쪽 변수3
	INT AdditionalDefence;		//버프, 디버프, 혹은 아이템의 영향을 받는 스텟
	INT AdditionalAttack;		//버프, 디버프, 혹은 아이템의 영향을 받는 스텟
	UINT AdditionalLuck;		//버프, 디버프, 혹은 아이템의 영향을 받는 스텟
	INT AdditionalMove;			//버프, 디버프, 혹은 아이템의 영향을 받는 스텟
	INT AdditionalHp;			//버프, 디버프, 혹은 아이템의 영향을 받는 스텟

	//▼커서쪽 변수
	std::set<class Tiles*> blueTiles;		//이동범위 표시된 타일들 모아둠
	std::set<class Tiles*> toMove;			//이동 화살표 표시용
	INT moveRangeCalculator;				//이동범위 계산용
	BOOL isCalculated;						//이동범위 계산 한번 했는지 체크용

private:
	void MakeItBlue(POINT _pos, UINT _move);
	void DisableMoveRange();				//본인 주변 범위 끄기
	BOOL CheckInCamera();					//카메라 속에 캐릭이 있는지 체크
	void ShowMoveRange();					//본인 주변 이동범위 표시
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
	const Occupation &GetOccupation() const { return this->classes; }
	const POINT &GetFrame() const { return this->frame; }
	const BOOL GetIsClicked() const { return charStatus == CharStatus::IsClicked; }


	Character();
	~Character() {};
};

