#pragma once
class Camera
{
private:
	enum class CameraStatus : UINT
	{
		Idle, Moving,Shaking
	};

public:
	static Camera& Getinstance() //마이어식 정적 싱글톤. 첫 호출 전까진 생성자 실행 안됨
	{
		static Camera instance;
		return instance;
	}
private:
	Camera(); //겟 인스턴스를 통하지 않으면 생성 못하게 생성자 숨김
	~Camera() {}; //소멸자 숨김

	
	RECT cRc;				//카메라 렉트
	RECT prevRc;			//카메라 이전좌표
	POINT toFollow;			//따라다닐 대상(좌표)
	FLOAT followSpeed;		//따라다니는 속도
	FLOAT maxDistance;		//최대 속도를 가질 거리
	FLOAT minDistance;		//최소 속도를 가질 거리
	FLOAT maxFollowSpd;		//따라다니는 속도
	class Cursor* cursor;	//커서
	CameraStatus cameraStatus;	//카메라 상태
	RECT adjustFourPoints;		//카메라 보정 최대치
	INT shakingTimer = 30;		//카메라 흔드는 값
	BOOL shakingLeft = true;	//카메라 흔드는 방향
	
	void SmoothenCamera();	//따라다닐 대상까지 부드럽게 바꿈
	void AdjustCamera();	//카메라 끝지점 보정

public:
	void Init();
	void Update();


	//▼겟터
	const RECT &GetCameraRc() const{ return cRc; }
	

	//▼세터
	void SetCamera(RECT _cRc) { cRc = _cRc; }	//카메라 렉트 대체하는 함수
	void SetCamera(POINT _centre, UINT _width, UINT _height) { cRc = RectMakeCentre(_centre.x, _centre.y, _width, _height); }
	void SetFollowingSpeed(FLOAT _maxFollowSpeed) { maxFollowSpd = _maxFollowSpeed; }	//따라다니는 속도 제어
	void SetAdjustingPoints(RECT _toAdjust) { adjustFourPoints = _toAdjust; }		//보정시킬 렉트 바꿀때 사용

	
	void SetShake(INT _shakeTimeEvenNumber) { cameraStatus = CameraStatus::Shaking; shakingTimer = _shakeTimeEvenNumber; }

	void Follow(RECT _target) { toFollow = GetRectCentre(_target); }				//타겟을 따라다니게 함, 렉트로 오버로딩
	void Follow(POINT _target) { toFollow = _target; }								//타겟을 따라다니게 함
	RECT RelativeCameraRect(RECT _toConvert);										//카메라에 상대적인 렉트 반환
	POINT RelativeCameraPoint(POINT _toConvert);									//카메라에 상대적인 렉트 반환

	RECT &RefCameraRc() { return cRc; }
};