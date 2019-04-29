#pragma once
#include "GameObject.h"
class Cursor : public GameObject
{
public:
	//��Ŀ���� ����. ���� UI�� �߰�
	enum class CursorState
	{
		cursorActivated, cursorDeactivated, 
	};

private:
	IngameStatus cursorTurn;				//Ŀ�� ��
	IngameStatus cursorTurnPrev;			//Ŀ�� ���� ��
	CursorState cursorState;				//Ŀ�� ����
	std::string cursorOccupied;				//Ŀ�� ��������
	POINT prevLocation;						//Ŀ�� �巡�׽� ��������
	UINT cursorFrame;						//Ŀ�� ������
	UINT cursorCounter;						//Ŀ�� �����ӿø� ī����

	std::map <std::string, GameObject*>& clippedTiles = DATACENTRE.RefObjects(ObjType::ClippedTile);

private:
	void CursorFrameManage();				//Ŀ�� ������ ���� �Լ� 

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

	void SetCursorStatus(CursorState state) { cursorState = state; }				//MŰ Ȱ��ȭ����
	void SetCursorTurn(const IngameStatus &_turn) { cursorTurn = _turn; }			//�� ����
	void SetCursorOccupied(std::string _target) { cursorOccupied = _target; }		//��� ����
	void SetCursorTurnPrev(const IngameStatus& _turnPrev) { cursorTurnPrev = _turnPrev; }			//�� ����
	

	Cursor();
	~Cursor() {};
};

