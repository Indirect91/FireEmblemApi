#pragma once
class Camera
{
public:
	static Camera& Getinstance() //마이어식 정적 싱글톤
	{
		static Camera instance;
		return instance;
	}
private:
	Camera(); //생성 못하게 생성자 숨김
	~Camera() {}; //소멸자 숨김

	RECT cRc;				//카메라 렉트
	RECT prevRc;			//카메라 이전좌표
	POINT toFollow;			//따라다닐 대상(좌표)
	FLOAT followSpeed;		//따라다니는 속도

	void Smoothen();		//따라다닐 대상까지 부드럽게 바꿈

public:
	//▼겟터
	const RECT &GetCameraRc() const{ return cRc; }

	//▼세터
	void SetCamera(RECT _cRc) { cRc = _cRc; }	//카메라 렉트 대체하는 함수
	void SetCamera(POINT _centre, UINT _width, UINT _height) { cRc = RectMakeCentre(_centre.x, _centre.y, _width, _height); }


	void SetFollowingSpeed(FLOAT _followSpeed) { followSpeed = _followSpeed; }	//따라다니는 속도 제어
	void Follow(RECT _target) { toFollow = GetRectCentre(_target); }			//타겟을 따라다니게 함, 렉트로 오버로딩
	void Follow(POINT _target) { toFollow = _target; }							//타겟을 따라다니게 함
	RECT RelativeCameraRect(RECT _toConvert);									//카메라에 상대적인 렉트 반환


	//▼콘스트 빠진 참조자, Ref붙은것들은 사용에 주의를 요망
	RECT &RefCameraRc() { return cRc; }
};

