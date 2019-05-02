#pragma once
#include "GameObject.h"
class Cursor : public GameObject
{
public:
	//��Ŀ���� ����. ���� UI�� �߰�
	enum class CursorState
	{cursorActivated, cursorDeactivated,};

	//��Ʊ��̳� ������ �ִ��� �Ǵ��Ҷ�
	struct IsOtherUnitOn
	{
		IsOtherUnitOn() { name = ""; whos = OwnedBy::Player; }
		std::string name;
		OwnedBy whos;
	};

private:
	UINT cursorFrame;						//Ŀ�� ������
	POINT prevLocation;						//Ŀ�� �巡�׽� ��������
	UINT cursorCounter;						//Ŀ�� �����ӿø� ī����
	IngameStatus cursorTurn;				//Ŀ�� ��
	CursorState cursorState;				//Ŀ�� ����
	std::string cursorOccupied;				//Ŀ�� ��������
	IngameStatus cursorTurnPrev;			//Ŀ�� ���� ��
	IsOtherUnitOn isOtherUnitOn;			//Ŀ���� �� ���� �ִ���

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
	const IsOtherUnitOn& GetIsOtherUnit() { return isOtherUnitOn; }
	const IngameStatus &GetCursorTurn() const { return cursorTurn; }
	const IngameStatus& GetCursorTurnPrev() const { return cursorTurnPrev; }

	void SetCursorStatus(CursorState state) { cursorState = state; }								//MŰ Ȱ��ȭ����
	void SetCursorTurn(const IngameStatus &_turn) { cursorTurn = _turn; }							//�� ����
	void SetCursorOccupied(std::string _target) { cursorOccupied = _target; }						//��� ����
	void SetCursorTurnPrev(const IngameStatus& _turnPrev) { cursorTurnPrev = _turnPrev; }			//�� ����
	
	void SetIsOtherUnitOn(std::string _name, OwnedBy _whos) { isOtherUnitOn.name = _name; isOtherUnitOn.whos = _whos; }

	Cursor();
	~Cursor() {};
};

