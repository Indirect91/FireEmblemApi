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
		AllyDead,

	};

	enum class BattleUIState
	{
		Idle,

		IsMovingOutwards,
		IsMovingInwards,
		IsBattleReady,
		IsWaitingForInput,
	};

	//▼예측화면서 사용할 그림 그릴 위치
	struct toRender
	{
		class Character* charPtr = nullptr;								//적 캐릭터 포인터
		POINT RenderPortrait = { 0 };								//적 초상화 그릴 위치
		POINT RenderPortraitOriginal = { 758,100 };					//적 초상화 원위치
		POINT RenderPortraitToMove = { 500,100 };					//적 초상화 이동하고자 하
		POINT RenderName = { 0 };									//적 이름 그릴 위치
		POINT RenderHealthPredict = { 0 };							//적 체력 예상 그릴 위치
		POINT RenderHitRatePredict = { 0 };							//적 명중 예상 그릴 위치
		POINT RenderWeaponAdvantage = { 0 };						//적 무기 상성 그릴 위치
	};

	//▼전투 예측화면일시
	struct BattlePredict
	{
		//▼통상
		BattleUIState battleUIState;
		//▼적관련
		toRender enemyRender;
		//▼공격한 캐릭터 관련
		toRender thisCharRender;
	};

	//▼힐 예측화면일시
	struct HealPredict
	{
		class Character* thisCharacter = nullptr;
		class Character* allyCharacter = nullptr;
		POINT thisCharacterRenderPosition = { 0 };
		POINT allyRenderPosition = { 0 };
	};

	//▼사망 화면일시
	struct CharDead
	{
		class Character* deadCharacter = nullptr;
		POINT deadPortraitPosition = {0,0};
		std::wstring randomText = L"";
		std::wstring toShow = L"";
		INT randomTextCounter = 0;
		INT randomTextInterval = 0;
		INT phase = 0;
	};

	//▼행동 들어왔을시
	struct SelectionBox
	{
		RECT selectionBoxList;			//선택 옵션에 따른 고르는 박스
		RECT selectionBoxTomove;		//선택 옵션에 따른 고르는 박스
	};		
	

private:
	std::vector<std::wstring> DeadDialogs;
	ToShow toShow;					//어느 상호작용을 요구하는지 체크하는 옵션
	FLOAT photoFrameAlpha;			//포토프레임 서서히 밝아짐
	HealPredict healPredict;		//힐 예측 상태일때 사용하는 변수들 모음
	SelectionBox selectionBox;		//고르는박스 상태일때 사용하는 변수들 모음
	BattlePredict battlePredict;	//전투예측 상태일때 사용하는 변수들 모음
	CharDead charDead;

public:

	void Release()override;
	void Update()override;
	void Render()override;
	void Init() override;

	void SetBattlePredict(Character* _thisCharacter, Character* _enemy) { battlePredict.thisCharRender.charPtr = _thisCharacter; battlePredict.enemyRender.charPtr = _enemy; }
	void SetHealPredict(Character* _thisCharacter, Character* _ally) { healPredict.thisCharacter = _thisCharacter; healPredict.allyCharacter = _ally; }
	void SetToShow(ToShow _whatToShow);  
	void SetPhotoFrameAlphaZero() { photoFrameAlpha = 0; }
	void SetBattlePredictInwards();
	void SetBattlePredictOutwards();
	void SetDeadChar(Character* _dead) { charDead.deadCharacter = _dead; }

	const ToShow & GetToShow() const { return toShow; }

	SelectionUI();
	~SelectionUI() {};
};

