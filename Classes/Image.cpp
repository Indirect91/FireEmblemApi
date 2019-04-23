#include "../stdafx.h"
#include "Image.h"

//▼일반 이미지
Image::Image(ID2D1Bitmap * const bitmap, const TagLoadedImageInfo & loadinfo)
	:mBitmap(bitmap), mLoadInfo(loadinfo), mScale(1.f), mAlpha(1.f), mAngle(0.f), mMaxFrameX(1), mMaxFrameY(1)
{
	mIsReverseAxisX = false;
	mIsReverseAxisY = false;
	this->mSize.x = (float)this->mBitmap->GetPixelSize().width;
	this->mSize.y = (float)this->mBitmap->GetPixelSize().height;

	FrameRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = mSize.x;
	rc.height = mSize.y;
	this->mFrameInfo.push_back(rc);

}

//▼프레임 이미지
Image::Image(ID2D1Bitmap * bitmap, const TagLoadedImageInfo & loadinfo, const int & maxFrameX, const int & maxFrameY)
	:mBitmap(bitmap), mLoadInfo(loadinfo), mMaxFrameX(maxFrameX), mMaxFrameY(maxFrameY), mScale(1.f), mAlpha(1.f), mAngle(0.f)
{
	mIsReverseAxisX = false;
	mIsReverseAxisY = false;
	this->mSize.x = (float)mBitmap->GetPixelSize().width;
	this->mSize.y = (float)mBitmap->GetPixelSize().height;

	float frameX = mSize.x / (float)this->mMaxFrameX;
	float frameY = mSize.y / (float)this->mMaxFrameY;

	FrameRect rc;
	for (int j = 0; j < maxFrameY; ++j)
	{
		for (int i = 0; i < maxFrameX; ++i)
		{
			rc.x = (float)i * (frameX);
			rc.y = (float)j * (frameY);
			rc.width = frameX;
			rc.height = frameY;
			this->mFrameInfo.push_back(rc);
		}
	}
}

//▼비트맵 해제
Image::~Image()
{
	SAFE_RELEASE(mBitmap);
}

//▼일반적인 랜더
void Image::Render( int x,  int y,  Pivot pivot)
{
	//그릴 사이즈 = 사이즈 * 스케일
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//렌더링 좌표
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);

	RECT renderRc = RectMakePivot(renderPos, mSize, Pivot::LeftTop);
	//크기 정보를 기반으로 scale행렬 구축 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0, 0 };
	//가로로 뒤집을 것인가
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	//세로로 뒤집을 것인가
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//angle정보를 기준으로 회전 행렬 구축 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//위치 정보와 offset값을 기반으로 이동 행렬 구축
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//렌더링 영역 생성 
	D2D1_RECT_F dxArea = D2D1::RectF(0.f, 0.f, mSize.x, mSize.y);
	//이미지가 화면에 그려질 최종 행렬은 크기행렬 * 회전행렬 * 이동행렬 
	//행렬은 교환법칙이 성립이 안되므로 반드시 위의 순서를 맞춰서 진행해 주어야 한다. 
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//렌더링 요청
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha);
	//렌더링이 끝났다면 옵션값 기본으로 세팅
	this->ResetRenderOption();
}

//▼카메라 상대적인곳에 랜더
void Image::RelativeRender(int x, int y, Pivot pivot)
{
	x -= CAMERA.GetCameraRc().left;
	y -= CAMERA.GetCameraRc().top;
	//그릴 사이즈 = 사이즈 * 스케일
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//렌더링 좌표
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);

	RECT renderRc = RectMakePivot(renderPos, mSize, Pivot::LeftTop);
	//크기 정보를 기반으로 scale행렬 구축 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0, 0 };
	//가로로 뒤집을 것인가
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	//세로로 뒤집을 것인가
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//angle정보를 기준으로 회전 행렬 구축 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//위치 정보와 offset값을 기반으로 이동 행렬 구축
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//렌더링 영역 생성 
	D2D1_RECT_F dxArea = D2D1::RectF(0.f, 0.f, mSize.x, mSize.y);
	//이미지가 화면에 그려질 최종 행렬은 크기행렬 * 회전행렬 * 이동행렬 
	//행렬은 교환법칙이 성립이 안되므로 반드시 위의 순서를 맞춰서 진행해 주어야 한다. 
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//렌더링 요청
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha);
	//렌더링이 끝났다면 옵션값 기본으로 세팅
	this->ResetRenderOption();
}

