#pragma once
#include "GameObject.h"
class SelectionUI : public GameObject
{
public:
	enum class ToShow
	{
		selectionBox,
		battlePredict,

	};



private:

	ToShow toShow;
	RECT selectionBoxList;				//���� �ɼǿ� ���� ���� �ڽ�
	RECT selectionBoxTomove;		//���� �ɼǿ� ���� ���� �ڽ�
	class Cursor *cursor;			//Ŀ���� �ʹ� Ŀ��
	FLOAT photoFrameAlpha;			//���������� ������ �����
public:


	
	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;


	void SetPhotoFrameAlphaZero() { photoFrameAlpha = 0; }
	void SetToShow(ToShow _whatToShow) { toShow = _whatToShow; }

	const ToShow & GetToShow() const { return toShow; }

	SelectionUI();
	~SelectionUI() {};
};

