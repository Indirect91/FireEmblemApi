#pragma once
class Camera
{
public:
	static Camera& Getinstance() //���� �̱���
	{
		static Camera instance;
		return instance;
	}
private:
	Camera();
	~Camera() {};

	//RECT clientRc; //�����츻�� Ŭ���̾�Ʈ ����
	RECT cRc;
public:
	//�����
	const RECT &GetCameraRc() const{ return cRc; }
	const POINT GetCentrePoint() const { return { cRc.right - cRc.left, cRc.bottom - cRc.top }; }

	//�弼��
	void SetCamera(RECT _cRc) { cRc = _cRc; }	//ī�޶� ��Ʈ ��ü�ϴ� �Լ�
	void SetCamera(POINT _centre, UINT _width, UINT _height) { cRc = RectMakeCentre(_centre.x, _centre.y, _width, _height); }


	RECT RelativeCameraRect(RECT _toConvert);

	//void SetClientRect(RECT _clientRc) { clientRc = _clientRc; } //ī�޶� ���� ������ _hWnd�� �𸣱⿡ ���� �Ѱ���� �Ѵ�


	//���ܽ�Ʈ ���� ������, Ref�����͵��� ��뿡 ���Ǹ� ���
	RECT &RefCameraRc() { return cRc; }
};

