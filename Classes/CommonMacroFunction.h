#pragma once

////  ------- WinAPI용 ------- 지금 D2D에선 안쓰기에 주석처리해둠
//
////▼선 그리기
//inline void LineMake(HDC hdc, int startX, int startY, int endX, int endY)
//{
//	MoveToEx(hdc, startX, startY, NULL);
//	LineTo(hdc, endX, endY);
//}
//
////▼렉탱글함수 이용해서 사각 그리기
//inline void RectagleMake(HDC hdc, int x, int y, int width, int height)
//{
//	Rectangle(hdc, x, y, x + width, y + height);
//}
//
////▼일립스함수 이용해서 타원 그리기
//inline void EllipseMake(HDC hdc, int x, int y, int width, int height)
//{
//	Ellipse(hdc, x, y, x + width, y + height);
//}
//
////▼일립스함수 이용해서 센터점에 타원 그리기
//inline void EllipseMakeCenter(HDC hdc, int x, int y, int width, int height)
//{
//	Ellipse(hdc, x - width / 2, y - height / 2, x + width / 2, y + height / 2);
//}
//
////렉트 던지면 그림
//inline void Rectangle(HDC hdc, RECT rc)
//{
//	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
//}
//inline void Ellipse(HDC hdc, RECT rc)
//{
//	Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
//}
//
////프레임렉트 함수 오버로딩(비어있는 렉트 만들기)
//inline void FrameRect(HDC hdc, RECT rc, COLORREF color)
//{
//	HBRUSH brush = CreateSolidBrush(color);
//	FrameRect(hdc, &rc, brush);
//	DeleteObject(brush);
//}
//
//inline void TextOutInt(HDC hdc, int x, int y, std::string whatIsIt, int intInput)
//{
//	whatIsIt += " : ";
//	whatIsIt += std::to_string(intInput);
//	TextOut(hdc, x, y, (LPCTSTR)whatIsIt.c_str(), whatIsIt.size());
//}
//
//inline void TextOutfloat(HDC hdc, int x, int y, std::string whatIsIt, float floatInput)
//{
//	whatIsIt += " : ";
//	whatIsIt += std::to_string(floatInput);
//	TextOut(hdc, x, y, (LPCTSTR)whatIsIt.c_str(), whatIsIt.size());
//}



// ------- 통상 -------

//▼포인트 두개가 같은지 참거짓 반환
inline BOOL PointCompare(POINT _pt1, POINT _pt2)
{
	return (_pt1.x == _pt2.x) && (_pt1.y == _pt2.y); //같으면 참 반환
}


//▼RECT 중앙점 반환
inline POINT GetRectCentre(RECT _rect)
{
	return { _rect.left + (_rect.right - _rect.left) / 2, _rect.top + (_rect.bottom - _rect.top) / 2 };
}

//▼RECT 만들기
inline RECT RectMake(int x, int y, int width, int height)
{
	RECT rc = { x, y, x + width, y + height };
	return rc;
}

//▼RECT 중심좌표에 만들기
inline RECT RectMakeCentre(int x, int y, int width, int height)
{
	RECT rc = { x - width / 2, y - height / 2, x + width / 2, y + height / 2 };
	return rc;
}

