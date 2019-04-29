#include "../stdafx.h"
#include "TileManager.h"
#include "Tiles.h"

//▼타일 매니져는 태초에 생성될떈 그냥 초기화값만 가진다
TileManager::TileManager()
{
	field = nullptr;
	clippedIndex = { 0 };
	clippedX = 0;
	clippedY = 0;

	field = new Tiles[TILECOLX * TILEROWY];

	for (int j = 0; j < TILEROWY; j++)
	{
		for (int i = 0; i < TILECOLX; i++)
		{
			field[j * TILECOLX + i].Init(); //생성된 타일 초기화
			//▼추후 불러오기로 대체
			field[j * TILECOLX + i].SetPosition(RectMake(i * TILESIZE, j * TILESIZE, TILESIZE, TILESIZE));
			field[j * TILECOLX + i].SetIndex({ i,j });
			field[j * TILECOLX + i].SetPositionViaIndex();
			if ((i == 2 && j == 4) || (i == 5 && j == 4) || (i == 6 && j == 5))
			{
				field[j * TILECOLX + i].SetObjT("tmp");
			}
			DATACENTRE.AddObj(ObjType::Tile, std::to_string(j * TILECOLX + i), &field[j * TILECOLX + i]);
		}
	}

}

//▼타일매니져 이닛을 해야 비로소 뉴할당이 일어난다.
void TileManager::Init()
{
	//field = new Tiles[TILECOLX * TILEROWY];
	//
	//for (int j = 0; j < TILEROWY; j++)
	//{
	//	for (int i = 0; i < TILECOLX; i++)
	//	{
	//		field[j * TILECOLX + i].Init(); //생성된 타일 초기화
	//		//▼추후 불러오기로 대체
	//		field[j * TILECOLX + i].SetPosition(RectMake(i * TILESIZE, j * TILESIZE, TILESIZE, TILESIZE));
	//		field[j * TILECOLX + i].SetIndex({ i,j });
	//		if ((i == 2 && j == 4) || (i == 5 && j == 4) || (i == 6 && j == 5))
	//		{
	//			field[j * TILECOLX + i].SetObjT("tmp");
	//		}
	//		DATACENTRE.AddObj(ObjType::Tile, std::to_string(j * TILECOLX + i), &field[j * TILECOLX + i]);
	//	}
	//}
}

//▼업데이트는 보통 클리핑을 실시함.
void TileManager::Update()
{	
	ClipTiles(); //타일 클리핑 범위를 계산함
	UpdateClippedTiles(); //범위에 따라 담고 해당타일 업뎃시킴
}

//▼릴리즈땐 사용한거 전부 비움
void TileManager::Release()
{
	DATACENTRE.ClearObjects(ObjType::Tile); //데이터 센터서 사용하던 타일 싹 다 비움
	DATACENTRE.ClearObjects(ObjType::ClippedTile);

	for (int i = 0; i < TILEROWY * TILECOLX; i++) //여기서 이닛때 만든 모든 타일들 돌면서 릴리즈
	{field[i].Release();}
	SAFE_DELETE_ARRAY(field);
}


void TileManager::ClipTiles()
{
	clippedTiles.clear(); //일단 클리핑 된 타일 목록을 비움
	clippedX = 0; //인덱스 초기화 해줌
	clippedY = 0; //마찬가지

	for (int i = 0; i < TILEROWY * TILECOLX; i++) //일단 모든 타일을 돌면서,
	{
		if (IntersectRect(&tempC, &CAMERA.GetCameraRc(), &field[i].GetPosition())) //첫번째로 카메라와 부딫힌 렉트를 찾아냄
		{
			RECT toCalculate = field[i].GetPosition();	//부딫힌 타일 정보를 계산하기 편하게 담아둠
			clippedIndex = field[i].GetIndex();			//부딫힌 타일의 인덱스를 저장함
			RECT tempCam = CAMERA.GetCameraRc();		//카메라도 마찬가지로 임시로 편하게 복사해둠

			if (tempCam.right > TILECOLX * TILESIZE) //카메라의 우측이 모든 타일 가로갯수보다 클때. 즉 카메라가 우측끝일때
			{
				clippedX = static_cast<UINT>(ceil(static_cast<float>((TILECOLX * TILESIZE) - toCalculate.left) / static_cast<float>(TILESIZE)));
			}
			else //우측 끝인 조건이 아닐땐
			{
				clippedX = static_cast<UINT>(ceil(static_cast<float>(tempCam.right - toCalculate.left) / static_cast<float>(TILESIZE)));
			}

			if (tempCam.bottom > TILEROWY * TILESIZE) //카메라의 바텀이 총 타일 세로갯수보다 클떄, 즉 카메라가 맨 아래일때
			{
				clippedY = static_cast<UINT>(ceil(static_cast<float>((TILEROWY * TILESIZE) - toCalculate.top) / static_cast<float>(TILESIZE)));
			}
			else //이 외 조건일때
			{
				clippedY = static_cast<UINT>(ceil(static_cast<float>(tempCam.bottom - toCalculate.top) / static_cast<float>(TILESIZE)));
			}
			break;
		}
	}


}

