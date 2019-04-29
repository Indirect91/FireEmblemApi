#pragma once
#include "GameObject.h"
class Cursor : public GameObject
{
public:
	//▼커서의 상태. 추후 UI쪽 추가
	enum class CursorState
	{
		cursorActivated, cursorDeactivated, 
	};

private:
	IngameStatus cursorTurn;				//커서 턴
	IngameStatus cursorTurnPrev;			//커서 이전 턴
	CursorState cursorState;				//커서 상태
	std::string cursorOccupied;				//커서 점유상태
	POINT prevLocation;						//커서 드래그시 시작지점
	UINT cursorFrame;						//커서 프레임
	UINT cursorCounter;						//커서 프레임올릴 카운터

	std::map <std::string, GameObject*>& clippedTiles = DATACENTRE.RefObjects(ObjType::ClippedTile);

private:
	void CursorFrameManage();				//커서 프레임 돌릴 함수 

public:
	void Init() override;
	void Render()override;
	void Update()override;
	void Release()override;

	BOOL MoveUp();
	BOOL MoveDown();
	BOOL MoveLeft();
	BOOL MoveRight();

	const UINT &GetCursorFrame() const{ return cursorFrame; }
	std::string & GetCursorOccupied() { return cursorOccupied; }
	const IngameStatus &GetCursorTurn() const { return cursorTurn; }
	const IngameStatus& GetCursorTurnPrev() const { return cursorTurnPrev; }

	void SetCursorStatus(CursorState state) { cursorState = state; }				//M키 활성화여부
	void SetCursorTurn(const IngameStatus &_turn) { cursorTurn = _turn; }			//턴 세팅
	void SetCursorOccupied(std::string _target) { cursorOccupied = _target; }		//대상 설정
	void SetCursorTurnPrev(const IngameStatus& _turnPrev) { cursorTurnPrev = _turnPrev; }			//턴 세팅
	

	Cursor();
	~Cursor() {};
};

