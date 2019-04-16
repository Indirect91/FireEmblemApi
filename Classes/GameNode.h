#pragma once
#include "Image.h"
class GameNode
{
protected:
	RECT tempC = { 0 };	//게임노드로부터 파생된 모든 씬은 충돌용 렉트를 가진다
public:
	GameNode() {};
	virtual ~GameNode() {}; //버츄얼 소멸자를 통해, 자식의 소멸자가 씹히는걸 방지한다

	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0; 
};