//▼일반적인 프레임 랜더
void Image::FrameRender( int x,  int y,  int frameX,  int frameY,  Pivot pivot)
{
	//프레임이 최대 프레임을 벗어났다면 그리지 않는다. 
	if (frameX >= this->mMaxFrameX || frameY >= this->mMaxFrameY)
	{
		ResetRenderOption();
		return;
	}
	//현재 프레임인덱스 
	int frame = frameY * mMaxFrameX + frameX;
	//사이즈 = 사이즈 * 스케일 
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//그릴 좌표 
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);

	//카메라 클리핑 
	RECT renderRc = RectMakePivot(renderPos, mSize, Pivot::LeftTop);
	
	//크기 행렬 구축 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0.f, 0.f };
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//회전 행렬 구축 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//이동 행렬 구축 
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//그릴 영역 세팅 
	D2D1_RECT_F dxArea = D2D1::RectF(0.0f, 0.0f, mSize.x, mSize.y);
	D2D1_RECT_F dxSrc = D2D1::RectF((float)mFrameInfo[frame].x, (float)mFrameInfo[frame].y,
		(float)(mFrameInfo[frame].x + mFrameInfo[frame].width),
		(float)(mFrameInfo[frame].y + mFrameInfo[frame].height));
	//최종행렬 세팅
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//렌더링 요청
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &dxSrc);

	this->ResetRenderOption();
}

//▼카메라 상대적 프레임랜더
void Image::RelativeFrameRender(int x, int y, int frameX, int frameY, Pivot pivot)
{
	x -= CAMERA.GetCameraRc().left;
	y -= CAMERA.GetCameraRc().top;
	//프레임이 최대 프레임을 벗어났다면 그리지 않는다. 
	if (frameX >= this->mMaxFrameX || frameY >= this->mMaxFrameY)
	{
		ResetRenderOption();
		return;
	}
	//현재 프레임인덱스 
	int frame = frameY * mMaxFrameX + frameX;
	//사이즈 = 사이즈 * 스케일 
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//그릴 좌표 
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);

	//카메라 클리핑 
	RECT renderRc = RectMakePivot(renderPos, mSize, Pivot::LeftTop);

	//크기 행렬 구축 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0.f, 0.f };
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//회전 행렬 구축 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//이동 행렬 구축 
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//그릴 영역 세팅 
	D2D1_RECT_F dxArea = D2D1::RectF(0.0f, 0.0f, mSize.x, mSize.y);
	D2D1_RECT_F dxSrc = D2D1::RectF((float)mFrameInfo[frame].x, (float)mFrameInfo[frame].y,
		(float)(mFrameInfo[frame].x + mFrameInfo[frame].width),
		(float)(mFrameInfo[frame].y + mFrameInfo[frame].height));
	//최종행렬 세팅
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//렌더링 요청
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &dxSrc);

	this->ResetRenderOption();
}

