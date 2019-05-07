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
		AllyDead,

	};

	enum class BattleUIState
	{
		Idle,

		IsMovingOutwards,
		IsMovingInwards,
		IsBattleReady,
		IsWaitingForInput,
	};

	//�忹��ȭ�鼭 ����� �׸� �׸� ��ġ
	struct toRender
	{
		class Character* charPtr = nullptr;								//�� ĳ���� ������
		POINT RenderPortrait = { 0 };								//�� �ʻ�ȭ �׸� ��ġ
		POINT RenderPortraitOriginal = { 758,100 };					//�� �ʻ�ȭ ����ġ
		POINT RenderPortraitToMove = { 500,100 };					//�� �ʻ�ȭ �̵��ϰ��� ��
		POINT RenderName = { 0 };									//�� �̸� �׸� ��ġ
		POINT RenderHealthPredict = { 0 };							//�� ü�� ���� �׸� ��ġ
		POINT RenderHitRatePredict = { 0 };							//�� ���� ���� �׸� ��ġ
		POINT RenderWeaponAdvantage = { 0 };						//�� ���� �� �׸� ��ġ
	};

	//������ ����ȭ���Ͻ�
	struct BattlePredict
	{
		//�����
		BattleUIState battleUIState;
		//��������
		toRender enemyRender;
		//������� ĳ���� ����
		toRender thisCharRender;
	};

	//���� ����ȭ���Ͻ�
	struct HealPredict
	{
		class Character* thisCharacter = nullptr;
		class Character* allyCharacter = nullptr;
		POINT thisCharacterRenderPosition = { 0 };
		POINT allyRenderPosition = { 0 };
	};

	//���� ȭ���Ͻ�
	struct CharDead
	{
		class Character* deadCharacter = nullptr;
		POINT deadPortraitPosition = {0,0};
		std::wstring randomText = L"";
		std::wstring toShow = L"";
		INT randomTextCounter = 0;
		INT randomTextInterval = 0;
		INT phase = 0;
	};

	//���ൿ ��������
	struct SelectionBox
	{
		RECT selectionBoxList;			//���� �ɼǿ� ���� ���� �ڽ�
		RECT selectionBoxTomove;		//���� �ɼǿ� ���� ���� �ڽ�
	};		
	

private:
	std::vector<std::wstring> DeadDialogs;
	ToShow toShow;					//��� ��ȣ�ۿ��� �䱸�ϴ��� üũ�ϴ� �ɼ�
	FLOAT photoFrameAlpha;			//���������� ������ �����
	HealPredict healPredict;		//�� ���� �����϶� ����ϴ� ������ ����
	SelectionBox selectionBox;		//���¹ڽ� �����϶� ����ϴ� ������ ����
	BattlePredict battlePredict;	//�������� �����϶� ����ϴ� ������ ����
	CharDead charDead;

public:

	void Release()override;
	void Update()override;
	void Render()override;
	void Init() override;

	void SetBattlePredict(Character* _thisCharacter, Character* _enemy) { battlePredict.thisCharRender.charPtr = _thisCharacter; battlePredict.enemyRender.charPtr = _enemy; }
	void SetHealPredict(Character* _thisCharacter, Character* _ally) { healPredict.thisCharacter = _thisCharacter; healPredict.allyCharacter = _ally; }
	void SetToShow(ToShow _whatToShow);  
	void SetPhotoFrameAlphaZero() { photoFrameAlpha = 0; }
	void SetBattlePredictInwards();
	void SetBattlePredictOutwards();
	void SetDeadChar(Character* _dead) { charDead.deadCharacter = _dead; }

	const ToShow & GetToShow() const { return toShow; }

	SelectionUI();
	~SelectionUI() {};
};

