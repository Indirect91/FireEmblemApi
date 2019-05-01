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
		
	};

	struct BattlePredict
	{
		class Character* allyPtr = nullptr;
		class Character* enemyPtr = nullptr;
		POINT allyRenderPosition = { 0 };
		POINT enemyRenderPosition = { 0 };
	};

	struct SelectionBox
	{
		RECT selectionBoxList;			//선택 옵션에 따른 고르는 박스
		RECT selectionBoxTomove;		//선택 옵션에 따른 고르는 박스
	};
private:

	ToShow toShow;					//어느 상호작용을 요구하는지 체크하는 옵션
	class Cursor *cursor;			//커서는 너무 커서
	FLOAT photoFrameAlpha;			//포토프레임 서서히 밝아짐
	SelectionBox selectionBox;		//고르는박스 상태일때 사용하는 변수들 모음
	BattlePredict battlePredict;	//전투예측 상태일때 사용하는 변수들 모음


public:


	
	void Release()override;
	void Update()override;
	void Render()override;
	void Init() override;

	void SetBattlePredict(Character* _ally, Character* _enemy) { battlePredict.allyPtr = _ally; battlePredict.enemyPtr = _enemy; }
	void SetToShow(ToShow _whatToShow) { toShow = _whatToShow; }
	void SetPhotoFrameAlphaZero() { photoFrameAlpha = 0; }

	const ToShow & GetToShow() const { return toShow; }

	SelectionUI();
	~SelectionUI() {};
};

