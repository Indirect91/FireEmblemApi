#include "../stdafx.h"
#include "Camera.h"
#include "Cursor.h"
#include "Tiles.h"

//������� �ʱ�ȭ��, ��붧 �ǹ��ִ� �� ����
Camera::Camera()
{
	cRc = { 0,0,WINSIZEX,WINSIZEY }; //�ϴ��� ���� �����ŭ ����� ����
	toFollow = { WINSIZEX / 2,WINSIZEY / 2 }; //�ϴ��� ȭ�� �߰����� ���
	followSpeed = 0.5f;
	maxFollowSpd = 10;
	prevRc = cRc;
	cStatus = CameraStatus::Idle;
	cursor = nullptr;
	maxDistance = 100;
	minDistance = 5;
	adjustFourPoints = { -80,-80,TILECOLX*TILESIZE + 80, TILEROWY * TILESIZE +TILESIZE + 180};
}

//��ī�޶� �̴�. ���� Ȱ��ȭ�� Ŀ���� ����
void Camera::Init()
{
	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
}

//��ī�޶� ����
void Camera::Update()
{
	//������� ����� ���� ī�޶��� ���¸� ����
	if (!PointCompare(toFollow, GetRectCentre(cRc)))
	{
		cStatus = CameraStatus::Moving;
	}
	else
	{
		cStatus = CameraStatus::Idle;
	}



	//���־��� ���¿� ���� �ൿ
	switch (cStatus)
	{
	case Camera::CameraStatus::Idle:
		break;
	case Camera::CameraStatus::Moving:
		SmoothenCamera();	//�ε巯�� �̵��� �̵��ÿ��� ����
		AdjustCamera();		//�����ڵ�� �̵��ÿ��� ����
		break;
	}
}

//��ε巯�� �̵� ������Ʈ. ������ �Ÿ��� �������� ������ �����̰� �ּ��� ������ ������
void Camera::SmoothenCamera()
{
	POINT cCentre = GetRectCentre(cRc); //ī�޶��� ������ ����
	FLOAT toCalculate = GetDistance(cCentre, toFollow); //���� �������� �Ÿ� ���
	FLOAT angle = GetAngle(cCentre, toFollow); //ī�޶� ���������� ����ٴϷ��� �������� ���� ����

	if (toCalculate < minDistance) //���� ������ �Ÿ��� �ʹ� �۴ٸ�, ��� ��� ���Կ��� ����
	{
		cRc = RectMakeCentre(toFollow.x, toFollow.y, GetWidth(cRc), GetHeight(cRc)); //��Ʈ ����
		followSpeed = 0; //�̵��ӵ� 0ó��
	}
	else //�ʹ� ���� �Ÿ��� �ƴ϶��
	{
		if (toCalculate > maxDistance) //�Ÿ��� �ʹ� �ָ� ī�޶� �ִ� �ӵ��� �̵�
		{
			followSpeed = maxFollowSpd;
		}
		else //������ �����ؾ� �� ��
		{
			//����ٴϴ� �ӵ��� ���� �Ÿ��� ����ؼ� �޶����� ��
			followSpeed = (maxFollowSpd/(maxDistance - minDistance)) * (toCalculate - minDistance);
		}

		cCentre.x += cosf(angle) * followSpeed;		//����� �� ���� �� �̵� ����
		cCentre.y += -sinf(angle) * followSpeed;	//����� �� ���� �� �̵� ����
	}
	cRc = RectMakeCentre(cCentre.x, cCentre.y, GetWidth(cRc), GetHeight(cRc)); //�̵��� ����Ʈ�� ��Ʈ ����
}

//��ī�޶� �ִ�ġ ����
void Camera::AdjustCamera()
{
	UINT width = GetWidth(cRc);
	UINT height = GetHeight(cRc);

	if (cRc.left < adjustFourPoints.left)
	{
		cRc.left = adjustFourPoints.left;
		cRc.right = cRc.left + width;
	}
	else if (cRc.right > adjustFourPoints.right)
	{
		cRc.right = adjustFourPoints.right;
		cRc.left = cRc.right - width;
	}
	if (cRc.top < adjustFourPoints.top)
	{
		cRc.top = adjustFourPoints.top;
		cRc.bottom = cRc.top + height;
	}
	else if (cRc.bottom > adjustFourPoints.bottom)
	{
		cRc.bottom = adjustFourPoints.bottom;
		cRc.top = cRc.bottom - height;
	}
}



//��ī�޶� ������� ��Ʈ
RECT Camera::RelativeCameraRect(RECT _toConvert)
{
	_toConvert.left -= cRc.left;
	_toConvert.top -= cRc.top;
	_toConvert.right -= cRc.left;
	_toConvert.bottom -= cRc.top;

	return _toConvert;
}

//��ī�޶� ������� ����Ʈ
POINT Camera::RelativeCameraPoint(POINT _toConvert)
{
	return {_toConvert.x - cRc.left, _toConvert.y - cRc.top };
}
