#pragma once
#include "GameObject.h"




class Tiles : public GameObject
{
public:

	//��Ÿ�� ����
	enum class TileStatus : UINT
	{
		none,				//�ƹ� ���µ� �ƴ�
		enemyOccupied,		//���� ������ ǥ��
		playerOccupied		//�Ʊ��� ������ ǥ��
	};

	//��
	struct colourTiles
	{
		colourTiles() { Num = 0; Alpha = 0; Clicked = false; }
		INT Num;
		FLOAT Alpha;
		BOOL Clicked;
	};

private:
	BOOL isStartLocation = 0;//�÷��̾� ĳ������ ���� ����������

	TileStatus status;	//Ÿ�Ͽ� ������ �ö�Դ���

	std::string terrain;//Ÿ���� ����ִ� ����
	POINT terrainFrame;	//Ÿ������ ���� �����ӷ�����

	std::string objectT; //Ÿ���� ����ִ� ������Ʈ
	POINT objTFrame;		//Ÿ������ ������Ʈ �����ӷ�����

	std::string movingT;//Ÿ���� ����ִ� �����̴� Ÿ��
	POINT movingtFrame;	//�����̴� Ÿ�� �����ӷ�����

	std::string arrowT;	//Ÿ���� ����ִ� ȭ��ǥ
	POINT arrowtFrame;	//ȭ��ǥ �����ӷ�����
	
	INT checkedNum = 0;			//Ÿ���� �˻�Ǿ����� ����
	INT routeNum = 0;			//�� ��ġ �������� ���� ����

	std::vector<Tiles*> neighbour;
	colourTiles purpleTile;
	colourTiles greenTile;
	colourTiles blueTile;
	colourTiles redTile;
	
public:
	//��������̵�� �Լ���
	virtual void Init()override;
	virtual void Release()override;
	virtual void Update()override;
	virtual void Render()override {}

	//�� ����. �����ڷ� ��ȯ�ϵ� ��������ʰ� ���ȭ.
	const TileStatus & GetTileStatus() const { return status; }

	//�� ������ �����̴� ������ Ÿ��. ù° ���̾�
	const std::string& GetMovingT() const { return movingT; }
	const POINT& GetMovingtFrame() const { return movingtFrame; }

	//�� ���� ����. ��° ���̾�
	const std::string& GetTerrain() const { return terrain; }
	const POINT& GetTerrainFrame() const { return terrainFrame; }

	//�� ������Ʈ��. ����° ���̾�
	const std::string & GetObjT() const { return objectT; }
	const POINT & GetObjTFrame() const { return objTFrame; }

	//�� ȭ��ǥ. �׹�° ���̾�
	const std::string& GetArrowT() const { return arrowT; }
	const POINT& GetArrowFrame() const { return arrowtFrame; }

	//������ ��ã��/����ǥ�ÿ�
	const INT & GetCheckedNum() const { return checkedNum; }		// ���� �˻��� Ÿ�� üũ����
	const INT& GetRouteNum() const { return routeNum; }

	//���Ķ�Ÿ�Ͽ�
	const BOOL & GetIsBlue() const { return blueTile.Num; }			//�Ķ�Ÿ������ ������
	const FLOAT & GetBlueAlpha() const { return blueTile.Alpha; }	//�Ķ� ���İ� ����

	//�廡��Ÿ�Ͽ�
	const BOOL& GetIsRed() const { return (redTile.Num); }			//����Ÿ������ ������
	const FLOAT& GetRedAlpha() const { return redTile.Alpha; }			//�Ķ� ���İ� ����

	//���ʷ�Ÿ�Ͽ�
	const BOOL& GetIsGreen() const { return (greenTile.Num); }			//����Ÿ������ ������
	const FLOAT& GetGreenAlpha() const { return greenTile.Alpha; }			//�Ķ� ���İ� ����

	//�庸��Ÿ�Ͽ�
	const BOOL& GetIsPurple() const { return (purpleTile.Num); }			//����Ÿ������ ������
	const FLOAT& GetPurpleAlpha() const { return purpleTile.Alpha; }			//�Ķ� ���İ� ����

	//�� ���� Ÿ���� ���������� �ľ���
	void SetStatus(TileStatus _status) { this->status = _status; }

	//������̴� Ÿ�� ����
	void SetMovingT(const std::string _movingT) { this->movingT = _movingT; }
	void SetMovingtFrame(const POINT _movingtFrame) { this->movingtFrame = _movingtFrame; }

	//������ ����
	void SetTerrain(const std::string _terrain) { this->terrain = _terrain; }
	void SetTerrainFrame(const POINT _terrainFrame) { this->terrainFrame = _terrainFrame; }

	//�������Ʈ ����
	void SetObjT(const std::string _object) { this->objectT = _object; }
	void SetObjTFrame(const POINT _objFrame) { this->objTFrame = _objFrame; }

	//��ȭ��ǥ ����
	void SetArrowT(const std::string _arrow) { this->arrowT = _arrow; }
	void SetArrowtFrame(const POINT _arrowFrame) { this->arrowtFrame = _arrowFrame; }

	void SetCheckedNum(const INT _checked) { this->checkedNum = _checked;} //üũ ���� ����
	void SetRouteNum(const INT _routeNum) { routeNum = _routeNum; }			//�� ������ ����

	//�忡�̽�Ÿ��
	BOOL isVisited = false;				//�湮 ����
	FLOAT fGlobalGoal = 0;				//������������ �Ÿ�
	FLOAT fLocalGoal = 0;				//�������������� �Ÿ�
	Tiles* parent = nullptr;			//���� ����� �θ� ã�ư��� ���� ������


	void IncreaseBlueNum();			//��� �������� ����
	void IncreaseRedNum();			//���� �������� ����
	void IncreaseGreenNum();		//�׸� �������� ����
	void IncreasePurpleNum();		//���� �������� ����

	void SetBlueNum(INT _blue);		//��� �������� ��������
	void SetGreenNum(INT _green);	//�׸� �������� ��������
	void SetRedNum(INT _red);		//���� �������� ��������
	void SetPurpleNum(INT _Purple);	//���� �������� ��������

	void DecreaseBlueNum();			//��� �������� ����
	void DecreaseRedNum();			//���� �������� ����
	void DecreaseGreenNum();		//�׸� �������� ����
	void DecreasePurpleNum();		//���� �������� ����

	void SetBlueAlpha(const FLOAT _blueAlpha) { blueTile.Alpha = _blueAlpha; }		//�����İ� ����
	void SetGreenAlpha(const FLOAT _green) { greenTile.Alpha = _green; }			//�ʷϾ��İ� ����
	void SetRedAlpha(const FLOAT _red) { redTile.Alpha = _red; }					//�������İ� ����
	void SetPurpleAlpha(const FLOAT _purple) {purpleTile.Alpha = _purple; }			//������İ� ����

	//�忡�̽�Ÿ ���� ������
	std::vector<Tiles*>& RefNeighbours() { return neighbour; }
	BOOL& RefIsVisited() { return isVisited; }
	FLOAT& RefGlobalGoal() { return fGlobalGoal; }
	FLOAT& RefLocalGoal() { return fLocalGoal; }
	Tiles* GetParentPtr() { return parent; }
	void SetParentPtr(Tiles * _parentPtr) { parent = _parentPtr; }

	Tiles();
	~Tiles() {};
};

