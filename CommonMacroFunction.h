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

//선 그리기
inline void LineMake(HDC hdc, int startX, int startY, int endX, int endY)
{
	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, endX, endY);
}

//RECT 만들기
inline RECT RectMake(int x, int y, int width, int height)
{
	RECT rc = { x, y, x + width, y + height };
	return rc;
}

//RECT 중심좌표에 만들기
inline RECT RectMakeCenter(int x, int y, int width, int height)
{
	RECT rc = { x - width / 2, y - height / 2, x + width / 2, y + height / 2 };
	return rc;
}

//렉탱글함수 이용해서 사각 그리기
inline void RectagleMake(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}

//일립스함수 이용해서 타원 그리기
inline void EllipseMake(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x, y, x + width, y + height);
}

//일립스함수 이용해서 센터점에 타원 그리기
inline void EllipseMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}

//귀찮아서 Rectangle()함수 오버로딩으로 만들어둠
inline void Rectangle(HDC hdc, RECT rc)
{
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
}
inline void Ellipse(HDC hdc, RECT rc)
{
	Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
}

//프레임렉트 함수 오버로딩(비어있는 렉트 만들기)
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
