#pragma once
class Camera
{
private:
	enum class CameraStatus : UINT
	{
		Idle, Moving,
	};

public:
	static Camera& Getinstance() //���̾�� ���� �̱���. ù ȣ�� ������ ������ ���� �ȵ�
	{
		static Camera instance;
		return instance;
	}
private:
	Camera(); //�� �ν��Ͻ��� ������ ������ ���� ���ϰ� ������ ����
	~Camera() {}; //�Ҹ��� ����

	
	RECT cRc;				//ī�޶� ��Ʈ
	RECT prevRc;			//ī�޶� ������ǥ
	POINT toFollow;			//����ٴ� ���(��ǥ)
	FLOAT followSpeed;		//����ٴϴ� �ӵ�
	FLOAT maxDistance;		//�ִ� �ӵ��� ���� �Ÿ�
	FLOAT minDistance;		//�ּ� �ӵ��� ���� �Ÿ�
	FLOAT maxFollowSpd;		//����ٴϴ� �ӵ�
	class Cursor* cursor;	//Ŀ��
	CameraStatus cStatus;	//ī�޶� ����
	RECT adjustFourPoints;	//ī�޶� ���� �ִ�ġ

	void SmoothenCamera();	//����ٴ� ������ �ε巴�� �ٲ�
	void AdjustCamera();	//ī�޶� ������ ����

public:
	void Init();
	void Update();


	//�����
	const RECT &GetCameraRc() const{ return cRc; }

	//�弼��
	void SetCamera(RECT _cRc) { cRc = _cRc; }	//ī�޶� ��Ʈ ��ü�ϴ� �Լ�
	void SetCamera(POINT _centre, UINT _width, UINT _height) { cRc = RectMakeCentre(_centre.x, _centre.y, _width, _height); }
	void SetFollowingSpeed(FLOAT _maxFollowSpeed) { maxFollowSpd = _maxFollowSpeed; }	//����ٴϴ� �ӵ� ����
	void SetAdjustingPoints(RECT _toAdjust) { adjustFourPoints = _toAdjust; }		//������ų ��Ʈ �ٲܶ� ���

	void Follow(RECT _target) { toFollow = GetRectCentre(_target); }				//Ÿ���� ����ٴϰ� ��, ��Ʈ�� �����ε�
	void Follow(POINT _target) { toFollow = _target; }								//Ÿ���� ����ٴϰ� ��
	RECT RelativeCameraRect(RECT _toConvert);										//ī�޶� ������� ��Ʈ ��ȯ
	POINT RelativeCameraPoint(POINT _toConvert);									//ī�޶� ������� ��Ʈ ��ȯ

	RECT &RefCameraRc() { return cRc; }
};