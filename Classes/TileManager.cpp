#include "../stdafx.h"
#include "TileManager.h"


//��Ÿ�� �Ŵ����� ���ʿ� �����ɋ� �׳� �ʱ�ȭ���� ������
TileManager::TileManager()
{
	clippedIndex = { 0 };
	clippedX = 0;
	clippedY = 0;

	field = nullptr;
	field = new Tiles[TILECOLX * TILEROWY];
}

//��Ÿ�ϸŴ��� �̴�
void TileManager::Init()
{
	HANDLE file;
	DWORD read;
	
	file = CreateFile(L"saveFile.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	assert(ReadFile(file, toLoad, sizeof(Tiles) * TILECOLX * TILEROWY, &read, NULL));
	CloseHandle(file);

	for (UINT i = 0; i < TILECOLX * TILEROWY; i++)
	{
		field[i] = toLoad[i];
	}


	for (int j = 0; j < TILEROWY; j++)
	{
		for (int i = 0; i < TILECOLX; i++)
		{
		//��4���� �̿� �߰�
		if (i > 0) //�� ���� ����ó��
			field[j * TILECOLX + i].RefNeighbours().push_back(&field[(j + 0) * TILECOLX + (i - 1)]);
		if (i < TILECOLX - 1) //�� ���� ����ó��
			field[j * TILECOLX + i].RefNeighbours().push_back(&field[(j + 0) * TILECOLX + (i + 1)]);
		if (j > 0) //���κ� ����ó��
			field[j * TILECOLX + i].RefNeighbours().push_back(&field[(j - 1) * TILECOLX + (i + 0)]);
		if (j < TILEROWY - 1) // �ٴ� ����ó��
			field[j * TILECOLX + i].RefNeighbours().push_back(&field[(j + 1) * TILECOLX + (i + 0)]);
		DATACENTRE.AddObj(ObjType::Tile, std::to_string(j * TILECOLX + i), &field[j * TILECOLX + i]);
		}
	}
}

//�������Ʈ�� ���� Ŭ������ �ǽ���.
void TileManager::Update()
{	
	ClipTiles(); //Ÿ�� Ŭ���� ������ �����
	UpdateClippedTiles(); //������ ���� ��� �ش�Ÿ�� ������Ŵ



	//��������ī����
	counter++;
	if (counter % 5 == 0)
	{
		movingTilex++;
		if (movingTilex > 14) movingTilex = 0;
	}
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

//��Ÿ�� Ŭ���� �ϴºκ�
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
		D2DRENDERER->DrawRectangle(CAMERA.RelativeCameraRect(toExamen->GetPosition()), D2DRenderer::DefaultBrush::Black, 1);

		if (toExamen->GetMovingT() != "")
		{
			IMAGEMANAGER->FindImage(toExamen->GetMovingT())->SetSize(IMAGEMANAGER->FindImage(toExamen->GetMovingT())->GetFrameSize());
			IMAGEMANAGER->FindImage(toExamen->GetMovingT())->RelativeFrameRender(toExamen->GetPosition().left, toExamen->GetPosition().top, movingTilex, 0);
		}
		if (toExamen->GetTerrain() != "")
		{
			IMAGEMANAGER->FindImage(toExamen->GetTerrain())->SetSize(IMAGEMANAGER->FindImage(toExamen->GetTerrain())->GetFrameSize());
			IMAGEMANAGER->FindImage(toExamen->GetTerrain())->RelativeFrameRender(toExamen->GetPosition().left, toExamen->GetPosition().top, toExamen->GetTerrainFrame().x, toExamen->GetTerrainFrame().y);
		}
		if (toExamen->GetObjT() != "")
		{
			IMAGEMANAGER->FindImage(toExamen->GetObjT())->SetSize(IMAGEMANAGER->FindImage(toExamen->GetObjT())->GetFrameSize());
			IMAGEMANAGER->FindImage(toExamen->GetObjT())->RelativeFrameRender(toExamen->GetPosition().left, toExamen->GetPosition().top, toExamen->GetObjTFrame().x, toExamen->GetObjTFrame().y);
		}

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
		if (toExamen->GetArrowT() != "")
		{
			auto arrow = IMAGEMANAGER->FindImage("MoveArrow");
			arrow->SetSize({ TILESIZE,TILESIZE });
			arrow->RelativeFrameRender(toExamen->GetPosition().left, toExamen->GetPosition().top, toExamen->GetArrowFrame().x, toExamen->GetArrowFrame().y);
		}

	}
}