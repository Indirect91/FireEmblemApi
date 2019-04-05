#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init(void)
{
	//로딩클래스 초기화
	_loading = new loading;
	_loading->init();

	//이미지 및 사운드 로딩
	this->loadingImage();
	this->loadingSound();

	return S_OK;
}

void loadingScene::release(void)
{
	//로딩클래스 해제
	_loading->release();
	SAFE_DELETE(_loading);
}

void loadingScene::update(void)
{
	//로딩클래스 업데이트
	_loading->update();

	//로딩완료후 화면(씬)변경
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
	//로딩클래스 렌더
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
	//스테이지 1번
	_loading->loadImage("Stage1", "image/Stage/Stage1_Resized.bmp", 17520, 2436, true, RGB(255, 0, 255));
	_loading->loadImage("Stage1Collision", "image/Stage/Stage1_Collision.bmp", 17520, 2436, true, RGB(255, 0, 255));

	//보스 스테이지
	_loading->loadImage("BossStage", "image/Stage/Boss_stage_Resized.bmp", 10976, 2436, true, RGB(255, 0, 255));
	_loading->loadImage("BossStageCollision", "image/Stage/Boss_stage_Collision.bmp", 10976, 2436, true, RGB(255, 0, 255));
	//마을 스테이지
	_loading->loadImage("Village", "image/Stage/Stage 마을.bmp", 2750, 768, true, RGB(255, 0, 255));
	_loading->loadImage("VillageCollision", "image/Stage/Stage 마을 바닥 픽셀.bmp", 2750, 768, true, RGB(255, 0, 255));
	_loading->loadImage("Village 배경", "image/Stage/Stage 마을 배경.bmp", 1366, 768);
	//월드맵 씬
	_loading->loadImage("WorldMap", "image/UI/WorldMap 1366,768.bmp", 1366, 768);
	_loading->loadImage("WorldMapBig", "image/UI/WorldMap 5464,3072.bmp", 5464, 3072);
	_loading->loadImage("WorldMapLow","image/UI/WorldMapLow 1210,72.bmp", 1210, 72, true, RGB(255, 0, 255));
	_loading->loadImage("WorldMapHigh", "image/UI/WorldMapHigh 1212,163.bmp", 1212, 163, true, RGB(255, 0, 255));

	//시네마씬
	_loading->loadImage("씬1", "image/Cinematic/씬1.bmp", 1366, 768);
	_loading->loadImage("씬2", "image/Cinematic/씬2.bmp", 1366, 768);
	_loading->loadImage("씬3", "image/Cinematic/씬3.bmp", 1366, 768);
	_loading->loadImage("씬4", "image/Cinematic/씬4.bmp", 1366, 768);
	_loading->loadImage("씬5", "image/Cinematic/씬5.bmp", 1366, 768);
	_loading->loadImage("씬6", "image/Cinematic/씬6.bmp", 1366, 768);
	_loading->loadImage("씬7", "image/Cinematic/씬7.bmp", 1366, 768);
	_loading->loadImage("씬8", "image/Cinematic/씬8.bmp", 1366, 768);
	_loading->loadImage("씬9", "image/Cinematic/씬9.bmp", 1366, 768);
	_loading->loadImage("씬10", "image/Cinematic/씬10.bmp", 1366, 768);
	_loading->loadImage("씬11", "image/Cinematic/씬11.bmp", 1366, 768);
	_loading->loadImage("커튼콜", "image/Cinematic/커튼콜.bmp", 1366, 768);

	//NPC
	_loading->loadFrameImage("NPC-상인", "image/NPC/NPC-상인 할아버지_12장.bmp", 1536, 132, 12, 1, true, RGB(255, 0, 255));
	_loading->loadImage("이펙트-말풍선", "image/NPC/이펙트 - 하이.bmp", 55, 45, true, RGB(255, 0, 255));

	//타이틀 씬
	_loading->loadImage("titleBg", "image/UI/titleBg.bmp", 1366, 768);
	_loading->loadImage("titleLogo", "image/UI/titleLogo.bmp", 599, 254, true, RGB(255, 0, 255));
	_loading->loadImage("pip1", "image/UI/titlePip1.bmp", 240, 228, true, RGB(255, 0, 255));
	_loading->loadImage("pip2", "image/UI/titlePip2.bmp", 252, 325, true, RGB(255, 0, 255));
	_loading->loadImage("pip3", "image/UI/titlePip3.bmp", 450, 472, true, RGB(255, 0, 255));
	_loading->loadImage("PressEnter", "image/UI/titlePressEnter.bmp", 488, 97, true, RGB(255, 0, 255));
	_loading->loadImage("PressEsc", "image/UI/titlePressEsc.bmp", 217, 63, true, RGB(255, 0, 255));
	
	//세이브 슬롯 픽화면
	_loading->loadImage("fileSelect", "image/UI/FileSelect(1000,510).bmp", 1000, 510, true, RGB(255, 0, 255));
	_loading->loadImage("loadFile", "image/UI/FileSelectLoadFile(219,266).bmp", 219, 266, true, RGB(255, 0, 255));
	_loading->loadImage("ESC_YSetting", "image/UI/FileSelectESC_YSetting(1368,82).bmp", 1368, 82, true, RGB(255, 0, 255));
	_loading->loadImage("newEmpty", "image/UI/FileSelectnewEmpty(219,266).bmp", 219, 266, true, RGB(255, 0, 255));
	_loading->loadImage("selection", "image/UI/FileSelectSelection (338, 361).bmp", 338, 361, true, RGB(255, 0, 255));

	//인간 핍(픽셀)
	_loading->loadFrameImage("pipIdle", "image/Pip/pipidle(49,76) 15장.bmp", 735, 152, 15, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pipAttackLeft", "image/Pip/pipattack(95,75)Left 12장.bmp", 1140, 75, 12, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pipAttackRight", "image/Pip/pipattack(95,75)Right 12장.bmp", 1140, 75, 12, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pipRun", "image/Pip/pipRun (73,71) 12장.bmp", 876, 142, 12, 2, true, RGB(255, 0, 255));
	_loading->loadImage("pipGrabLeft", "image/Pip/pipGrabLeft.bmp", 54, 73, true, RGB(255, 0, 255));
	_loading->loadImage("pipGrabRight", "image/Pip/pipGrabRight.bmp", 54, 73, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pipJumpUp", "image/Pip/인간핍 점프상승 (54,76) 7장.bmp", 378, 152, 7, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pipJumpDown", "image/Pip/인간핍 점프하락 (73,82)6장.bmp", 438, 164, 6, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pipDownAttack", "image/Pip/인간핍 공격하단(93,184) 11장.bmp", 1023, 184, 11, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pipChangeForm", "image/Pip/변신핍(76,114) 10장.bmp", 570, 152, 10, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("changeEffect", "image/Pip/changeEffect.bmp", 4400, 100, 44, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pipHit", "image/Pip/playerHit1.bmp", 450, 145, 6, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("portl", "2312,308_ 복사본.bmp", 2312, 308, 17, 1, true, RGB(255, 0, 255));

	//젤리 핍(젤리)
	_loading->loadFrameImage("jellyStay", "image/Pip/젤리대기(46,37) 8장.bmp", 368, 37, 8, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("jellyMove", "image/Pip/젤리이동.bmp", 120, 47, 3, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("jellyJumpUp", "image/Pip/젤리점프.bmp", 400, 42, 8, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("jellyJumpDown", "image/Pip/젤리점프다운.bmp", 562, 43, 11, 1, true, RGB(255, 0, 255));

	//스테이지 뒷배경
	_loading->loadImage("나무배경1", "image/Stage/나무배경1.bmp", 1788, 740);
	_loading->loadImage("나무배경2", "image/Stage/나무배경2.bmp", 1700, 794, true, RGB(255, 0, 255));
	_loading->loadImage("나무배경3", "image/Stage/나무배경3.bmp", 1487, 771, true, RGB(255, 0, 255));
	_loading->loadImage("나무배경4", "image/Stage/나무배경4.bmp", 1204, 1412, true, RGB(255, 0, 255));

	//에너미
	_loading->loadFrameImage("enemy-떠다니는 문어", "image/Enemy/enemy-떠다니는 문어.bmp", 940, 66, 20, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("enemy-기어다니는 벌레", "image/Enemy/enemy-기어다니는 벌레.bmp", 940, 48, 20, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("enemy-떠다니는 크리스탈", "image/Enemy/enemy-떠다니는 크리스탈.bmp", 1170, 166, 13, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("enemy-둥둥이 유령", "image/Enemy/enemy-둥둥이 유령.bmp", 236, 140, 4, 2, true, RGB(255, 0, 255));

	//보스
	_loading->loadFrameImage("boss-걷기", "image/Enemy/boss-걷기_11장.bmp", 2838, 420, 11, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("boss-공격1", "image/Enemy/boss-공격1_7장.bmp", 3570, 456, 7, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("boss-공격2", "image/Enemy/boss-공격2_14장.bmp", 4284, 480, 14, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("boss-대기", "image/Enemy/boss-대기_9장.bmp", 864, 92, 9, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("boss-죽음", "image/Enemy/boss-죽음_25장.bmp", 10200, 420, 25, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("boss-피격", "image/Enemy/boss-피격당함_4장.bmp", 984, 420, 4, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("boss-고함", "image/Enemy/boss-고함_7장.bmp", 2184, 408, 7, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("boss-달리기", "image/Enemy/boss-달리기_8장.bmp", 1968, 420, 8, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("boss-변신", "image/Enemy/boss-변신_9장.bmp", 2376, 366, 9, 1, true, RGB(255, 0, 255));

	//펫
	_loading->loadFrameImage("pets-박쥐", "image/NPC/pets-박쥐.bmp", 840, 96, 10, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pets-물개", "image/NPC/pets-물개.bmp", 960, 130, 12, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("pets-상어", "image/NPC/pets-상어.bmp", 320, 104, 4, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("펫 하트", "image/NPC/이펙트 - 하트 _5장.bmp", 250, 42, 5, 1, true, RGB(255, 0, 255));

	//지형물
	_loading->loadFrameImage("세이브 포인트", "image/Stage/세이브 포인트.bmp", 390, 126, 13, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("세이브 빛", "image/Stage/세이브 빛.bmp", 1850, 105, 37, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("보물상자", "image/Stage/보물상자.bmp", 180, 88, 3, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("보석", "image/Stage/보석.bmp", 260, 20, 13, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("클리어 포인트", "image/Stage/이펙트-클리어 포인트 2_8장.bmp", 1856, 308, 8, 1, true, RGB(255, 0, 255));

	//지형
	_loading->loadFrameImage("변신타일", "image/Stage/변신하면부서지는 타일.bmp", 1260, 45, 28, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("폭발", "image/Stage/변신타일 터지는모습.bmp", 540, 45, 12, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("무너지는타일", "image/Stage/무너지는 타일.bmp", 258, 41, 6, 1, true, RGB(255, 0, 255));

	//총알
	_loading->loadImage("총알", "image/Enemy/bullet.bmp", 21, 21, true, RGB(255, 0, 255));

	//UI
	_loading->loadImage("UI-보석모양", "image/UI/UI - 보석이미지.bmp", 34, 34, true, RGB(255, 0, 255));
	_loading->loadFrameImage("UI-체력", "image/UI/UI - 체력 _2장.bmp", 102, 47, 2, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("UI-펫 발견", "image/UI/UI - 펫 발견 이미지.bmp", 192, 48, 4, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("UI-숫자", "image/UI/UI - 숫자이미지.bmp", 198, 26, 11, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("WorldMapPipIdle", "image/UI/WorldMapPipIdle(25,39).bmp", 375, 78, 15, 2);
	_loading->loadFrameImage("WorldMapPipRun", "image/UI/WorldMapPipRun(37,36).bmp", 444, 76, 12, 2);
	_loading->loadImage("Continue", "image/UI/continue.bmp", 162, 36);
	//아이템
	_loading->loadImage("상점", "image/UI/ITEM 상점.bmp", 500, 345, true, RGB(255, 0, 255));
	_loading->loadImage("아이템선택", "image/UI/ITEM 아이템창 선택.bmp", 98, 98, true, RGB(255, 0, 255));
	_loading->loadImage("말풍선", "image/UI/UI-말풍선.bmp", 700, 200, true, RGB(255, 0, 255));
	_loading->loadImage("힌트사용", "image/UI/UI - 펫이 여기있다 이미지.bmp", 107, 91, true, RGB(255, 0, 255));

}

void loadingScene::loadingSound()
{
	//인트로 배경음악
	_loading->loadSound("시네마틱", "SOUND/Intro_Cinematic.ogg", true, true);
	//스테이지 사운드
	_loading->loadSound("스테이지", "SOUND/Forest.ogg", true, true);
	//마을 사운드
	_loading->loadSound("마을", "SOUND/Town.ogg", true, true);
	//타이틀 로딩
	_loading->loadSound("타이틀", "SOUND/Title_Theme.ogg", true, true);
	//동물 구출
	_loading->loadSound("구출", "SOUND/villager_save.ogg", false, false);
	//상자열때
	_loading->loadSound("상자깡", "SOUND/chest_small.ogg", false, false);
	//스테이지 클리어
	_loading->loadSound("클리어", "SOUND/Stage_clear.ogg", false, false);
	//주먹으로 벽쳤을때
	_loading->loadSound("벽치기", "SOUND/punch_block.ogg", false, false);
	//주먹으로 허공치기
	_loading->loadSound("헛치기", "SOUND/punch_air.ogg", false, false);
	//메뉴 선택
	_loading->loadSound("메뉴선택", "SOUND/menu_select.ogg", false, false);
	//스테이지 선택시(월드맵)
	_loading->loadSound("스테이지선택", "SOUND/level_selected.ogg", false, false);
	//점프
	_loading->loadSound("점프", "SOUND/jump_pip2.ogg", false, false);
	//진화
	_loading->loadSound("진화", "SOUND/evolution.ogg", false, false);
	//에너미 사망
	_loading->loadSound("에너미사망", "SOUND/enemy_death.ogg", false, false);
	//플레이어 사망
	_loading->loadSound("플레이어사망", "SOUND/devo_blast_explode.ogg", false, false);
	//보석획득
	_loading->loadSound("보석획득", "SOUND/collect_pixel.ogg", false, false);
	//세이브포인트
	_loading->loadSound("세이브", "SOUND/checkpoint.ogg", false, false);
	//점프대
	_loading->loadSound("점프대", "SOUND/bounce.ogg");
	//메뉴 좌우이동
	_loading->loadSound("메뉴이동", "SOUND/menu_back.ogg");
	//플레이어 피격당함
	_loading->loadSound("플레이어피격", "SOUND/devo_blast_crouch.ogg");

}
