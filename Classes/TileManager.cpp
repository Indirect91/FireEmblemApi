#include "../stdafx.h"
#include "TileManager.h"
#include "Tiles.h"
#include "Cursor.h"

TileManager::TileManager()
{
	field = nullptr;
	cursor = nullptr;
	clippedIndex = { 0 };
	clippedX = 0;
	clippedY = 0;
}

void TileManager::Init()
{
	field = new Tiles[TILECOLX * TILEROWY];

	for (int j = 0; j < TILEROWY; j++)
	{
		for (int i = 0; i < TILECOLX; i++)
		{
			field[j * TILECOLX + i].Init(); //생성된 타일 초기화
			//▼추후 불러오기로 대체
			field[j * TILECOLX + i].SetPosition(RectMake(i * TILESIZE, j * TILESIZE, TILESIZE, TILESIZE));
			field[j * TILECOLX + i].SetIndex({ i,j });
			if ((i == 2 && j == 4) || (i == 5 && j == 4) || (i == 6 && j == 5))
			{
				field[j * TILECOLX + i].SetObj("tmp");
			}
			DATACENTRE.AddObj(ObjType::Tile, std::to_string(j * TILECOLX + i), &field[j * TILECOLX + i]);
		}
	}

	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor")); //UI서 만든 커서 등록

	clippedTiles.clear();
}

void TileManager::Update()
{
	clippedTiles.clear(); //업데이트 돌 때마다 일단 클리핑 된 타일 목록을 비움
	ClipTiles();
	

	for (int i = 0; i < TILEROWY * TILECOLX; i++)
	{
		if (PtInRect(&field[i].GetPosition(), _ptMouse))
		{
			cursor->SetCursorVisibility(true);
			cursor->SetIndex(field[i].GetIndex());
			break;
		}
		else
		{
			cursor->SetCursorVisibility(false);
		}
	}
}

void TileManager::Release()
{
	DATACENTRE.ClearObjects(ObjType::Tile); //데이터 센터서 사용하던 타일 싹 다 비움
	for (int i = 0; i < TILEROWY * TILECOLX; i++) //여기서 이닛때 만든 모든 타일들 돌면서 릴리즈
	{field[i].Release();}
	SAFE_DELETE_ARRAY(field);
}

void TileManager::Render()
{
	//for (int i = 0; i < TILEROWY * TILECOLX; i++)
	//{
	//	if (IntersectRect(&tempC, &field[i].GetPosition(), &CAMERA.GetCameraRc()))
	//	{
	//		D2DRENDERER->DrawRectangle(CAMERA.RelativeCameraRect((field[i].GetPosition())), D2DRenderer::DefaultBrush::White, 2);
	//		
	//		if (field[i].GetIsBlue())
	//		{
	//			//D2DRENDERER->FillRectangle(arrField[i].GetPosition(), D2D1::ColorF::Blue, arrField[i].GetBlueAlpha());
	//			IMAGEMANAGER->FindImage("Blue")->SetAlpha(field[i].GetBlueAlpha());
	//			IMAGEMANAGER->FindImage("Blue")->SetSize({ 48,48 });
	//			IMAGEMANAGER->FindImage("Blue")->Render(field[i].GetPosition().left, field[i].GetPosition().top);
	//		}
	//		else if (field[i].GetObj() != "")
	//		{
	//			D2DRENDERER->FillRectangle(field[i].GetPosition(), D2D1::ColorF::Red, field[i].GetBlueAlpha());
	//		}
	//	}
	//}

	for (auto& toRender : clippedTiles)
	{
		D2DRENDERER->DrawRectangle(CAMERA.RelativeCameraRect(toRender->GetPosition()), D2DRenderer::DefaultBrush::White, 1);
	}

	D2DRENDERER->RenderText(10, 200, std::to_wstring(clippedX),20);
	D2DRENDERER->RenderText(10, 240, std::to_wstring(clippedY),20);


	D2DRENDERER->DrawRectangle(CAMERA.RelativeCameraRect(CAMERA.GetCameraRc()),D2DRenderer::DefaultBrush::Red,1);

}

void TileManager::ClipTiles()
{
	clippedX = 0;
	clippedY = 0;
	for (int i = 0; i < TILEROWY * TILECOLX; i++) //일단 모든 타일을 돌면서,
	{
		if (IntersectRect(&tempC, &CAMERA.GetCameraRc(), &field[i].GetPosition())) //첫번째로 카메라와 부딫힌 렉트를 찾아냄
		{
			RECT toCalculate = field[i].GetPosition();	//부딫힌 타일 정보를 계산하기 편하게 담아둠
			clippedIndex = field[i].GetIndex();			//부딫힌 타일의 인덱스를 저장함
			RECT tempCam = CAMERA.GetCameraRc();		//카메라도 마찬가지로 임시로 편하게 복사해둠

			//if (tempCam.left < toCalculate.left)		//카메라의 좌측이 첫째 타일의 좌측보다 작을때, 즉 카메라가 좌측 끝일때
			//{
			//	clippedX = ceil(static_cast<float>(tempCam.right - toCalculate.left) / static_cast<float>(TILESIZE));
			//}
			if (tempCam.right > TILECOLX * TILESIZE) //카메라의 우측이 모든 타일 가로갯수보다 클때. 즉 카메라가 우측끝일때
			{
				clippedX = ceil(static_cast<float>((TILECOLX * TILESIZE) - toCalculate.left) / static_cast<float>(TILESIZE));
			}
			else //카메라가 중간 조건일때
			{
				clippedX = ceil(static_cast<float>(tempCam.right - toCalculate.left) / static_cast<float>(TILESIZE));
			}

			//if (tempCam.top < toCalculate.top) //카메라의 탑이 첫째 타일의 탑보다 작을때, 즉 카메라가 맨 위일때
			//{
			//	clippedY = ceil(static_cast<float>(tempCam.bottom - toCalculate.top) / static_cast<float>(TILESIZE));
			//}
			if (tempCam.bottom > TILEROWY * TILESIZE) //카메라의 바텀이 총 타일 세로갯수보다 클떄, 즉 카메라가 맨 아래일때
			{
				clippedY = ceil(static_cast<float>((TILEROWY * TILESIZE) - toCalculate.top) / static_cast<float>(TILESIZE));
			}
			else //카메라가 중간 조건일때
			{
				clippedY = ceil(static_cast<float>(tempCam.bottom - toCalculate.top) / static_cast<float>(TILESIZE));
			}
			break; //해당 연산 한번만 해서 TempX와 TempY를 
		}
	}

	//▼알아낸 X,Y를 바탕으로 해당하는 타일들 
	for (UINT j = clippedIndex.y; j < clippedIndex.y + clippedY; j++) //클리핑 시작한 인덱스부터, 클리핑Y까지
	{
		for (UINT i = clippedIndex.x; i < clippedIndex.x + clippedX; i++) //클리핑 시작한 인덱스부터, 클리핑X까지
		{
			clippedTiles.push_back(&field[j * TILECOLX + i]); //대상들을 집어넣음
		}
	}
}