//▼렉트 뒤집기
inline RECT RectMakePivot(POINTFLOAT pos, POINTFLOAT size, Pivot pivot)
{
	RECT result = {0};
	switch (pivot)
	{
	case Pivot::LeftTop:
		result.left = pos.x;
		result.top = pos.y;
		result.right = pos.x + size.x;
		result.bottom = pos.y + size.y;
		return result;
	case Pivot::Centre:
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

//▼ 두 점 사이 각을 구함
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

//▼ 두 점 사이 각을 구함. 포인트 버전
inline float GetAngle(POINT _start, POINT _end)
{
	float x = _end.x - _start.x;
	float y = _end.y - _start.y;

	float distance = sqrtf((x * x) + (y * y));

	float angle = acos(x / distance);

	if (_end.y > _start.y)
	{
		angle = PI * 2.0f - angle;
		if (angle >= PI * 2.0f)
			angle -= PI * 2.0f;
	}

	return angle;
}

//▼ 두 점 사이 거리 구함
inline float GetDistance(float x1, float y1, float x2, float y2)
{
	float x = x2 - x1;
	float y = y2 - y1;

	float distance = sqrtf((x * x) + (y * y));

	return  distance;
}

//▼ 두 점 사이 거리 구함. 포인트버전
inline float GetDistance(POINT _first,POINT _second)
{
	float x = static_cast<float>(_second.x) - static_cast<float>(_first.x);
	float y = static_cast<float>(_second.y) - static_cast<float>(_first.y);

	float distance = sqrtf((x * x) + (y * y));

	return  distance;
}

//▼ 렉트 넓이
inline LONG GetWidth(RECT rc)
{
	return rc.right - rc.left;
}

// ▼렉트 높이
inline LONG GetHeight(RECT rc)
{
	return rc.bottom - rc.top;
}

//▼원 원 충돌
inline bool Ellipse_Ellipse_Collision(RECT round1, RECT round2)
{
	float c1x = (float)round1.left + (float)GetWidth(round1) / 2;
	float c1y = (float)round1.top + (float)GetHeight(round1) / 2;
	float c2x = (float)round2.left + (float)GetWidth(round2) / 2;
	float c2y = (float)round2.top + (float)GetHeight(round2) / 2;
	float deltaX = c1x - c2x;
	float deltaY = c1y - c2y;
	float length = sqrt(deltaX*deltaX + deltaY * deltaY);

	if (length > ((float)GetWidth(round1) / 2 + (float)GetWidth(round2) / 2)) return false;

	return true;
}

//▼ 랜덤값 정수형
inline int RandomRangeInt(int fromNum, int toNum)
{
	return rand() % (toNum - fromNum + 1) + fromNum;
}

//▼ 랜덤값 실수형
inline float RandomRangeFloat(float fromNum, float toNum)
{
	float rnd = (float)rand() / (float)RAND_MAX;
	return (rnd * (toNum - fromNum) + fromNum);
}

//▼ 2차원배열 인덱스 1차원배열 인덱스로 바꿔줌
inline std::string TwoDimentionArrayToOneString(POINT _twoDimention, int _tileCOLX)
{
	return std::to_string(_twoDimention.y * _tileCOLX + _twoDimention.x);
}

inline POINT IndexToLeftTop(POINT _index, int _tileSize)
{
	return { _index.x * _tileSize, _index.y * _tileSize };
}

inline POINT RectLeftTopOnly(RECT _position)
{
	return { _position.left, _position.top };
}

inline float ToRadian(const float& _degree)
{
	return _degree * 3.141592f / 180.0f;
}

inline float ToDegree(const float& _radian)
{
	return _radian * 180.0f / 3.141592f;
}

//▼ 두 점 사이 각을 구함. 포인트 버전, 디그리
inline float GetAngleDegree(POINT _start, POINT _end)
{
	float x = _end.x - _start.x;
	float y = _end.y - _start.y;

	float distance = sqrtf((x * x) + (y * y));

	float angle = acos(x / distance);

	if (_end.y > _start.y)
	{
		angle = PI * 2.0f - angle;
		if (angle >= PI * 2.0f)
			angle -= PI * 2.0f;
	}

	return floor(ToDegree(angle) + 0.5f);
}

//▼렉트 움직인거 반환하는 인라인들
inline RECT moveLeft(RECT _toMove, INT _amount)
{return{_toMove.left - _amount,_toMove.top,_toMove.right - _amount ,_toMove.bottom };}
inline RECT moveRight(RECT _toMove, INT _amount)
{return{ _toMove.left + _amount,_toMove.top,_toMove.right + _amount ,_toMove.bottom };}
inline RECT moveUp(RECT _toMove, INT _amount)
{return{ _toMove.left,_toMove.top - _amount,_toMove.right ,_toMove.bottom - _amount };}
inline RECT moveDown(RECT _toMove, INT _amount)
{return{ _toMove.left,_toMove.top + _amount,_toMove.right ,_toMove.bottom + _amount };}
inline RECT moveDownLeft(RECT _toMove, INT _amount)
{return{ _toMove.left- _amount,_toMove.top + _amount,_toMove.right-_amount ,_toMove.bottom + _amount };}
inline RECT moveDownRight(RECT _toMove, INT _amount)
{return{ _toMove.left+ _amount,_toMove.top + _amount,_toMove.right+_amount ,_toMove.bottom + _amount };}
inline RECT moveUpLeft(RECT _toMove, INT _amount)
{return{ _toMove.left- _amount,_toMove.top - _amount,_toMove.right- _amount ,_toMove.bottom - _amount };}
inline RECT moveUpRight(RECT _toMove, INT _amount)
{return{ _toMove.left+ _amount,_toMove.top - _amount,_toMove.right+ _amount ,_toMove.bottom - _amount };}