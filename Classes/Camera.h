#pragma once
class Camera
{
public:
	static Camera& Getinstance() //정적 싱글톤
	{
		static Camera instance;
		return instance;
	}
private:
	Camera();
	~Camera() {};

	//RECT clientRc; //윈도우말고 클라이언트 영역
	RECT cRc;
public:
	//▼겟터
	const RECT &GetCameraRc() const{ return cRc; }
	const POINT GetCentrePoint() const { return { cRc.right - cRc.left, cRc.bottom - cRc.top }; }

	//▼세터
	void SetCamera(RECT _cRc) { cRc = _cRc; }	//카메라 렉트 대체하는 함수
	void SetCamera(POINT _centre, UINT _width, UINT _height) { cRc = RectMakeCentre(_centre.x, _centre.y, _width, _height); }


	RECT RelativeCameraRect(RECT _toConvert);

	//void SetClientRect(RECT _clientRc) { clientRc = _clientRc; } //카메라 생성 시점엔 _hWnd를 모르기에 추후 넘겨줘야 한다


	//▼콘스트 빠진 참조자, Ref붙은것들은 사용에 주의를 요망
	RECT &RefCameraRc() { return cRc; }
};