//▼회전에 따라 길이가 같은 기울기 랜더
void Image::SkewRenderRotate(int x, int y,float angleX, float angleY, Pivot pivot)
{
	//그릴 사이즈 = 사이즈 * 스케일
	this->mSize.x = mSize.x * mScale * (cosf(angleY * 3.14 / 180));
	this->mSize.y = mSize.y * mScale* (cosf(angleX * 3.14 / 180));

	//렌더링 좌표
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);

	RECT renderRc = RectMakePivot(renderPos, mSize, Pivot::LeftTop);
	//크기 정보를 기반으로 scale행렬 구축 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0, 0 };
	//가로로 뒤집을 것인가
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	//세로로 뒤집을 것인가
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//angle정보를 기준으로 회전 행렬 구축 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));

	//D2D1::Matrix3x2F skew = D2D1::Matrix3x2F::Skew(angleX, angleY, D2D1::Point2(centerPoint.x, centerPoint.y));
	D2D1::Matrix3x2F skew = D2D1::Matrix3x2F::Skew(angleX, angleY);

	//위치 정보와 offset값을 기반으로 이동 행렬 구축
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//렌더링 영역 생성 
	D2D1_RECT_F dxArea = D2D1::RectF(0.f, 0.f, mSize.x, mSize.y);
	//이미지가 화면에 그려질 최종 행렬은 크기행렬 * 회전행렬 * 이동행렬 
	//행렬은 교환법칙이 성립이 안되므로 반드시 위의 순서를 맞춰서 진행해 주어야 한다. 
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * skew *translation);
	//렌더링 요청
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha);
	//렌더링이 끝났다면 옵션값 기본으로 세팅
	this->ResetRenderOption();
}

//▼회전율과 높이변동 무관한 기울기랜더
void Image::SkewRenderStretch(int x, int y, float angleX, float angleY, Pivot pivot)
{
	//그릴 사이즈 = 사이즈 * 스케일
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;


	//렌더링 좌표
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);

	RECT renderRc = RectMakePivot(renderPos, mSize, Pivot::LeftTop);
	//크기 정보를 기반으로 scale행렬 구축 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0, 0 };
	//가로로 뒤집을 것인가
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	//세로로 뒤집을 것인가
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//angle정보를 기준으로 회전 행렬 구축 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));

	//D2D1::Matrix3x2F skew = D2D1::Matrix3x2F::Skew(angleX, angleY, D2D1::Point2(centerPoint.x, centerPoint.y));
	D2D1::Matrix3x2F skew = D2D1::Matrix3x2F::Skew(angleX, angleY);

	//위치 정보와 offset값을 기반으로 이동 행렬 구축
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//렌더링 영역 생성 
	D2D1_RECT_F dxArea = D2D1::RectF(0.f, 0.f, mSize.x, mSize.y);
	//이미지가 화면에 그려질 최종 행렬은 크기행렬 * 회전행렬 * 이동행렬 
	//행렬은 교환법칙이 성립이 안되므로 반드시 위의 순서를 맞춰서 진행해 주어야 한다. 
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * skew *translation);
	//렌더링 요청
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha);
	//렌더링이 끝났다면 옵션값 기본으로 세팅
	this->ResetRenderOption();
}

//▼이미지 옵션 초기화
void Image::ResetRenderOption()
{
	this->mAlpha = 1.0f;
	this->mScale = 1.0f;
	this->mSize.x = (float)mBitmap->GetPixelSize().width;
	this->mSize.y = (float)mBitmap->GetPixelSize().height;
	this->mIsReverseAxisX = false;
	this->mIsReverseAxisY = false;
}

//▼들어온 피봇에 따라 랜더 위치 돌림
const POINTFLOAT Image::GetPivotPosition( int x,  int  y, Pivot pivot)
{
	if (pivot == Pivot::LeftTop)
		return { (FLOAT)x, (FLOAT)y };

	POINTFLOAT pos = { x, y };

	switch (pivot)
	{
	case Pivot::Centre:
		pos.x -= mSize.x * mScale * 0.5f;
		pos.y -= mSize.y * mScale * 0.5f;
		break;

	case Pivot::Bottom:
		pos.x -= mSize.x * mScale * 0.5f;
		pos.y -= mSize.y * mScale;
		break;
	}

	return pos;
}
