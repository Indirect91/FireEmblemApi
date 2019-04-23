#pragma once
#include "GameObject.h"

constexpr UINT TILESIZE = 48; //타일 하나의 사이즈
constexpr UINT TILEROWY = 20; //타일 Y 갯수
constexpr auto TILECOLX = 32; //타일 X 갯수, constexpr 자동추론 테스트

class Tiles : public GameObject
{
public:
	enum class TileStatus
	{
		none,				//아무 상태도 아님
		red,				//공격 범위 타일 표시중
		blue,				//이동 범위 타일 표시중
		enemyOccupied,		//적이 점거중 표시
		playerOccupied		//아군이 점거중 표시
	};

private:

	TileStatus status;	//타일에 무엇이 올라왔는지

	std::string terrain;//타일이 들고있는 지형
	POINT terrainFrame;	//타일위의 지형 프레임렌더용

	std::string object; //타일이 들고있는 오브젝트
	POINT objFrame;		//타일위의 오브젝트 프레임렌더용

	std::string movingT;//타일이 들고있는 움직이는 타일
	POINT movingtFrame;	//움직이는 타일 프레임렌더용
	
	BOOL checked;			//타일이 검사되었는지 여부
	INT blueNum;			//타일의 범위 중복 판단시 사용
	FLOAT blueAlpha;		//진한 연한 파란 여부
	BOOL blueClicked;		//클릭 눌렸는지 여부

public:
	//▼오버라이드된 함수들
	virtual void Init()override;
	virtual void Release()override;
	virtual void Update()override;
	virtual void Render()override;

	//▼ 겟터. 참조자로 반환하되 변경되지않게 상수화.
	const TileStatus & GetTileStatus() const { return status; }

	//▼ 물같이 움직이는 프레임 타일. 맨 첫쨰로 깔림
	const std::string& GetMovingT() const { return movingT; }
	const POINT& GetMovingtFrame() const { return movingtFrame; }

	//▼ 실제 지형. 둘째로 깔림
	const std::string& GetTerrain() const { return terrain; }
	const POINT& GetTerrainFrame() const { return objFrame; }

	//▼ 오브젝트들. 세번째 레이어
	const std::string & GetObj() const { return object; }
	const POINT & GetObjFrame() const { return objFrame; }

	const BOOL& GetIsChecked() const { return checked; } // 지형 검색용 타일 체크여부
	const BOOL &GetIsBlue() const { return blueNum; } //파란타일인지 참거짓
	const FLOAT &GetBlueAlpha() const { return blueAlpha; } //파란 알파값 설정

	//▼ 누가 타일을 점령중인지 파악함
	void SetStatus(TileStatus _status) { this->status = _status; }

	//▼움직이는 타일 세팅
	void SetMovingT(const std::string _movingT) { this->terrain = _movingT; }
	void SetMovingtFrame(const POINT _movingtFrame) { this->movingtFrame = _movingtFrame; }

	//▼지형 세팅
	void SetTerrain(const std::string _terrain) { this->terrain = _terrain; }
	void SetTerrainFrame(const POINT _terrainFrame) { this->terrainFrame = _terrainFrame; }

	//▼오브젝트 세팅
	void SetObj(const std::string _object) { this->object = _object; }
	void SetObjFrame(const POINT _objFrame) { this->objFrame = _objFrame; }

	void SetIsChecked(const BOOL _checked) { this->checked;} //체크 여부 변경
	void IncreaseBlueNum() { blueNum++; } //블루 참조갯수 증가
	void SetBlueNum(INT _blue); //블루 참조갯수 강제설정
	void DecreaseBlueNum(); //블루 참조갯수 감소

	void SetBlueAlpha(const FLOAT _blueAlpha) { blueAlpha = _blueAlpha; } //블루알파값 세팅

	Tiles();
	~Tiles() {};
};

