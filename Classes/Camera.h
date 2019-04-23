#pragma once
class Camera
{
public:
	static Camera& Getinstance() //���̾�� ���� �̱���
	{
		static Camera instance;
		return instance;
	}
private:
	Camera(); //���� ���ϰ� ������ ����
	~Camera() {}; //�Ҹ��� ����

	RECT cRc;				//ī�޶� ��Ʈ
	RECT prevRc;			//ī�޶� ������ǥ
	POINT toFollow;			//����ٴ� ���(��ǥ)
	FLOAT followSpeed;		//����ٴϴ� �ӵ�

	void Smoothen();		//����ٴ� ������ �ε巴�� �ٲ�

public:
	//�����
	const RECT &GetCameraRc() const{ return cRc; }

	//�弼��
	void SetCamera(RECT _cRc) { cRc = _cRc; }	//ī�޶� ��Ʈ ��ü�ϴ� �Լ�
	void SetCamera(POINT _centre, UINT _width, UINT _height) { cRc = RectMakeCentre(_centre.x, _centre.y, _width, _height); }


	void SetFollowingSpeed(FLOAT _followSpeed) { followSpeed = _followSpeed; }	//����ٴϴ� �ӵ� ����
	void Follow(RECT _target) { toFollow = GetRectCentre(_target); }			//Ÿ���� ����ٴϰ� ��, ��Ʈ�� �����ε�
	void Follow(POINT _target) { toFollow = _target; }							//Ÿ���� ����ٴϰ� ��
	RECT RelativeCameraRect(RECT _toConvert);									//ī�޶� ������� ��Ʈ ��ȯ


	//���ܽ�Ʈ ���� ������, Ref�����͵��� ��뿡 ���Ǹ� ���
	RECT &RefCameraRc() { return cRc; }
};

