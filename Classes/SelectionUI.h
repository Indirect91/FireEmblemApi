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
		HealPredict,

	};

	enum class BattleUIState
	{
		Idle,

		IsMovingOutwards,
		IsMovingInwards,
		IsBattleReady,
		IsWaitingForInput,
	};

	//���� ����ȭ���Ͻ�
	struct HealPredict
	{
		class Character* thisCharacter = nullptr;
		class Character* allyCharacter = nullptr;
		POINT thisCharacterRenderPosition = { 0 };
		POINT allyRenderPosition = { 0 };
	};

	//������ ����ȭ���Ͻ�
	struct BattlePredict
	{
		//�����
		//BOOL isMovingOutwards = false;
		//BOOL isMovingInwards = false;
		//BOOL isBattleReady = false;
		BattleUIState battleUIState;

		//��������
		class Character* enemyPtr = nullptr;								//�� ĳ���� ������
		POINT enemyRenderPortrait = { 0 };									//�� �ʻ�ȭ �׸� ��ġ
		POINT enemyRenderPortraitOriginal = { 758,100 };					//�� �ʻ�ȭ ����ġ
		POINT enemyRenderPortraitToMove = { 500,100 };						//�� �ʻ�ȭ �̵��ϰ��� �ϴ� ��ġ
		POINT enemyRenderName = { 0 };										//�� �̸� �׸� ��ġ
		POINT enemyRenderHealthPredict = { 0 };								//�� ü�� ���� �׸� ��ġ
		POINT enemyRenderHitRatePredict = { 0 };							//�� ���� ���� �׸� ��ġ
		POINT enemyRenderWeaponAdvantage = { 0 };							//�� ���� �� �׸� ��ġ

		//������� ĳ���� ����
		class Character* thisCharacterPtr = nullptr;						//���� ĳ���� ������
		POINT thisCharacterRenderPortrait = { 0 };							//���� ĳ���� �ʻ�ȭ �׸� ��ġ
		POINT thisCharacterRenderPortraitOriginal = { -158,100 };			//���� ĳ���� ����ġ
		POINT thisCharacterRenderPortraitToMove = { 100,100 };				//���� ĳ���� �̵��ϰ��� �ϴ� ��ġ
		POINT thisCharacterRenderName = { 0 };								//���� ĳ���� �̸� �׸� ��ġ
		POINT thisCharacterRenderHealthPredict = { 0 };						//���� ĳ���� ü�� ���� �׸� ��ġ
		POINT thisCharacterRenderHitRatePredict = { 0 };					//���� ĳ���� ���� ���� �׸� ��ġ
		POINT thisCharacterRenderWeaponAdvantage = { 0 };					//���� ĳ���� ���� �� �׸� ��ġ
	};

	//���ൿ ��������
	struct SelectionBox
	{RECT selectionBoxList;			//���� �ɼǿ� ���� ���� �ڽ�
	RECT selectionBoxTomove;};		//���� �ɼǿ� ���� ���� �ڽ�
	
private:

	ToShow toShow;					//��� ��ȣ�ۿ��� �䱸�ϴ��� üũ�ϴ� �ɼ�
	class Cursor *cursor;			//Ŀ���� �ʹ� Ŀ��
	FLOAT photoFrameAlpha;			//���������� ������ �����
	HealPredict healPredict;		//�� ���� �����϶� ����ϴ� ������ ����
	SelectionBox selectionBox;		//���¹ڽ� �����϶� ����ϴ� ������ ����
	BattlePredict battlePredict;	//�������� �����϶� ����ϴ� ������ ����


public:

	void Release()override;
	void Update()override;
	void Render()override;
	void Init() override;

	void SetBattlePredict(Character* _thisCharacter, Character* _enemy) { battlePredict.thisCharacterPtr = _thisCharacter; battlePredict.enemyPtr = _enemy; }
	void SetHealPredict(Character* _thisCharacter, Character* _ally) { healPredict.thisCharacter = _thisCharacter; healPredict.allyCharacter = _ally; }
	void SetToShow(ToShow _whatToShow);  
	void SetPhotoFrameAlphaZero() { photoFrameAlpha = 0; }
	void SetBattlePredictInwards();
	void SetBattlePredictOutwards();

	const ToShow & GetToShow() const { return toShow; }

	SelectionUI();
	~SelectionUI() {};
};

