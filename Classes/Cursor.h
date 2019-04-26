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
	
	//▼커서 색을 표시하기 위한 열거형
	enum class CursorColour
	{
		PlayerTurn, EnemyTurn, UI
	};

private:
	CursorColour cursorColour;				//커서 색깔
	CursorState cursorState;				//커서 상태
	std::string cursorOccupied;				//커서 점유상태
	POINT prevLocation;						//커서 드래그시 시작지점
	UINT cursorFrame;						//커서 프레임
	UINT cursorCounter;						//커서 프레임올릴 카운터

	void CursorFrameManage();				//커서 프레임 돌릴 함수 
	std::map <std::string, GameObject*>& clippedTiles = DATACENTRE.RefObjects(ObjType::ClippedTile);

public:
	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	BOOL MoveUp();
	BOOL MoveDown();
	BOOL MoveLeft();
	BOOL MoveRight();

	UINT &GetCursorFrame() { return cursorFrame; }
	std::string& GetCursorOccupied() { return cursorOccupied; }

	void SetCursorOccupied(std::string _target) { cursorOccupied = _target; }	//대상 설정
	void SetCursorStatus(CursorState state) { cursorState = state; }		//M키 활성화여부
	void SetCursorColour(const CursorColour &colour) { cursorColour = colour; }	//색 세팅

	Cursor();
	~Cursor() {};
};

