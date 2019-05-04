#pragma once
#include "GameNode.h"
class TitleScene : public GameNode
{
	struct selectionTool
	{
		selectionTool() { x = 0; y = 0; }
		UINT x;		//�ڽ� ��ǥ
		UINT y;		//�ڽ� ��ǥ
	};
	
	BOOL isMaptool;			//������ �ȵ� ����
	BOOL isNewPressed;		//New��ư ���ȴ��� ����
	BOOL isContinuePressed;	//Continue ��ư ���ȴ��� ����
	BOOL isDone;			//������ ���ġ �������� ����
	BOOL ascending;			//���ϴ� �����׸� ���İ� ���������
	UINT Tcounter;			//�׳� ī����
	UINT falchionframeX;	//�Ƚÿ� ������ ī����X
	UINT Mcounter;			//�޴��� ī����

	float pickAlpha;		//���ϴ� �׸� ���İ�
	float titleAlpha;		//���ϴ� �׸� ���İ�
	float moveWindowRateX;	//������ �̵���ų ��ġ
	float moveWindowRateY;	//������ �̵���ų ��ġ
	float moveWindowRateAccX; //������ �̵���ų ��ġ ����
	float moveWindowRateAccY; //������ �̵���ų ��ġ ����
	float resizeWindowRateX; //������ ũ�� �������� ��ġ
	float resizeWindowRateY; //������ ũ�� �������� ��ġ
	float resizeWindowRateAccX; //������ ũ�� �������� ��ġ
	float resizeWindowRateAccY; //������ ũ�� �������� ��ġ

	selectionTool selectionBox; //���� �ڽ�


public:

	void Init();
	void Release();
	void Update();
	void Render();

	TitleScene();
	~TitleScene() {};
};

