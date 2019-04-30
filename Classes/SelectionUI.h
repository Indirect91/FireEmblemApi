#pragma once
#include "GameObject.h"
class SelectionUI : public GameObject
{
private:


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


	SelectionUI();
	~SelectionUI() {};
};

