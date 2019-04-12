#pragma once
#include "GameNode.h"
class GameObject : public GameNode
{
protected:
	RECT position; //모든 게임상의 물체는 포지션을 가진다


public:





	//▼겟터. 참조자로 반환하되 변경되지않게 상수화.
	const RECT & GetPosition()const { return this->position; }

	//▼셋터. 상수화시켜서 변동되지않게 된 참조자로 받아 변수에 세팅해줌.
	void SetPosition(const RECT &_position) { this->position = _position; }




	GameObject() {};
	~GameObject() {};
};

