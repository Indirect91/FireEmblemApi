#pragma once
//=============================================================
//	## commomMacroFunction ##
//=============================================================

//POINT
inline POINT PointMake(int x, int y)
{
	POINT pt = { x, y };
	return pt;
}

//�� �׸���
inline void LineMake(HDC hdc, int startX, int startY, int endX, int endY)
{
	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, endX, endY);
}

//RECT �����
inline RECT RectMake(int x, int y, int width, int height)
{
	RECT rc = { x, y, x + width, y + height };
	return rc;
}

//RECT �߽���ǥ�� �����
inline RECT RectMakeCenter(int x, int y, int width, int height)
{
	RECT rc = { x - width / 2, y - height / 2, x + width / 2, y + height / 2 };
	return rc;
}

//���ʱ��Լ� �̿��ؼ� �簢 �׸���
inline void RectagleMake(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}

//�ϸ����Լ� �̿��ؼ� Ÿ�� �׸���
inline void EllipseMake(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x, y, x + width, y + height);
}

//�ϸ����Լ� �̿��ؼ� �������� Ÿ�� �׸���
inline void EllipseMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}

//�����Ƽ� Rectangle()�Լ� �����ε����� ������
inline void Rectangle(HDC hdc, RECT rc)
{
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
}
inline void Ellipse(HDC hdc, RECT rc)
{
	Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
}

//�����ӷ�Ʈ �Լ� �����ε�(����ִ� ��Ʈ �����)
inline void FrameRect(HDC hdc, RECT rc, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	FrameRect(hdc, &rc, brush);
	DeleteObject(brush);
}

inline RECT RectMakePivot(POINTFLOAT pos, POINTFLOAT size, Pivot pivot)
{
	RECT result;
	switch (pivot)
	{
	case Pivot::LeftTop:
		result.left = pos.x;
		result.top = pos.y;
		result.right = pos.x + size.x;
		result.bottom = pos.y + size.y;
		return result;
	case Pivot::Center:
		result.left = pos.x - size.x / 2.f;
		result.top = pos.y - size.y / 2.f;
		result.right = pos.x + size.x / 2.f;
		result.bottom = pos.y + size.y / 2.f;
		return result;
	case Pivot::Bottom:
		result.left = pos.x - size.x / 2.f;
		result.top = pos.y - size.y;
		result.right = pos.x + size.x / 2.f;
		result.bottom = pos.y;
		return result;
	}
	return result;
}

inline float GetAngle(float x1, float y1, float x2, float y2)
{
	float x = x2 - x1;
	float y = y2 - y1;

	float distance = sqrtf((x * x) + (y * y));

	float angle = acos(x / distance);

	if (y2 > y1)
	{
		angle = PI * 2.0f - angle;
		if (angle >= PI * 2.0f)
			angle -= PI * 2.0f;
	}

	return angle;
}

inline float GetDistance(float x1, float y1, float x2, float y2)
{
	float x = x2 - x1;
	float y = y2 - y1;

	float distance = sqrtf((x * x) + (y * y));

	return  distance;
}

inline LONG getWidth(RECT rc)
{
	return rc.right - rc.left;
}

inline LONG getHeight(RECT rc)
{
	return rc.bottom - rc.top;
}

inline bool Ellipse_Ellipse_Collision(RECT round1, RECT round2)
{
	float c1x = (float)round1.left + (float)getWidth(round1) / 2;
	float c1y = (float)round1.top + (float)getHeight(round1) / 2;
	float c2x = (float)round2.left + (float)getWidth(round2) / 2;
	float c2y = (float)round2.top + (float)getHeight(round2) / 2;
	float deltaX = c1x - c2x;
	float deltaY = c1y - c2y;
	float length = sqrt(deltaX*deltaX + deltaY * deltaY);

	if (length > ((float)getWidth(round1) / 2 + (float)getWidth(round2) / 2)) return false;

	return true;
}

inline int RandomRangeInt(int fromNum, int toNum)
{
	return rand() % (toNum - fromNum + 1) + fromNum;
}

float RandomRangeFloat(float fromNum, float toNum)
{
	float rnd = (float)rand() / (float)RAND_MAX;
	return (rnd * (toNum - fromNum) + fromNum);
}

inline void TextOutInt(HDC hdc, int x, int y, std::string whatIsIt, int intInput)
{
	whatIsIt += " : ";
	whatIsIt += std::to_string(intInput);
	TextOut(hdc, x, y, (LPCTSTR)whatIsIt.c_str(), whatIsIt.size());
}

inline void TextOutfloat(HDC hdc, int x, int y, std::string whatIsIt, float floatInput)
{
	whatIsIt += " : ";
	whatIsIt += std::to_string(floatInput);
	TextOut(hdc, x, y, (LPCTSTR)whatIsIt.c_str(), whatIsIt.size());
}