//▼클리핑된 타일들 저장 및 업뎃
void TileManager::UpdateClippedTiles()
{
	//▼알아낸 X,Y를 바탕으로 해당하는 타일들 
	for (UINT j = clippedIndex.y; j < clippedIndex.y + clippedY; j++) //클리핑 시작한 인덱스부터, 클리핑Y까지
	{
		for (UINT i = clippedIndex.x; i < clippedIndex.x + clippedX; i++) //클리핑 시작한 인덱스부터, 클리핑X까지
		{
			field[j * TILECOLX + i].Update(); //클리핑된 타일들 업데이트
			clippedTiles.insert({ std::to_string(j * TILECOLX + i), &field[j * TILECOLX + i] }); //대상들을 집어넣음
		}
	}
}
  

//▼카메라에 들어온 타일들만 굴림
void TileManager::Render()
{
	for (auto& toRender : clippedTiles) //클리핑된 타일들만 돌면서
	{
		Tiles* toExamen = dynamic_cast<Tiles*>(toRender.second); //검사할 타일 하나를 임시 포인터에 저장
		D2DRENDERER->DrawRectangle(CAMERA.RelativeCameraRect(toExamen->GetPosition()), D2DRenderer::DefaultBrush::White, 2);

		//1.무빙타일
		//2.터레인
		//3.오브젝트
		//4.화살표
		if (toExamen->GetIsRed())
		{
			IMAGEMANAGER->FindImage("Red")->SetAlpha(toExamen->GetRedAlpha()); //빨간 알파값 가져와서
			IMAGEMANAGER->FindImage("Red")->SetSize({ TILESIZE,TILESIZE }); //사이즈 세팅해주고
			IMAGEMANAGER->FindImage("Red")->RelativeRender(toExamen->GetPosition().left, toExamen->GetPosition().top);//출력 
		}

		else if (toExamen->GetIsBlue()) //파란타일일경우
		{
			IMAGEMANAGER->FindImage("Blue")->SetAlpha(toExamen->GetBlueAlpha()); //파란 알파값 가져와서
			IMAGEMANAGER->FindImage("Blue")->SetSize({ TILESIZE,TILESIZE }); //사이즈 세팅해주고
			IMAGEMANAGER->FindImage("Blue")->RelativeRender(toExamen->GetPosition().left, toExamen->GetPosition().top);//출력
		}
		else if (toExamen->GetIsGreen()) //초록타일일경우
		{
			IMAGEMANAGER->FindImage("Green")->SetAlpha(toExamen->GetBlueAlpha()); //파란 알파값 가져와서
			IMAGEMANAGER->FindImage("Green")->SetSize({ TILESIZE,TILESIZE }); //사이즈 세팅해주고
			IMAGEMANAGER->FindImage("Green")->RelativeRender(toExamen->GetPosition().left, toExamen->GetPosition().top);//출력
		}
		else if (toExamen->GetIsPurple()) //퍼플타일일경우
		{
			IMAGEMANAGER->FindImage("Purple")->SetAlpha(toExamen->GetPurpleAlpha()); //파란 알파값 가져와서
			IMAGEMANAGER->FindImage("Purple")->SetSize({ TILESIZE,TILESIZE }); //사이즈 세팅해주고
			IMAGEMANAGER->FindImage("Purple")->RelativeRender(toExamen->GetPosition().left, toExamen->GetPosition().top);//출력
		}
		//else if (toExamen->GetObjT() != "")
		//{
		//	D2DRENDERER->FillRectangle(CAMERA.RelativeCameraRect(toExamen->GetPosition()), D2D1::ColorF::Red, toExamen->GetBlueAlpha());
		//}
		if (toExamen->GetArrowT() != "")
		{
			auto arrow = IMAGEMANAGER->FindImage("MoveArrow");
			arrow->SetSize({ TILESIZE,TILESIZE });
			arrow->RelativeFrameRender(toExamen->GetPosition().left, toExamen->GetPosition().top, toExamen->GetArrowFrame().x, toExamen->GetArrowFrame().y);
		}

	}

	//D2DRENDERER->DrawRectangle(CAMERA.RelativeCameraRect(CAMERA.GetCameraRc()),D2DRenderer::DefaultBrush::Red,1);

}