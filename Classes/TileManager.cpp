#include "../stdafx.h"
#include "TileManager.h"
#include "Tiles.h"

//��Ÿ�� �Ŵ����� ���ʿ� �����ɋ� �׳� �ʱ�ȭ���� ������
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
			field[j * TILECOLX + i].Init(); //������ Ÿ�� �ʱ�ȭ
			//������ �ҷ������ ��ü
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

//��Ÿ�ϸŴ��� �̴��� �ؾ� ��μ� ���Ҵ��� �Ͼ��.
void TileManager::Init()
{
	//field = new Tiles[TILECOLX * TILEROWY];
	//
	//for (int j = 0; j < TILEROWY; j++)
	//{
	//	for (int i = 0; i < TILECOLX; i++)
	//	{
	//		field[j * TILECOLX + i].Init(); //������ Ÿ�� �ʱ�ȭ
	//		//������ �ҷ������ ��ü
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

//�������Ʈ�� ���� Ŭ������ �ǽ���.
void TileManager::Update()
{	
	ClipTiles(); //Ÿ�� Ŭ���� ������ �����
	UpdateClippedTiles(); //������ ���� ��� �ش�Ÿ�� ������Ŵ
}

//�帱��� ����Ѱ� ���� ���
void TileManager::Release()
{
	DATACENTRE.ClearObjects(ObjType::Tile); //������ ���ͼ� ����ϴ� Ÿ�� �� �� ���
	DATACENTRE.ClearObjects(ObjType::ClippedTile);

	for (int i = 0; i < TILEROWY * TILECOLX; i++) //���⼭ �̴ֶ� ���� ��� Ÿ�ϵ� ���鼭 ������
	{field[i].Release();}
	SAFE_DELETE_ARRAY(field);
}


void TileManager::ClipTiles()
{
	clippedTiles.clear(); //�ϴ� Ŭ���� �� Ÿ�� ����� ���
	clippedX = 0; //�ε��� �ʱ�ȭ ����
	clippedY = 0; //��������

	for (int i = 0; i < TILEROWY * TILECOLX; i++) //�ϴ� ��� Ÿ���� ���鼭,
	{
		if (IntersectRect(&tempC, &CAMERA.GetCameraRc(), &field[i].GetPosition())) //ù��°�� ī�޶�� �΋H�� ��Ʈ�� ã�Ƴ�
		{
			RECT toCalculate = field[i].GetPosition();	//�΋H�� Ÿ�� ������ ����ϱ� ���ϰ� ��Ƶ�
			clippedIndex = field[i].GetIndex();			//�΋H�� Ÿ���� �ε����� ������
			RECT tempCam = CAMERA.GetCameraRc();		//ī�޶� ���������� �ӽ÷� ���ϰ� �����ص�

			if (tempCam.right > TILECOLX * TILESIZE) //ī�޶��� ������ ��� Ÿ�� ���ΰ������� Ŭ��. �� ī�޶� �������϶�
			{
				clippedX = static_cast<UINT>(ceil(static_cast<float>((TILECOLX * TILESIZE) - toCalculate.left) / static_cast<float>(TILESIZE)));
			}
			else //���� ���� ������ �ƴҶ�
			{
				clippedX = static_cast<UINT>(ceil(static_cast<float>(tempCam.right - toCalculate.left) / static_cast<float>(TILESIZE)));
			}

			if (tempCam.bottom > TILEROWY * TILESIZE) //ī�޶��� ������ �� Ÿ�� ���ΰ������� Ŭ��, �� ī�޶� �� �Ʒ��϶�
			{
				clippedY = static_cast<UINT>(ceil(static_cast<float>((TILEROWY * TILESIZE) - toCalculate.top) / static_cast<float>(TILESIZE)));
			}
			else //�� �� �����϶�
			{
				clippedY = static_cast<UINT>(ceil(static_cast<float>(tempCam.bottom - toCalculate.top) / static_cast<float>(TILESIZE)));
			}
			break;
		}
	}


}

//��Ŭ���ε� Ÿ�ϵ� ���� �� ����
void TileManager::UpdateClippedTiles()
{
	//��˾Ƴ� X,Y�� �������� �ش��ϴ� Ÿ�ϵ� 
	for (UINT j = clippedIndex.y; j < clippedIndex.y + clippedY; j++) //Ŭ���� ������ �ε�������, Ŭ����Y����
	{
		for (UINT i = clippedIndex.x; i < clippedIndex.x + clippedX; i++) //Ŭ���� ������ �ε�������, Ŭ����X����
		{
			field[j * TILECOLX + i].Update(); //Ŭ���ε� Ÿ�ϵ� ������Ʈ
			clippedTiles.insert({ std::to_string(j * TILECOLX + i), &field[j * TILECOLX + i] }); //������ �������
		}
	}
}
  

//��ī�޶� ���� Ÿ�ϵ鸸 ����
void TileManager::Render()
{
	for (auto& toRender : clippedTiles) //Ŭ���ε� Ÿ�ϵ鸸 ���鼭
	{
		Tiles* toExamen = dynamic_cast<Tiles*>(toRender.second); //�˻��� Ÿ�� �ϳ��� �ӽ� �����Ϳ� ����
		D2DRENDERER->DrawRectangle(CAMERA.RelativeCameraRect(toExamen->GetPosition()), D2DRenderer::DefaultBrush::White, 2);

		//1.����Ÿ��
		//2.�ͷ���
		//3.������Ʈ
		//4.ȭ��ǥ
		if (toExamen->GetIsRed())
		{
			IMAGEMANAGER->FindImage("Red")->SetAlpha(toExamen->GetRedAlpha()); //���� ���İ� �����ͼ�
			IMAGEMANAGER->FindImage("Red")->SetSize({ TILESIZE,TILESIZE }); //������ �������ְ�
			IMAGEMANAGER->FindImage("Red")->RelativeRender(toExamen->GetPosition().left, toExamen->GetPosition().top);//��� 
		}

		else if (toExamen->GetIsBlue()) //�Ķ�Ÿ���ϰ��
		{
			IMAGEMANAGER->FindImage("Blue")->SetAlpha(toExamen->GetBlueAlpha()); //�Ķ� ���İ� �����ͼ�
			IMAGEMANAGER->FindImage("Blue")->SetSize({ TILESIZE,TILESIZE }); //������ �������ְ�
			IMAGEMANAGER->FindImage("Blue")->RelativeRender(toExamen->GetPosition().left, toExamen->GetPosition().top);//���
		}
		else if (toExamen->GetIsGreen()) //�ʷ�Ÿ���ϰ��
		{
			IMAGEMANAGER->FindImage("Green")->SetAlpha(toExamen->GetBlueAlpha()); //�Ķ� ���İ� �����ͼ�
			IMAGEMANAGER->FindImage("Green")->SetSize({ TILESIZE,TILESIZE }); //������ �������ְ�
			IMAGEMANAGER->FindImage("Green")->RelativeRender(toExamen->GetPosition().left, toExamen->GetPosition().top);//���
		}
		else if (toExamen->GetIsPurple()) //����Ÿ���ϰ��
		{
			IMAGEMANAGER->FindImage("Purple")->SetAlpha(toExamen->GetPurpleAlpha()); //�Ķ� ���İ� �����ͼ�
			IMAGEMANAGER->FindImage("Purple")->SetSize({ TILESIZE,TILESIZE }); //������ �������ְ�
			IMAGEMANAGER->FindImage("Purple")->RelativeRender(toExamen->GetPosition().left, toExamen->GetPosition().top);//���
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