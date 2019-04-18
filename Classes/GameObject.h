#pragma once
#include "GameNode.h"

class GameObject : public GameNode
{
protected:
	RECT position;  //��� ���ӻ��� ��ü�� �������� ������
	POINT index;	//���� �������� �ƴ� �ε����� �̵��ϱ⵵ �Ѵ�
public:

	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	//�����. �����ڷ� ��ȯ�ϵ� ��������ʰ� ���ȭ.
	const RECT & GetPosition()const { return this->position; }
	const POINT& GetIndex()const { return this->index; }

	//�����. ���ȭ���Ѽ� ���������ʰ� �� �����ڷ� �޾� ������ ��������.
	void SetPosition(const RECT &_position) { this->position = _position; }
	void SetIndex(const POINT& _index) { this->index = _index; }
	void SetPositionViaIndex() { position = RectMake(index.x*48, index.y* 48,48,48); }
	GameObject();
	~GameObject() {};
};

