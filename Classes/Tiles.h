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
		none,			//아무일도 없음
		red,			//공격 범위 타일
		blue,			//이동 범위 타일
//		available,		//?? 까먹음. 과거의 내가 만들어둠
//		unavailable,	//?? 먼가 이유가 있었을텐데 멀까
	};

private:

	TileStatus status;	//타일 파란색 빨간색
	BOOL isOccupied;	//누군가가 서있는지 여부

	std::string terrain;//타일이 들고있는 지형
	POINT terrainFrame;	//타일위의 지형 프레임렌더용

	std::string object; //타일이 들고있는 오브젝트
	POINT objFrame;		//타일위의 오브젝트 프레임렌더용

	std::string movingT;//타일이 들고있는 움직이는 타일
	POINT movingtFrame;	//움직이는 타일 프레임렌더용
	
	BOOL checked;			//타일이 검사되었는지 여부
	//BOOL checkDuplicate;	//추후 적 범위 전부 합칠때 사용
	INT blueNum;			//타일의 범위 중복 판단시 사용

public:
	//▼오버라이드된 함수들
	virtual void Init()override;
	virtual void Release()override;
	virtual void Update()override;
	virtual void Render()override;

	//▼겟터. 참조자로 반환하되 변경되지않게 상수화.
	const TileStatus & GetTileStatus() const { return status; }

	const std::string & GetObj() const { return object; }
	const POINT & GetObjFrame() const { return objFrame; }

	const std::string & GetTerrain() const { return terrain; }
	const POINT & GetTerrainFrame() const { return objFrame; }

	const std::string & GetMovingT() const { return movingT; }
	const POINT & GetMovingtFrame() const { return movingtFrame; }

	const BOOL& GetIsChecked() const { return checked; }

	//▼셋터. 인자 상수화시켜 받아옴
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

