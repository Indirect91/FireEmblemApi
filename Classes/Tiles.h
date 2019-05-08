#pragma once
#include "GameObject.h"




class Tiles : public GameObject
{
public:

	//▼타일 상태
	enum class TileStatus : UINT
	{
		none,				//아무 상태도 아님
		enemyOccupied,		//적이 점거중 표시
		playerOccupied		//아군이 점거중 표시
	};

	//▼
	struct colourTiles
	{
		colourTiles() { Num = 0; Alpha = 0; Clicked = false; }
		INT Num;
		FLOAT Alpha;
		BOOL Clicked;
	};

private:
	BOOL isStartLocation = 0;//플레이어 캐릭들이 놓일 시작점인지

	TileStatus status;	//타일에 무엇이 올라왔는지

	std::string terrain;//타일이 들고있는 지형
	POINT terrainFrame;	//타일위의 지형 프레임렌더용

	std::string objectT; //타일이 들고있는 오브젝트
	POINT objTFrame;		//타일위의 오브젝트 프레임렌더용

	std::string movingT;//타일이 들고있는 움직이는 타일
	POINT movingtFrame;	//움직이는 타일 프레임렌더용

	std::string arrowT;	//타일이 들고있는 화살표
	POINT arrowtFrame;	//화살표 프레임랜더용
	
	INT checkedNum = 0;			//타일이 검사되었는지 여부
	INT routeNum = 0;			//적 위치 추적에만 사용될 숫자

	std::vector<Tiles*> neighbour;
	colourTiles purpleTile;
	colourTiles greenTile;
	colourTiles blueTile;
	colourTiles redTile;
	
public:
	//▼오버라이드된 함수들
	virtual void Init()override;
	virtual void Release()override;
	virtual void Update()override;
	virtual void Render()override {}

	//▼ 겟터. 참조자로 반환하되 변경되지않게 상수화.
	const TileStatus & GetTileStatus() const { return status; }

	//▼ 물같이 움직이는 프레임 타일. 첫째 레이어
	const std::string& GetMovingT() const { return movingT; }
	const POINT& GetMovingtFrame() const { return movingtFrame; }

	//▼ 실제 지형. 둘째 레이어
	const std::string& GetTerrain() const { return terrain; }
	const POINT& GetTerrainFrame() const { return terrainFrame; }

	//▼ 오브젝트들. 세번째 레이어
	const std::string & GetObjT() const { return objectT; }
	const POINT & GetObjTFrame() const { return objTFrame; }

	//▼ 화살표. 네번째 레이어
	const std::string& GetArrowT() const { return arrowT; }
	const POINT& GetArrowFrame() const { return arrowtFrame; }

	//▼통합 길찾기/범위표시용
	const INT & GetCheckedNum() const { return checkedNum; }		// 지형 검색용 타일 체크여부
	const INT& GetRouteNum() const { return routeNum; }

	//▼파란타일용
	const BOOL & GetIsBlue() const { return blueTile.Num; }			//파란타일인지 참거짓
	const FLOAT & GetBlueAlpha() const { return blueTile.Alpha; }	//파란 알파값 설정

	//▼빨간타일용
	const BOOL& GetIsRed() const { return (redTile.Num); }			//빨간타일인지 참거짓
	const FLOAT& GetRedAlpha() const { return redTile.Alpha; }			//파란 알파값 설정

	//▼초록타일용
	const BOOL& GetIsGreen() const { return (greenTile.Num); }			//빨간타일인지 참거짓
	const FLOAT& GetGreenAlpha() const { return greenTile.Alpha; }			//파란 알파값 설정

	//▼보라타일용
	const BOOL& GetIsPurple() const { return (purpleTile.Num); }			//빨간타일인지 참거짓
	const FLOAT& GetPurpleAlpha() const { return purpleTile.Alpha; }			//파란 알파값 설정

	//▼ 누가 타일을 점령중인지 파악함
	void SetStatus(TileStatus _status) { this->status = _status; }

	//▼움직이는 타일 세팅
	void SetMovingT(const std::string _movingT) { this->movingT = _movingT; }
	void SetMovingtFrame(const POINT _movingtFrame) { this->movingtFrame = _movingtFrame; }

	//▼지형 세팅
	void SetTerrain(const std::string _terrain) { this->terrain = _terrain; }
	void SetTerrainFrame(const POINT _terrainFrame) { this->terrainFrame = _terrainFrame; }

	//▼오브젝트 세팅
	void SetObjT(const std::string _object) { this->objectT = _object; }
	void SetObjTFrame(const POINT _objFrame) { this->objTFrame = _objFrame; }

	//▼화살표 세팅
	void SetArrowT(const std::string _arrow) { this->arrowT = _arrow; }
	void SetArrowtFrame(const POINT _arrowFrame) { this->arrowtFrame = _arrowFrame; }

	void SetCheckedNum(const INT _checked) { this->checkedNum = _checked;} //체크 여부 변경
	void SetRouteNum(const INT _routeNum) { routeNum = _routeNum; }			//길 추적용 숫자

	//▼에이스타용
	BOOL isVisited = false;				//방문 여부
	FLOAT fGlobalGoal = 0;				//목적지까지의 거리
	FLOAT fLocalGoal = 0;				//시작점서부터의 거리
	Tiles* parent = nullptr;			//가장 가까운 부모 찾아가기 위한 포인터


	void IncreaseBlueNum();			//블루 참조갯수 증가
	void IncreaseRedNum();			//레드 참조갯수 증가
	void IncreaseGreenNum();		//그린 참조갯수 증가
	void IncreasePurpleNum();		//퍼플 참조갯수 증가

	void SetBlueNum(INT _blue);		//블루 참조갯수 강제설정
	void SetGreenNum(INT _green);	//그린 참조갯수 강제설정
	void SetRedNum(INT _red);		//레드 참조갯수 강제설정
	void SetPurpleNum(INT _Purple);	//퍼플 참조갯수 강제설정

	void DecreaseBlueNum();			//블루 참조갯수 감소
	void DecreaseRedNum();			//레드 참조갯수 감소
	void DecreaseGreenNum();		//그린 참조갯수 감소
	void DecreasePurpleNum();		//퍼플 참조갯수 감소

	void SetBlueAlpha(const FLOAT _blueAlpha) { blueTile.Alpha = _blueAlpha; }		//블루알파값 세팅
	void SetGreenAlpha(const FLOAT _green) { greenTile.Alpha = _green; }			//초록알파값 세팅
	void SetRedAlpha(const FLOAT _red) { redTile.Alpha = _red; }					//빨강알파값 세팅
	void SetPurpleAlpha(const FLOAT _purple) {purpleTile.Alpha = _purple; }			//보라알파값 세팅

	//▼에이스타 빠른 참조용
	std::vector<Tiles*>& RefNeighbours() { return neighbour; }
	BOOL& RefIsVisited() { return isVisited; }
	FLOAT& RefGlobalGoal() { return fGlobalGoal; }
	FLOAT& RefLocalGoal() { return fLocalGoal; }
	Tiles* GetParentPtr() { return parent; }
	void SetParentPtr(Tiles * _parentPtr) { parent = _parentPtr; }

	Tiles();
	~Tiles() {};
};

