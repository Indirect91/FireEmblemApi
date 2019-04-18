#pragma once
#include "GameNode.h"

class GameObject : public GameNode
{
protected:
	RECT position;  //모든 게임상의 물체는 포지션을 가진다
	POINT index;	//실제 포지션이 아닌 인덱스로 이동하기도 한다
public:

	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	//▼겟터. 참조자로 반환하되 변경되지않게 상수화.
	const RECT & GetPosition()const { return this->position; }
	const POINT& GetIndex()const { return this->index; }

	//▼셋터. 상수화시켜서 변동되지않게 된 참조자로 받아 변수에 세팅해줌.
	void SetPosition(const RECT &_position) { this->position = _position; }
	void SetIndex(const POINT& _index) { this->index = _index; }
	void SetPositionViaIndex() { position = RectMake(index.x*48, index.y* 48,48,48); }
	GameObject();
	~GameObject() {};
};

