#pragma once
#include "GameNode.h"
class GameObject : public GameNode
{
protected:
	RECT position; //��� ���ӻ��� ��ü�� �������� ������


public:





	//�����. �����ڷ� ��ȯ�ϵ� ��������ʰ� ���ȭ.
	const RECT & GetPosition()const { return this->position; }

	//�����. ���ȭ���Ѽ� ���������ʰ� �� �����ڷ� �޾� ������ ��������.
	void SetPosition(const RECT &_position) { this->position = _position; }




	GameObject() {};
	~GameObject() {};
};

