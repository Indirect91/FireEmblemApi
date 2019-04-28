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
	
	//��Ŀ�� ���� ǥ���ϱ� ���� ������
	enum class CursorTurn
	{
		PlayerTurn, EnemyTurn, Ally, UI
	};

private:
	CursorTurn cursorTurn;				//Ŀ�� ��
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
	void Release()override;
	void Update()override;
	void Render()override;

	BOOL MoveUp();
	BOOL MoveDown();
	BOOL MoveLeft();
	BOOL MoveRight();

	const UINT &GetCursorFrame() const{ return cursorFrame; }
	const CursorTurn &GetCursorTurn() const { return cursorTurn; }
	std::string & GetCursorOccupied() { return cursorOccupied; }

	void SetCursorOccupied(std::string _target) { cursorOccupied = _target; }	//��� ����
	void SetCursorStatus(CursorState state) { cursorState = state; }			//MŰ Ȱ��ȭ����
	void SetCursorTurn(const CursorTurn &colour) { cursorTurn = colour; }		//�� ����
	

	Cursor();
	~Cursor() {};
};

