#pragma once
#include "GameObject.h"
class Tiles : public GameObject
{
public:
	enum class TileStatus
	{
		none, red, blue, available, unavailable,
	};

private:
	RECT rc;			//Ÿ�Ͽ� ���� ��Ʈ
	std::string object; //Ÿ���� ����ִ� ������Ʈ
	std::string terrain;//Ÿ���� ����ִ� ����
	TileStatus status;	//Ÿ�� �Ķ��� ������
	UINT objX;			//Ÿ������ ������Ʈ �����ӷ�����X
	UINT objY;			//Ÿ������ ������Ʈ �����ӷ�����Y
	UINT counter;		//Ÿ�������ӷ����� ī����

public:
	//��������̵�� �Լ���
	virtual void Init()override;
	virtual void Release()override;
	virtual void Update()override;
	virtual void Render()override;

	//�����. �����ڷ� ��ȯ�ϵ� ��������ʰ� ���ȭ.
	const std::string & GetObj() const { return object; }
	const std::string & GetTerrain() const { return terrain; }
	const RECT & GetRc()const { return this->rc; }
	const TileStatus & GetTileStatus() const { return status; }
	//const UINT & GetObjY() const { return objY; }

	//�����. ���ȭ���Ѽ� ���������ʰ� �� �����ڷ� �޾� ������ ��������.
	void SetRc(const RECT &_rc) { this->rc = _rc; }
	void SetObj(const std::string &_object) { this->object = _object; }
	void SetTerrain(const std::string &_terrain) { this->terrain = _terrain; }
	void SetStatus(TileStatus &_status) { this->status = _status; }
	void SetObjY(UINT &_objY) { this->objY = _objY; }

	Tiles();
	~Tiles() {};
};

