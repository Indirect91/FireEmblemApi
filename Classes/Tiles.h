#pragma once
#include "GameObject.h"
#define TILESIZE 48
#define TILEROWY 48
#define TILECOLX 60

class Tiles : public GameObject
{
public:
	enum class TileStatus
	{
		none,			//�ƹ��ϵ� ����
		red,			//���� ���� Ÿ��
		blue,			//�̵� ���� Ÿ��
//		available,		//?? �����. ������ ���� ������
//		unavailable,	//?? �հ� ������ �־����ٵ� �ֱ�
	};

private:

	TileStatus status;	//Ÿ�� �Ķ��� ������
	BOOL isOccupied;	//�������� ���ִ��� ����

	std::string terrain;//Ÿ���� ����ִ� ����
	POINT terrainFrame;	//Ÿ������ ���� �����ӷ�����

	std::string object; //Ÿ���� ����ִ� ������Ʈ
	POINT objFrame;		//Ÿ������ ������Ʈ �����ӷ�����

	std::string movingT;//Ÿ���� ����ִ� �����̴� Ÿ��
	POINT movingtFrame;	//�����̴� Ÿ�� �����ӷ�����
	
	BOOL checked;			//Ÿ���� �˻�Ǿ����� ����
	//BOOL checkDuplicate;	//���� �� ���� ���� ��ĥ�� ���
	INT blueNum;			//Ÿ���� ���� �ߺ� �Ǵܽ� ���

public:
	//��������̵�� �Լ���
	virtual void Init()override;
	virtual void Release()override;
	virtual void Update()override;
	virtual void Render()override;

	//�����. �����ڷ� ��ȯ�ϵ� ��������ʰ� ���ȭ.
	const TileStatus & GetTileStatus() const { return status; }

	const std::string & GetObj() const { return object; }
	const POINT & GetObjFrame() const { return objFrame; }

	const std::string & GetTerrain() const { return terrain; }
	const POINT & GetTerrainFrame() const { return objFrame; }

	const std::string & GetMovingT() const { return movingT; }
	const POINT & GetMovingtFrame() const { return movingtFrame; }

	const BOOL& GetIsChecked() const { return checked; }

	//�����. ���� ���ȭ���� �޾ƿ�
	void SetStatus(TileStatus _status) { this->status = _status; }

	void SetObj(const std::string _object) { this->object = _object; }
	void SetObjFrame(const POINT _objFrame) { this->objFrame = _objFrame; }

	void SetTerrain(const std::string _terrain) { this->terrain = _terrain; }
	void SetTerrainFrame(const POINT _terrainFrame) { this->terrainFrame = _terrainFrame; }

	void SetMovingT(const std::string _movingT) { this->terrain = _movingT; }
	void SetMovingtFrame(const POINT _movingtFrame) { this->movingtFrame = _movingtFrame; }

	void SetIsChecked(const BOOL _checked) { this->checked; }
	void SetBlueNum(INT _blue);
	void IncreaseBlueNum();
	void DecreaseBlueNum();
	BOOL isBlue() { return blueNum; }

	Tiles();
	~Tiles() {};
};

