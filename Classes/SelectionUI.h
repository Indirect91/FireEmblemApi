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
	void SetToShow(ToShow _whatToShow) { toShow = _whatToShow; }

	const ToShow & GetToShow() const { return toShow; }

	SelectionUI();
	~SelectionUI() {};
};

