#pragma once
#include "GameNode.h"

class GameObject : public GameNode
{
protected:
	RECT position;  //��� ���ӻ��� ��ü�� �������� ������
	POINT index;	//���� �������� �ƴ� �ε����� �̵��ϱ⵵ �Ѵ�
public:

	//�����. �����ڷ� ��ȯ�ϵ� ��������ʰ� ���ȭ.
	const RECT & GetPosition()const { return this->position; }
	const POINT& GetIndex()const { return this->index; }

	//�����. ���ȭ���Ѽ� ���������ʰ� �� �����ڷ� �޾� ������ ��������.
	void SetPosition(const RECT &_position) { this->position = _position; }
	void SetIndex(const RECT& _index) { this->position = _index; }

	GameObject();
	~GameObject() {};
};

