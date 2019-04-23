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
			field[j * TILECOLX + i].Init(); //������ Ÿ�� �ʱ�ȭ
			//������ �ҷ������ ��ü
			field[j * TILECOLX + i].SetPosition(RectMake(i * TILESIZE, j * TILESIZE, TILESIZE, TILESIZE));
			field[j * TILECOLX + i].SetIndex({ i,j });
			if ((i == 2 && j == 4) || (i == 5 && j == 4) || (i == 6 && j == 5))
			{
				field[j * TILECOLX + i].SetObj("tmp");
			}
			DATACENTRE.AddObj(ObjType::Tile, std::to_string(j * TILECOLX + i), &field[j * TILECOLX + i]);
		}
	}

	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor")); //UI�� ���� Ŀ�� ���

	clippedTiles.clear();
}

void TileManager::Update()
{
	clippedTiles.clear(); //������Ʈ �� ������ �ϴ� Ŭ���� �� Ÿ�� ����� ���
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
	DATACENTRE.ClearObjects(ObjType::Tile); //������ ���ͼ� ����ϴ� Ÿ�� �� �� ���
	for (int i = 0; i < TILEROWY * TILECOLX; i++) //���⼭ �̴ֶ� ���� ��� Ÿ�ϵ� ���鼭 ������
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
	for (int i = 0; i < TILEROWY * TILECOLX; i++) //�ϴ� ��� Ÿ���� ���鼭,
	{
		if (IntersectRect(&tempC, &CAMERA.GetCameraRc(), &field[i].GetPosition())) //ù��°�� ī�޶�� �΋H�� ��Ʈ�� ã�Ƴ�
		{
			RECT toCalculate = field[i].GetPosition();	//�΋H�� Ÿ�� ������ ����ϱ� ���ϰ� ��Ƶ�
			clippedIndex = field[i].GetIndex();			//�΋H�� Ÿ���� �ε����� ������
			RECT tempCam = CAMERA.GetCameraRc();		//ī�޶� ���������� �ӽ÷� ���ϰ� �����ص�

			//if (tempCam.left < toCalculate.left)		//ī�޶��� ������ ù° Ÿ���� �������� ������, �� ī�޶� ���� ���϶�
			//{
			//	clippedX = ceil(static_cast<float>(tempCam.right - toCalculate.left) / static_cast<float>(TILESIZE));
			//}
			if (tempCam.right > TILECOLX * TILESIZE) //ī�޶��� ������ ��� Ÿ�� ���ΰ������� Ŭ��. �� ī�޶� �������϶�
			{
				clippedX = ceil(static_cast<float>((TILECOLX * TILESIZE) - toCalculate.left) / static_cast<float>(TILESIZE));
			}
			else //ī�޶� �߰� �����϶�
			{
				clippedX = ceil(static_cast<float>(tempCam.right - toCalculate.left) / static_cast<float>(TILESIZE));
			}

			//if (tempCam.top < toCalculate.top) //ī�޶��� ž�� ù° Ÿ���� ž���� ������, �� ī�޶� �� ���϶�
			//{
			//	clippedY = ceil(static_cast<float>(tempCam.bottom - toCalculate.top) / static_cast<float>(TILESIZE));
			//}
			if (tempCam.bottom > TILEROWY * TILESIZE) //ī�޶��� ������ �� Ÿ�� ���ΰ������� Ŭ��, �� ī�޶� �� �Ʒ��϶�
			{
				clippedY = ceil(static_cast<float>((TILEROWY * TILESIZE) - toCalculate.top) / static_cast<float>(TILESIZE));
			}
			else //ī�޶� �߰� �����϶�
			{
				clippedY = ceil(static_cast<float>(tempCam.bottom - toCalculate.top) / static_cast<float>(TILESIZE));
			}
			break; //�ش� ���� �ѹ��� �ؼ� TempX�� TempY�� 
		}
	}

	//��˾Ƴ� X,Y�� �������� �ش��ϴ� Ÿ�ϵ� 
	for (UINT j = clippedIndex.y; j < clippedIndex.y + clippedY; j++) //Ŭ���� ������ �ε�������, Ŭ����Y����
	{
		for (UINT i = clippedIndex.x; i < clippedIndex.x + clippedX; i++) //Ŭ���� ������ �ε�������, Ŭ����X����
		{
			clippedTiles.push_back(&field[j * TILECOLX + i]); //������ �������
		}
	}
}