#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init(void)
{
	//�ε�Ŭ���� �ʱ�ȭ
	_loading = new loading;
	_loading->init();

	//�̹��� �� ���� �ε�
	this->loadingImage();
	this->loadingSound();

	return S_OK;
}

void loadingScene::release(void)
{
	//�ε�Ŭ���� ����
	_loading->release();
	SAFE_DELETE(_loading);
}

void loadingScene::update(void)
{
	//�ε�Ŭ���� ������Ʈ
	_loading->update();

	//�ε��Ϸ��� ȭ��(��)����
	if (_loading->loadingDone())
	{
		//SCENEMANAGER->loadScene("BossStage");
		//SCENEMANAGER->loadScene("Stage1");
		SCENEMANAGER->loadScene("Title");
		//SCENEMANAGER->loadScene("Cinema");

	}
}

void loadingScene::render(void)
{
	//�ε�Ŭ���� ����
	float num = _loading->getLoadItem().size();
	float cur = _loading->getCurrentGauge();
	per = cur / num * 100;
	_loading->render(per);

	char str[128];
	sprintf(str, "%.f %%", per);
	SetBkMode(getMemDC(), TRANSPARENT);
	TextOut(getMemDC(), 660, 662, str, strlen(str));

}

void loadingScene::loadingImage()
{
	//�������� 1��
	_loading->loadImage("Stage1", "image/Stage/Stage1_Resized.bmp", 17520, 2436, true, RGB(255, 0, 255));
	_loading->loadImage("Stage1Collision", "image/Stage/Stage1_Collision.bmp", 17520, 2436, true, RGB(255, 0, 255));

	//���� ��������
	_loading->loadImage("BossStage", "image/Stage/Boss_stage_Resized.bmp", 10976, 2436, true, RGB(255, 0, 255));
	_loading->loadImage("BossStageCollision", "image/Stage/Boss_stage_Collision.bmp", 10976, 2436, true, RGB(255, 0, 255));
	//���� ��������
	_loading->loadImage("Village", "image/Stage/Stage ����.bmp", 2750, 768, true, RGB(255, 0, 255));
	_loading->loadImage("VillageCollision", "image/Stage/Stage ���� �ٴ� �ȼ�.bmp", 2750, 768, true, RGB(255, 0, 255));
	_loading->loadImage("Village ���", "image/Stage/Stage ���� ���.bmp", 1366, 768);
	//����� ��
	_loading->loadImage("WorldMap", "image/UI/WorldMap 1366,768.bmp", 1366, 768);
	_loading->loadImage("WorldMapBig", "image/UI/WorldMap 5464,3072.bmp", 5464, 3072);
	_loading->loadImage("WorldMapLow","image/UI/WorldMapLow 1210,72.bmp", 1210, 72, true, RGB(255, 0, 255));
	_loading->loadImage("WorldMapHigh", "image/UI/WorldMapHigh 1212,163.bmp", 1212, 163, true, RGB(255, 0, 255));

	//�ó׸���
	_loading->loadImage("��1", "image/Cinematic/��1.bmp", 1366, 768);
	_loading->loadImage("��2", "image/Cinematic/��2.bmp", 1366, 768);
	_loading->loadImage("��3", "image/Cinematic/��3.bmp", 1366, 768);
	_loading->loadImage("��4", "image/Cinematic/��4.bmp", 1366, 768);
	_loading->loadImage("��5", "image/Cinematic/��5.bmp", 1366, 768);
	_loading->loadImage("��6", "image/Cinematic/��6.bmp", 1366, 768);
	_loading->loadImage("��7", "image/Cinematic/��7.bmp", 1366, 768);
	_loading->loadImage("��8", "image/Cinematic/��8.bmp", 1366, 768);
	_loading->loadImage("��9", "image/Cinematic/��9.bmp", 1366, 768);
	_loading->loadImage("��10", "image/Cinematic/��10.bmp", 1366, 768);
	_loading->loadImage("��11", "image/Cinematic/��11.bmp", 1366, 768);
	_loading->loadImage("Ŀư��", "image/Cinematic/Ŀư��.bmp", 1366, 768);

	//NPC
	_loading->loadFrameImage("NPC-����", "image/NPC/NPC-���� �Ҿƹ���_12��.bmp", 1536, 132, 12, 1, true, RGB(255, 0, 255));
	_loading->loadImage("����Ʈ-��ǳ��", "image/NPC/����Ʈ - ����.bmp", 55, 45, true, RGB(255, 0, 255));

	//Ÿ��Ʋ ��
	_loading->loadImage("titleBg", "image/UI/titleBg.bmp", 1366, 768);
	_loading->loadImage("titleLogo", "image/UI/titleLogo.bmp", 599, 254, true, RGB(255, 0, 255));
	_loading->loadImage("pip1", "image/UI/titlePip1.bmp", 240, 228, true, RGB(255, 0, 255));
	_loading->loadImage("pip2", "image/UI/titlePip2.bmp", 252, 325, true, RGB(255, 0, 255));
	_loading->loadImage("pip3", "image/UI/titlePip3.bmp", 450, 472, true, RGB(255, 0, 255));
	_loading->loadImage("PressEnter", "image/UI/titlePressEnter.bmp", 488, 97, true, RGB(255, 0, 255));
	_loading->loadImage("PressEsc", "image/UI/titlePressEsc.bmp", 217, 63, true, RGB(255, 0, 255));
	
	//���̺� ���� ��ȭ��
	_loading->loadImage("fileSelect", "image/UI/FileSelect(1000,510).bmp", 1000, 510, true, RGB(255, 0, 255));
	_loading->loadImage("loadFile", "image/UI/FileSelectLoadFile(219,266).bmp", 219, 266, true, RGB(255, 0, 255));
	_loading->loadImage("ESC_YSetting", "image/UI/FileSelectESC_YSetting(1368,82).bmp", 1368, 82, true, RGB(255, 0, 255));
	_loading->loadImage("newEmpty", "image/UI/FileSelectnewEmpty(219,266).bmp", 219, 266, true, RGB(255, 0, 255));
	_loading->loadImage("selection", "image/UI/FileSelectSelection (338, 361).bmp", 338, 361, true, RGB(255, 0, 255));

	//�ΰ� ��(�ȼ�)
	_loading->loadFrameImage("pipIdle", "image/Pip/pipidle(49,76) 15��.bmp", 735, 152, 15, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pipAttackLeft", "image/Pip/pipattack(95,75)Left 12��.bmp", 1140, 75, 12, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pipAttackRight", "image/Pip/pipattack(95,75)Right 12��.bmp", 1140, 75, 12, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pipRun", "image/Pip/pipRun (73,71) 12��.bmp", 876, 142, 12, 2, true, RGB(255, 0, 255));
	_loading->loadImage("pipGrabLeft", "image/Pip/pipGrabLeft.bmp", 54, 73, true, RGB(255, 0, 255));
	_loading->loadImage("pipGrabRight", "image/Pip/pipGrabRight.bmp", 54, 73, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pipJumpUp", "image/Pip/�ΰ��� ������� (54,76) 7��.bmp", 378, 152, 7, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pipJumpDown", "image/Pip/�ΰ��� �����϶� (73,82)6��.bmp", 438, 164, 6, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pipDownAttack", "image/Pip/�ΰ��� �����ϴ�(93,184) 11��.bmp", 1023, 184, 11, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pipChangeForm", "image/Pip/������(76,114) 10��.bmp", 570, 152, 10, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("changeEffect", "image/Pip/changeEffect.bmp", 4400, 100, 44, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pipHit", "image/Pip/playerHit1.bmp", 450, 145, 6, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("portl", "2312,308_ ���纻.bmp", 2312, 308, 17, 1, true, RGB(255, 0, 255));

	//���� ��(����)
	_loading->loadFrameImage("jellyStay", "image/Pip/�������(46,37) 8��.bmp", 368, 37, 8, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("jellyMove", "image/Pip/�����̵�.bmp", 120, 47, 3, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("jellyJumpUp", "image/Pip/��������.bmp", 400, 42, 8, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("jellyJumpDown", "image/Pip/���������ٿ�.bmp", 562, 43, 11, 1, true, RGB(255, 0, 255));

	//�������� �޹��
	_loading->loadImage("�������1", "image/Stage/�������1.bmp", 1788, 740);
	_loading->loadImage("�������2", "image/Stage/�������2.bmp", 1700, 794, true, RGB(255, 0, 255));
	_loading->loadImage("�������3", "image/Stage/�������3.bmp", 1487, 771, true, RGB(255, 0, 255));
	_loading->loadImage("�������4", "image/Stage/�������4.bmp", 1204, 1412, true, RGB(255, 0, 255));

	//���ʹ�
	_loading->loadFrameImage("enemy-���ٴϴ� ����", "image/Enemy/enemy-���ٴϴ� ����.bmp", 940, 66, 20, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("enemy-���ٴϴ� ����", "image/Enemy/enemy-���ٴϴ� ����.bmp", 940, 48, 20, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("enemy-���ٴϴ� ũ����Ż", "image/Enemy/enemy-���ٴϴ� ũ����Ż.bmp", 1170, 166, 13, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("enemy-�յ��� ����", "image/Enemy/enemy-�յ��� ����.bmp", 236, 140, 4, 2, true, RGB(255, 0, 255));

	//����
	_loading->loadFrameImage("boss-�ȱ�", "image/Enemy/boss-�ȱ�_11��.bmp", 2838, 420, 11, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("boss-����1", "image/Enemy/boss-����1_7��.bmp", 3570, 456, 7, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("boss-����2", "image/Enemy/boss-����2_14��.bmp", 4284, 480, 14, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("boss-���", "image/Enemy/boss-���_9��.bmp", 864, 92, 9, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("boss-����", "image/Enemy/boss-����_25��.bmp", 10200, 420, 25, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("boss-�ǰ�", "image/Enemy/boss-�ǰݴ���_4��.bmp", 984, 420, 4, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("boss-����", "image/Enemy/boss-����_7��.bmp", 2184, 408, 7, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("boss-�޸���", "image/Enemy/boss-�޸���_8��.bmp", 1968, 420, 8, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("boss-����", "image/Enemy/boss-����_9��.bmp", 2376, 366, 9, 1, true, RGB(255, 0, 255));

	//��
	_loading->loadFrameImage("pets-����", "image/NPC/pets-����.bmp", 840, 96, 10, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pets-����", "image/NPC/pets-����.bmp", 960, 130, 12, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pets-���", "image/NPC/pets-���.bmp", 320, 104, 4, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("�� ��Ʈ", "image/NPC/����Ʈ - ��Ʈ _5��.bmp", 250, 42, 5, 1, true, RGB(255, 0, 255));

	//������
	_loading->loadFrameImage("���̺� ����Ʈ", "image/Stage/���̺� ����Ʈ.bmp", 390, 126, 13, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("���̺� ��", "image/Stage/���̺� ��.bmp", 1850, 105, 37, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("��������", "image/Stage/��������.bmp", 180, 88, 3, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("����", "image/Stage/����.bmp", 260, 20, 13, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("Ŭ���� ����Ʈ", "image/Stage/����Ʈ-Ŭ���� ����Ʈ 2_8��.bmp", 1856, 308, 8, 1, true, RGB(255, 0, 255));

	//����
	_loading->loadFrameImage("����Ÿ��", "image/Stage/�����ϸ�μ����� Ÿ��.bmp", 1260, 45, 28, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("����", "image/Stage/����Ÿ�� �����¸��.bmp", 540, 45, 12, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("��������Ÿ��", "image/Stage/�������� Ÿ��.bmp", 258, 41, 6, 1, true, RGB(255, 0, 255));

	//�Ѿ�
	_loading->loadImage("�Ѿ�", "image/Enemy/bullet.bmp", 21, 21, true, RGB(255, 0, 255));

	//UI
	_loading->loadImage("UI-�������", "image/UI/UI - �����̹���.bmp", 34, 34, true, RGB(255, 0, 255));
	_loading->loadFrameImage("UI-ü��", "image/UI/UI - ü�� _2��.bmp", 102, 47, 2, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("UI-�� �߰�", "image/UI/UI - �� �߰� �̹���.bmp", 192, 48, 4, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("UI-����", "image/UI/UI - �����̹���.bmp", 198, 26, 11, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("WorldMapPipIdle", "image/UI/WorldMapPipIdle(25,39).bmp", 375, 78, 15, 2);
	_loading->loadFrameImage("WorldMapPipRun", "image/UI/WorldMapPipRun(37,36).bmp", 444, 76, 12, 2);
	_loading->loadImage("Continue", "image/UI/continue.bmp", 162, 36);
	//������
	_loading->loadImage("����", "image/UI/ITEM ����.bmp", 500, 345, true, RGB(255, 0, 255));
	_loading->loadImage("�����ۼ���", "image/UI/ITEM ������â ����.bmp", 98, 98, true, RGB(255, 0, 255));
	_loading->loadImage("��ǳ��", "image/UI/UI-��ǳ��.bmp", 700, 200, true, RGB(255, 0, 255));
	_loading->loadImage("��Ʈ���", "image/UI/UI - ���� �����ִ� �̹���.bmp", 107, 91, true, RGB(255, 0, 255));

}

void loadingScene::loadingSound()
{
	//��Ʈ�� �������
	_loading->loadSound("�ó׸�ƽ", "SOUND/Intro_Cinematic.ogg", true, true);
	//�������� ����
	_loading->loadSound("��������", "SOUND/Forest.ogg", true, true);
	//���� ����
	_loading->loadSound("����", "SOUND/Town.ogg", true, true);
	//Ÿ��Ʋ �ε�
	_loading->loadSound("Ÿ��Ʋ", "SOUND/Title_Theme.ogg", true, true);
	//���� ����
	_loading->loadSound("����", "SOUND/villager_save.ogg", false, false);
	//���ڿ���
	_loading->loadSound("���ڱ�", "SOUND/chest_small.ogg", false, false);
	//�������� Ŭ����
	_loading->loadSound("Ŭ����", "SOUND/Stage_clear.ogg", false, false);
	//�ָ����� ��������
	_loading->loadSound("��ġ��", "SOUND/punch_block.ogg", false, false);
	//�ָ����� ���ġ��
	_loading->loadSound("��ġ��", "SOUND/punch_air.ogg", false, false);
	//�޴� ����
	_loading->loadSound("�޴�����", "SOUND/menu_select.ogg", false, false);
	//�������� ���ý�(�����)
	_loading->loadSound("������������", "SOUND/level_selected.ogg", false, false);
	//����
	_loading->loadSound("����", "SOUND/jump_pip2.ogg", false, false);
	//��ȭ
	_loading->loadSound("��ȭ", "SOUND/evolution.ogg", false, false);
	//���ʹ� ���
	_loading->loadSound("���ʹ̻��", "SOUND/enemy_death.ogg", false, false);
	//�÷��̾� ���
	_loading->loadSound("�÷��̾���", "SOUND/devo_blast_explode.ogg", false, false);
	//����ȹ��
	_loading->loadSound("����ȹ��", "SOUND/collect_pixel.ogg", false, false);
	//���̺�����Ʈ
	_loading->loadSound("���̺�", "SOUND/checkpoint.ogg", false, false);
	//������
	_loading->loadSound("������", "SOUND/bounce.ogg");
	//�޴� �¿��̵�
	_loading->loadSound("�޴��̵�", "SOUND/menu_back.ogg");
	//�÷��̾� �ǰݴ���
	_loading->loadSound("�÷��̾��ǰ�", "SOUND/devo_blast_crouch.ogg");

}
