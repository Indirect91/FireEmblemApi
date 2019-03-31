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
	RECT rc;			//타일에 대한 렉트
	std::string object; //타일이 들고있는 오브젝트
	std::string terrain;//타일이 들고있는 지형
	TileStatus status;	//타일 파란색 빨간색
	UINT objX;			//타일위의 오브젝트 프레임렌더용X
	UINT objY;			//타일위의 오브젝트 프레임렌더용Y
	UINT counter;		//타일프레임랜더용 카운터

public:
	//▼오버라이드된 함수들
	virtual void Init()override;
	virtual void Release()override;
	virtual void Update()override;
	virtual void Render()override;

	//▼겟터. 참조자로 반환하되 변경되지않게 상수화.
	const std::string & GetObj() const { return object; }
	const std::string & GetTerrain() const { return terrain; }
	const RECT & GetRc()const { return this->rc; }
	const TileStatus & GetTileStatus() const { return status; }
	//const UINT & GetObjY() const { return objY; }

	//▼셋터. 상수화시켜서 변동되지않게 된 참조자로 받아 변수에 세팅해줌.
	void SetRc(const RECT &_rc) { this->rc = _rc; }
	void SetObj(const std::string &_object) { this->object = _object; }
	void SetTerrain(const std::string &_terrain) { this->terrain = _terrain; }
	void SetStatus(TileStatus &_status) { this->status = _status; }
	void SetObjY(UINT &_objY) { this->objY = _objY; }

	Tiles();
	~Tiles() {};
};

