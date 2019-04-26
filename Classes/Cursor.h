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
	enum class CursorColour
	{
		PlayerTurn, EnemyTurn, UI
	};

private:
	CursorColour cursorColour;				//Ŀ�� ����
	CursorState cursorState;				//Ŀ�� ����
	std::string cursorOccupied;				//Ŀ�� ��������
	POINT prevLocation;						//Ŀ�� �巡�׽� ��������
	UINT cursorFrame;						//Ŀ�� ������
	UINT cursorCounter;						//Ŀ�� �����ӿø� ī����

	void CursorFrameManage();				//Ŀ�� ������ ���� �Լ� 
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

	void SetCursorOccupied(std::string _target) { cursorOccupied = _target; }	//��� ����
	void SetCursorStatus(CursorState state) { cursorState = state; }		//MŰ Ȱ��ȭ����
	void SetCursorColour(const CursorColour &colour) { cursorColour = colour; }	//�� ����

	Cursor();
	~Cursor() {};
};

