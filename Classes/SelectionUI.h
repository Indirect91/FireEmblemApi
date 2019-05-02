#pragma once
#include "GameObject.h"
class SelectionUI : public GameObject
{
public:
	//▼어느 상호작용을 요구하는지
	enum class ToShow
	{
		Idle,

		SelectionBox,
		BattlePredict,
		HealPredict,

	};

	enum class BattleUIState
	{
		Idle,

		IsMovingOutwards,
		IsMovingInwards,
		IsBattleReady,
		IsWaitingForInput,
	};

	//▼힐 예측화면일시
	struct HealPredict
	{
		class Character* thisCharacter = nullptr;
		class Character* allyCharacter = nullptr;
		POINT thisCharacterRenderPosition = { 0 };
		POINT allyRenderPosition = { 0 };
	};

	//▼전투 예측화면일시
	struct BattlePredict
	{
		//▼통상
		//BOOL isMovingOutwards = false;
		//BOOL isMovingInwards = false;
		//BOOL isBattleReady = false;
		BattleUIState battleUIState;

		//▼적관련
		class Character* enemyPtr = nullptr;								//적 캐릭터 포인터
		POINT enemyRenderPortrait = { 0 };									//적 초상화 그릴 위치
		POINT enemyRenderPortraitOriginal = { 758,100 };					//적 초상화 원위치
		POINT enemyRenderPortraitToMove = { 500,100 };						//적 초상화 이동하고자 하는 위치
		POINT enemyRenderName = { 0 };										//적 이름 그릴 위치
		POINT enemyRenderHealthPredict = { 0 };								//적 체력 예상 그릴 위치
		POINT enemyRenderHitRatePredict = { 0 };							//적 명중 예상 그릴 위치
		POINT enemyRenderWeaponAdvantage = { 0 };							//적 무기 상성 그릴 위치

		//▼공격한 캐릭터 관련
		class Character* thisCharacterPtr = nullptr;						//현재 캐릭터 포인터
		POINT thisCharacterRenderPortrait = { 0 };							//현재 캐릭터 초상화 그릴 위치
		POINT thisCharacterRenderPortraitOriginal = { -158,100 };			//현재 캐릭터 원위치
		POINT thisCharacterRenderPortraitToMove = { 100,100 };				//현재 캐릭터 이동하고자 하는 위치
		POINT thisCharacterRenderName = { 0 };								//현재 캐릭터 이름 그릴 위치
		POINT thisCharacterRenderHealthPredict = { 0 };						//현재 캐릭터 체력 예상 그릴 위치
		POINT thisCharacterRenderHitRatePredict = { 0 };					//현재 캐릭터 명중 예상 그릴 위치
		POINT thisCharacterRenderWeaponAdvantage = { 0 };					//현재 캐릭터 무기 상성 그릴 위치
	};

	//▼행동 들어왔을시
	struct SelectionBox
	{RECT selectionBoxList;			//선택 옵션에 따른 고르는 박스
	RECT selectionBoxTomove;};		//선택 옵션에 따른 고르는 박스
	
private:

	ToShow toShow;					//어느 상호작용을 요구하는지 체크하는 옵션
	class Cursor *cursor;			//커서는 너무 커서
	FLOAT photoFrameAlpha;			//포토프레임 서서히 밝아짐
	HealPredict healPredict;		//힐 예측 상태일때 사용하는 변수들 모음
	SelectionBox selectionBox;		//고르는박스 상태일때 사용하는 변수들 모음
	BattlePredict battlePredict;	//전투예측 상태일때 사용하는 변수들 모음


public:

	void Release()override;
	void Update()override;
	void Render()override;
	void Init() override;

	void SetBattlePredict(Character* _thisCharacter, Character* _enemy) { battlePredict.thisCharacterPtr = _thisCharacter; battlePredict.enemyPtr = _enemy; }
	void SetHealPredict(Character* _thisCharacter, Character* _ally) { healPredict.thisCharacter = _thisCharacter; healPredict.allyCharacter = _ally; }
	void SetToShow(ToShow _whatToShow);  
	void SetPhotoFrameAlphaZero() { photoFrameAlpha = 0; }
	void SetBattlePredictInwards();
	void SetBattlePredictOutwards();

	const ToShow & GetToShow() const { return toShow; }

	SelectionUI();
	~SelectionUI() {};
};

