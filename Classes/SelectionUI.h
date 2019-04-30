#pragma once
#include "GameObject.h"
class SelectionUI : public GameObject
{
private:


	RECT selectionBoxList;				//선택 옵션에 따른 고르는 박스
	RECT selectionBoxTomove;		//선택 옵션에 따른 고르는 박스
	class Cursor *cursor;			//커서는 너무 커서
	FLOAT photoFrameAlpha;			//포토프레임 서서히 밝아짐
public:


	
	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	void SetPhotoFrameAlphaZero() { photoFrameAlpha = 0; }


	SelectionUI();
	~SelectionUI() {};
};

