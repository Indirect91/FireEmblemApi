#pragma once
#include "GameObject.h"

constexpr UINT TILESIZE = 48; //Ÿ�� �ϳ��� ������
constexpr UINT TILEROWY = 20; //Ÿ�� Y ����
constexpr auto TILECOLX = 32; //Ÿ�� X ����, constexpr �ڵ��߷� �׽�Ʈ

class Tiles : public GameObject
{
public:
	enum class TileStatus
	{
		none,				//�ƹ� ���µ� �ƴ�
		red,				//���� ���� Ÿ�� ǥ����
		blue,				//�̵� ���� Ÿ�� ǥ����
		enemyOccupied,		//���� ������ ǥ��
		playerOccupied		//�Ʊ��� ������ ǥ��
	};

private:

	TileStatus status;	//Ÿ�Ͽ� ������ �ö�Դ���

	std::string terrain;//Ÿ���� ����ִ� ����
	POINT terrainFrame;	//Ÿ������ ���� �����ӷ�����

	std::string object; //Ÿ���� ����ִ� ������Ʈ
	POINT objFrame;		//Ÿ������ ������Ʈ �����ӷ�����

	std::string movingT;//Ÿ���� ����ִ� �����̴� Ÿ��
	POINT movingtFrame;	//�����̴� Ÿ�� �����ӷ�����
	
	BOOL checked;			//Ÿ���� �˻�Ǿ����� ����
	INT blueNum;			//Ÿ���� ���� �ߺ� �Ǵܽ� ���
	FLOAT blueAlpha;		//���� ���� �Ķ� ����
	BOOL blueClicked;		//Ŭ�� ���ȴ��� ����

public:
	//��������̵�� �Լ���
	virtual void Init()override;
	virtual void Release()override;
	virtual void Update()override;
	virtual void Render()override;

	//�� ����. �����ڷ� ��ȯ�ϵ� ��������ʰ� ���ȭ.
	const TileStatus & GetTileStatus() const { return status; }

	//�� ������ �����̴� ������ Ÿ��. �� ù���� ��
	const std::string& GetMovingT() const { return movingT; }
	const POINT& GetMovingtFrame() const { return movingtFrame; }

	//�� ���� ����. ��°�� ��
	const std::string& GetTerrain() const { return terrain; }
	const POINT& GetTerrainFrame() const { return objFrame; }

	//�� ������Ʈ��. ����° ���̾�
	const std::string & GetObj() const { return object; }
	const POINT & GetObjFrame() const { return objFrame; }

	const BOOL& GetIsChecked() const { return checked; } // ���� �˻��� Ÿ�� üũ����
	const BOOL &GetIsBlue() const { return blueNum; } //�Ķ�Ÿ������ ������
	const FLOAT &GetBlueAlpha() const { return blueAlpha; } //�Ķ� ���İ� ����

	//�� ���� Ÿ���� ���������� �ľ���
	void SetStatus(TileStatus _status) { this->status = _status; }

	//������̴� Ÿ�� ����
	void SetMovingT(const std::string _movingT) { this->terrain = _movingT; }
	void SetMovingtFrame(const POINT _movingtFrame) { this->movingtFrame = _movingtFrame; }

	//������ ����
	void SetTerrain(const std::string _terrain) { this->terrain = _terrain; }
	void SetTerrainFrame(const POINT _terrainFrame) { this->terrainFrame = _terrainFrame; }

	//�������Ʈ ����
	void SetObj(const std::string _object) { this->object = _object; }
	void SetObjFrame(const POINT _objFrame) { this->objFrame = _objFrame; }

	void SetIsChecked(const BOOL _checked) { this->checked;} //üũ ���� ����
	void IncreaseBlueNum() { blueNum++; } //��� �������� ����
	void SetBlueNum(INT _blue); //��� �������� ��������
	void DecreaseBlueNum(); //��� �������� ����

	void SetBlueAlpha(const FLOAT _blueAlpha) { blueAlpha = _blueAlpha; } //�����İ� ����

	Tiles();
	~Tiles() {};
};

