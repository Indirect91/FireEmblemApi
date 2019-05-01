#pragma once
#include "GameObject.h"
class SelectionUI : public GameObject
{
public:
	//���� ��ȣ�ۿ��� �䱸�ϴ���
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
		RECT selectionBoxList;			//���� �ɼǿ� ���� ���� �ڽ�
		RECT selectionBoxTomove;		//���� �ɼǿ� ���� ���� �ڽ�
	};
private:

	ToShow toShow;					//��� ��ȣ�ۿ��� �䱸�ϴ��� üũ�ϴ� �ɼ�
	class Cursor *cursor;			//Ŀ���� �ʹ� Ŀ��
	FLOAT photoFrameAlpha;			//���������� ������ �����
	SelectionBox selectionBox;		//���¹ڽ� �����϶� ����ϴ� ������ ����
	BattlePredict battlePredict;	//�������� �����϶� ����ϴ� ������ ����


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

