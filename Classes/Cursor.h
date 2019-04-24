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

	void SetCursorStatus(CursorState state) { cursorState = state; }
	void SetCursorColour(CursorColour colour) { cursorColour = colour; } //�� ����

	Cursor();
	~Cursor() {};
};

