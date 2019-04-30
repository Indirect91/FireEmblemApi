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
	std::string isEnemyOn;							//Ŀ���� �� ���� �ִ���

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

	const IngameStatus& GetCursorTurnPrev() const { return cursorTurnPrev; }
	const IngameStatus &GetCursorTurn() const { return cursorTurn; }
	std::string & GetCursorOccupied() { return cursorOccupied; }
	const UINT &GetCursorFrame() const{ return cursorFrame; }
	const std::string& GetIsEnemyOn() { return isEnemyOn; }

	void SetCursorTurnPrev(const IngameStatus& _turnPrev) { cursorTurnPrev = _turnPrev; }			//�� ����
	void SetCursorOccupied(std::string _target) { cursorOccupied = _target; }						//��� ����
	void SetCursorTurn(const IngameStatus &_turn) { cursorTurn = _turn; }							//�� ����
	void SetIsEnemyOn(const std::string _isEnemyOn) { isEnemyOn = _isEnemyOn; }
	void SetCursorStatus(CursorState state) { cursorState = state; }								//MŰ Ȱ��ȭ����

	Cursor();
	~Cursor() {};
};

