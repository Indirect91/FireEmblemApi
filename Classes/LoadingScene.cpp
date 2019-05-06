#include "../stdafx.h"
#include "LoadingScene.h"
#include "TitleScene.h"
#include "MapToolScene.h"
#include "testScene.h"
#include "WorldScene.h"
#include "Battle01.h"

void LoadingScene::Init()
{
	//▼로딩클래스 뉴할당 해주고 초기화
	_loading = new Loading;
	_loading->Init();
	IMAGEMANAGER->AddFrameImage("Loading", L"IMAGE/TitleScene/Loading.png", 1, 44);

	//▼이미지 사운드 씬 로딩
	this->LoadingImage();
	this->LoadingSound();
	this->LoadScenes();

}

void LoadingScene::Release()
{
	//로딩클래스 뉴할당 해준적 있으니 해제
	_loading->Release();
	SAFE_DELETE(_loading);
}

//▼업데이트 한번에 하나의 아이템 로딩함. 로딩서 움직이는 이미지 추가해줘도 되고, 로딩씬서 해줘도 되고.
void LoadingScene::Update()
{
	_loading->Update(); //로딩클래스 애들 업데이트

	if (_loading->LoadingDone()) //이프 조건문을 확인하는 동시에 그 아래 있는 함수들까지 실행됨
	{
		//▼로딩이 끝나고 이동할 씬
		SCENEMANAGER->RefcoverAlpha() = 1.f;
		SCENEMANAGER->LoadScene("TitleScene");
	}
}


//▼이미지 로딩
void LoadingScene::LoadingImage()
{
	//▼통상
	_loading->LoadImg("GameOver", L"IMAGE/ETC/GameOver.png");
	_loading->LoadImg("PlayerWon", L"IMAGE/ETC/PlayerWon.png");

	//▼턴
	_loading->LoadImg("PlayerTurnBack", L"IMAGE/ETC/PlayerTurnBack.png");
	_loading->LoadImg("PlayerTurnRing", L"IMAGE/ETC/PlayerTurnRing.png");
	_loading->LoadImg("PlayerTurnText", L"IMAGE/ETC/PlayerTurnText.png");
	_loading->LoadImg("EnemyTurnRing", L"IMAGE/ETC/EnemyTurnRing.png");
	_loading->LoadImg("EnemyTurnBack", L"IMAGE/ETC/EnemyTurnBack.png");
	_loading->LoadImg("EnemyTurnText", L"IMAGE/ETC/EnemyTurnText.png");

	//▼커서
	_loading->LoadImg("Cursor", L"IMAGE/ETC/cursor.png");
	_loading->LoadImg("CursorRed", L"IMAGE/ETC/cursorRed.png");
	_loading->LoadImg("CursorBlue", L"IMAGE/ETC/cursorBlue.png");
	_loading->LoadImg("CursorBattle", L"IMAGE/ETC/cursorBattle.png");
	

	//▼색
	_loading->LoadImg("Blue", L"IMAGE/ETC/BLUE.png");
	_loading->LoadImg("Yellow", L"IMAGE/ETC/YELLOW.png");
	_loading->LoadImg("Green", L"IMAGE/ETC/GREEN.png");
	_loading->LoadImg("Red", L"IMAGE/ETC/RED.png");
	_loading->LoadImg("Purple", L"IMAGE/ETC/PURPLE.png");

	//▼맵툴용 
	_loading->LoadImg("MapToolBg1",		L"IMAGE/MapToolScene/MapToolBg1.png");
	_loading->LoadImg("MapToolBg2",		L"IMAGE/MapToolScene/MapToolBg2.png");
	_loading->LoadImg("MapToolTitle",	L"IMAGE/MapToolScene/MaptoolTitle.png");
	_loading->LoadImg("MapToolMapbox",	L"IMAGE/MapToolScene/MaptoolMapbox2.png");
	_loading->LoadImg("MapToolMat2",	L"IMAGE/MapToolScene/MaptoolMat2.png");
	_loading->LoadImg("oldpaper",		L"IMAGE/MapToolScene/oldpaper.png");
	
	//▼타이틀용 
	_loading->LoadImg("타이틀백그라운드",		L"IMAGE/TitleScene/TitleBackground.png");
	_loading->LoadImg("타이틀선택박스",		L"IMAGE/TitleScene/TitlePick.png");
	_loading->LoadImg("타이틀텍스트",			L"IMAGE/TitleScene/TitleLogo.png");
	_loading->LoadFrameImage("타이틀팔시온",	L"IMAGE/TitleScene/TitleFalchion.png", 34, 1);

	//▼캐릭터 프레임이미지
	_loading->LoadFrameImage("캐릭터Anna", L"IMAGE/Characters/Anna.png", 12, 293);
	_loading->LoadFrameImage("캐릭터Cherche", L"IMAGE/Characters/Cherche.png", 12, 293);
	_loading->LoadFrameImage("캐릭터Chrome", L"IMAGE/Characters/Chrome.png", 12, 293);
	_loading->LoadFrameImage("캐릭터Lissa", L"IMAGE/Characters/Lissa.png", 12, 293);
	_loading->LoadFrameImage("캐릭터Lucina", L"IMAGE/Characters/Lucina.png", 12, 293);
	_loading->LoadFrameImage("캐릭터Olivia", L"IMAGE/Characters/Olivia.png", 12, 293);
	_loading->LoadFrameImage("캐릭터Tharja", L"IMAGE/Characters/Tharja.png", 12, 293);
	_loading->LoadFrameImage("캐릭터Virion", L"IMAGE/Characters/Virion.png", 12, 293);
	
	//▼캐릭터 초상화
	_loading->LoadImg("초상화Anna", L"IMAGE/Portraits/Anna.png");
	_loading->LoadImg("초상화Cherche", L"IMAGE/Portraits/Cherche.png");
	_loading->LoadImg("초상화Chrome", L"IMAGE/Portraits/Chrome.png");
	_loading->LoadImg("초상화Lissa", L"IMAGE/Portraits/Lissa.png");
	_loading->LoadImg("초상화Lucina", L"IMAGE/Portraits/Lucina.png");
	_loading->LoadImg("초상화Olivia", L"IMAGE/Portraits/Olivia.png");
	_loading->LoadImg("초상화Tharja", L"IMAGE/Portraits/Tharja.png");
	_loading->LoadImg("초상화Virion", L"IMAGE/Portraits/Virion.png");

	//▼캐릭터 작은 공격 이미지
	_loading->LoadImg("공격Anna", L"IMAGE/Portraits/AnnaAttack.png");
	_loading->LoadImg("공격Cherche", L"IMAGE/Portraits/ChercheAttack.png");
	_loading->LoadImg("공격Chrome", L"IMAGE/Portraits/ChromeAttack.png");
	_loading->LoadImg("공격Lissa", L"IMAGE/Portraits/LissaAttack.png");
	_loading->LoadImg("공격Lucina", L"IMAGE/Portraits/LucinaAttack.png");
	_loading->LoadImg("공격Olivia", L"IMAGE/Portraits/OliviaAttack.png");
	_loading->LoadImg("공격Tharja", L"IMAGE/Portraits/TharjaAttack.png");
	_loading->LoadImg("공격Virion", L"IMAGE/Portraits/VirionAttack.png");

	//▼캐릭터 큰 공격 이미지
	_loading->LoadImg("공격풀사이즈Anna", L"IMAGE/Portraits/AnnaAttackFull.png");
	_loading->LoadImg("공격풀사이즈Cherche", L"IMAGE/Portraits/ChercheAttackFull.png");
	_loading->LoadImg("공격풀사이즈Chrome", L"IMAGE/Portraits/ChromeAttackFull.png");
	_loading->LoadImg("공격풀사이즈Lissa", L"IMAGE/Portraits/LissaAttackFull.png");
	_loading->LoadImg("공격풀사이즈Lucina", L"IMAGE/Portraits/LucinaAttackFull.png");
	_loading->LoadImg("공격풀사이즈Olivia", L"IMAGE/Portraits/OliviaAttackFull.png");
	_loading->LoadImg("공격풀사이즈Tharja", L"IMAGE/Portraits/TharjaAttackFull.png");
	_loading->LoadImg("공격풀사이즈Virion", L"IMAGE/Portraits/VirionAttackFull.png");

	//▼캐릭터 큰 크리티컬 이미지
	_loading->LoadImg("치명타풀사이즈Anna", L"IMAGE/Portraits/AnnaCriticalFull.png");
	_loading->LoadImg("치명타풀사이즈Cherche", L"IMAGE/Portraits/ChercheCriticalFull.png");
	_loading->LoadImg("치명타풀사이즈Chrome", L"IMAGE/Portraits/ChromeCriticalFull.png");
	_loading->LoadImg("치명타풀사이즈Lissa", L"IMAGE/Portraits/LissaCriticalFull.png");
	_loading->LoadImg("치명타풀사이즈Lucina", L"IMAGE/Portraits/LucinaCriticalFull.png");
	_loading->LoadImg("치명타풀사이즈Olivia", L"IMAGE/Portraits/OliviaCriticalFull.png");
	_loading->LoadImg("치명타풀사이즈Tharja", L"IMAGE/Portraits/TharjaCriticalFull.png");
	_loading->LoadImg("치명타풀사이즈Virion", L"IMAGE/Portraits/VirionCriticalFull.png");

	//▼캐릭터 작은 사망 이미지
	_loading->LoadImg("사망Anna", L"IMAGE/Portraits/AnnaDead.png");
	_loading->LoadImg("사망Cherche", L"IMAGE/Portraits/ChercheDead.png");
	_loading->LoadImg("사망Chrome", L"IMAGE/Portraits/ChromeDead.png");
	_loading->LoadImg("사망Lissa", L"IMAGE/Portraits/LissaDead.png");
	_loading->LoadImg("사망Lucina", L"IMAGE/Portraits/LucinaDead.png");
	_loading->LoadImg("사망Olivia", L"IMAGE/Portraits/OliviaDead.png");
	_loading->LoadImg("사망Tharja", L"IMAGE/Portraits/TharjaDead.png");
	_loading->LoadImg("사망Virion", L"IMAGE/Portraits/VirionDead.png");

	//▼캐릭터 큰 사망 이미지
	_loading->LoadImg("사망풀사이즈Anna", L"IMAGE/Portraits/AnnaDeadFull.png");
	_loading->LoadImg("사망풀사이즈Cherche", L"IMAGE/Portraits/ChercheDeadFull.png");
	_loading->LoadImg("사망풀사이즈Chrome", L"IMAGE/Portraits/ChromeDeadFull.png");
	_loading->LoadImg("사망풀사이즈Lissa", L"IMAGE/Portraits/LissaDeadFull.png");
	_loading->LoadImg("사망풀사이즈Lucina", L"IMAGE/Portraits/LucinaDeadFull.png");
	_loading->LoadImg("사망풀사이즈Olivia", L"IMAGE/Portraits/OliviaDeadFull.png");
	_loading->LoadImg("사망풀사이즈Tharja", L"IMAGE/Portraits/TharjaDeadFull.png");
	_loading->LoadImg("사망풀사이즈Virion", L"IMAGE/Portraits/VirionDeadFull.png");

	//▼캐릭터 대화 큰 이미지
	_loading->LoadImg("대화Anna", L"IMAGE/Portraits/AnnaTalk.png");
	_loading->LoadImg("대화Cherche", L"IMAGE/Portraits/ChercheTalk.png");
	_loading->LoadImg("대화Chrome", L"IMAGE/Portraits/ChromeTalk.png");
	_loading->LoadImg("대화Lissa", L"IMAGE/Portraits/LissaTalk.png");
	_loading->LoadImg("대화Lucina", L"IMAGE/Portraits/LucinaTalk.png");
	_loading->LoadImg("대화Olivia", L"IMAGE/Portraits/OliviaTalk.png");
	_loading->LoadImg("대화Tharja", L"IMAGE/Portraits/TharjaTalk.png");
	_loading->LoadImg("대화Virion", L"IMAGE/Portraits/VirionTalk.png");

	//▼타일용
	_loading->LoadFrameImage("타일커서", L"IMAGE/Tiles/SelectedTile.png", 4, 1);
	_loading->LoadFrameImage("MoveArrow", L"IMAGE/Tiles/MoveArrow.png", 4, 4);

	//UI용
	_loading->LoadImg("ActionTaken", L"IMAGE/ETC/ActionTaken.png");
	_loading->LoadImg("BattleUIBg", L"IMAGE/ETC/BattleUIBg.png");
	_loading->LoadImg("BattleBg", L"IMAGE/ETC/BattleBg.png");
	_loading->LoadImg("PhotoFrame", L"IMAGE/ETC/PhotoFrame.png");
	_loading->LoadImg("DialogBox", L"IMAGE/ETC/dialogBox.png");
	
	//▼이펙트용
	_loading->LoadFrameImage("Blink", L"IMAGE/Tiles/Blink.png",8,1);

}

//▼사운드
void LoadingScene::LoadingSound()
{
	//▼BGM
	_loading->LoadSound("맵툴BGM", "SOUND/FE/BGM/MapTool_Road Taken.mp3", true, true);//맵툴용
	_loading->LoadSound("타이틀BGM", "SOUND/FE/BGM/TitleBgm.mp3", true, true);//타이틀용
	_loading->LoadSound("인게임BGM", "SOUND/FE/BGM/InGame_March to Deliverance.mp3", true, true); //인게임용
	_loading->LoadSound("DeadBGM", "SOUND/FE/BGM/DeadBGM.mp3", true, true); //인게임용

	//▼SFX
	_loading->LoadSound("SelectionTitleComplete", "SOUND/FE/SFX/SelectionTitleComplete.wav");
	_loading->LoadSound("SelectionTitle", "SOUND/FE/SFX/SelectionTitle.wav");
	_loading->LoadSound("SelectionSmall", "SOUND/FE/SFX/SelectionSmall.wav");
	_loading->LoadSound("NormalAttack", "SOUND/FE/SFX/NormalAttack.wav");
	_loading->LoadSound("LevelUp", "SOUND/FE/SFX/LevelUp.wav");
	_loading->LoadSound("CharacterDead", "SOUND/FE/SFX/CharacterDead.wav");
	_loading->LoadSound("IntoFullBattle", "SOUND/FE/SFX/IntoFullBattle.wav");
	_loading->LoadSound("CriticalAttack", "SOUND/FE/SFX/CriticalAttack.wav");
	_loading->LoadSound("CharacterDamage0", "SOUND/FE/SFX/CharacterDamage0.wav");
	_loading->LoadSound("ChangeClass", "SOUND/FE/SFX/ChangeClass.wav");
	_loading->LoadSound("Cancel", "SOUND/FE/SFX/Cancel.wav");
	_loading->LoadSound("MissAttack", "SOUND/FE/SFX/Miss.mp3");
	_loading->LoadSound("CriticalInit", "SOUND/FE/SFX/CriticalInit.mp3");
	_loading->LoadSound("PlayerTurn", "SOUND/FE/SFX/PlayerTurn.wav");
	_loading->LoadSound("EnemyTurn", "SOUND/FE/SFX/EnemyTurn.wav");

	//▼캐릭터 음성
	_loading->LoadSound("Anna1",			"SOUND/FE/Character/Anna/Anna1.wav");
	_loading->LoadSound("Anna2",			"SOUND/FE/Character/Anna/Anna2.wav");
	_loading->LoadSound("Anna3",			"SOUND/FE/Character/Anna/Anna3.wav");
	_loading->LoadSound("AnnaAttack",		"SOUND/FE/Character/Anna/AnnaAttack.wav");
	_loading->LoadSound("AnnaCritical",		"SOUND/FE/Character/Anna/AnnaCritical.wav");
	_loading->LoadSound("AnnaDamage",		"SOUND/FE/Character/Anna/AnnaDamage.wav");
	_loading->LoadSound("AnnaDead",			"SOUND/FE/Character/Anna/AnnaDead.wav");

	_loading->LoadSound("Cherche1",			"SOUND/FE/Character/Cherche/Cherche1.wav");
	_loading->LoadSound("Cherche2",			"SOUND/FE/Character/Cherche/Cherche2.wav");
	_loading->LoadSound("Cherche3",			"SOUND/FE/Character/Cherche/Cherche3.wav");
	_loading->LoadSound("ChercheAttack",	"SOUND/FE/Character/Cherche/ChercheAttack.wav");
	_loading->LoadSound("ChercheCritical",	"SOUND/FE/Character/Cherche/ChercheCritical.wav");
	_loading->LoadSound("ChercheDamage",	"SOUND/FE/Character/Cherche/ChercheDamage.wav");
	_loading->LoadSound("ChercheDead",		"SOUND/FE/Character/Cherche/ChercheDead.wav");

	_loading->LoadSound("Chrome1",			"SOUND/FE/Character/Chrome/Chrome1.wav");
	_loading->LoadSound("Chrome2",			"SOUND/FE/Character/Chrome/Chrome2.wav");
	_loading->LoadSound("Chrome3",			"SOUND/FE/Character/Chrome/Chrome3.wav");
	_loading->LoadSound("ChromeAttack",		"SOUND/FE/Character/Chrome/ChromeAttack.wav");
	_loading->LoadSound("ChromeCritical",	"SOUND/FE/Character/Chrome/ChromeCritical.wav");
	_loading->LoadSound("ChromeDamage",		"SOUND/FE/Character/Chrome/ChromeDamage.wav");
	_loading->LoadSound("ChromeDead",		"SOUND/FE/Character/Chrome/ChromeDead.wav");

	_loading->LoadSound("Lissa1",			"SOUND/FE/Character/Lissa/Lissa1.wav");
	_loading->LoadSound("Lissa2",			"SOUND/FE/Character/Lissa/Lissa2.wav");
	_loading->LoadSound("Lissa3",			"SOUND/FE/Character/Lissa/Lissa3.wav");
	_loading->LoadSound("LissaAttack",		"SOUND/FE/Character/Lissa/LissaAttack.wav");
	_loading->LoadSound("LissaCritical",	"SOUND/FE/Character/Lissa/LissaCritical.wav");
	_loading->LoadSound("LissaDamage",		"SOUND/FE/Character/Lissa/LissaDamage.wav");
	_loading->LoadSound("LissaDead",		"SOUND/FE/Character/Lissa/LissaDead.wav");

	_loading->LoadSound("Lucina1",			"SOUND/FE/Character/Lucina/Lucina1.wav");
	_loading->LoadSound("Lucina2",			"SOUND/FE/Character/Lucina/Lucina2.wav");
	_loading->LoadSound("Lucina3",			"SOUND/FE/Character/Lucina/Lucina3.wav");
	_loading->LoadSound("LucinaAttack",		"SOUND/FE/Character/Lucina/LucinaAttack.wav");
	_loading->LoadSound("LucinaCritical",	"SOUND/FE/Character/Lucina/LucinaCritical.wav");
	_loading->LoadSound("LucinaDamage",		"SOUND/FE/Character/Lucina/LucinaDamage.wav");
	_loading->LoadSound("LucinaDead",		"SOUND/FE/Character/Lucina/LucinaDead.wav");

	_loading->LoadSound("Olivia1",			"SOUND/FE/Character/Olivia/Olivia1.wav");
	_loading->LoadSound("Olivia2",			"SOUND/FE/Character/Olivia/Olivia2.wav");
	_loading->LoadSound("Olivia3",			"SOUND/FE/Character/Olivia/Olivia3.wav");
	_loading->LoadSound("OliviaAttack",		"SOUND/FE/Character/Olivia/OliviaAttack.wav");
	_loading->LoadSound("OliviaCritical",	"SOUND/FE/Character/Olivia/OliviaCritical.wav");
	_loading->LoadSound("OliviaDamage",		"SOUND/FE/Character/Olivia/OliviaDamage.wav");
	_loading->LoadSound("OliviaDead",		"SOUND/FE/Character/Olivia/OliviaDead.wav");

	_loading->LoadSound("Tharja1",			"SOUND/FE/Character/Tharja/Tharja1.wav");
	_loading->LoadSound("Tharja2",			"SOUND/FE/Character/Tharja/Tharja2.wav");
	_loading->LoadSound("Tharja3",			"SOUND/FE/Character/Tharja/Tharja3.wav");
	_loading->LoadSound("TharjaAttack",		"SOUND/FE/Character/Tharja/TharjaAttack.wav");
	_loading->LoadSound("TharjaCritical",	"SOUND/FE/Character/Tharja/TharjaCritical.wav");
	_loading->LoadSound("TharjaDamage",		"SOUND/FE/Character/Tharja/TharjaDamage.wav");
	_loading->LoadSound("TharjaDead",		"SOUND/FE/Character/Tharja/TharjaDead.wav");

	_loading->LoadSound("Virion1",			"SOUND/FE/Character/Virion/Virion1.wav");
	_loading->LoadSound("Virion2",			"SOUND/FE/Character/Virion/Virion2.wav");
	_loading->LoadSound("Virion3",			"SOUND/FE/Character/Virion/Virion3.wav");
	_loading->LoadSound("VirionAttack",		"SOUND/FE/Character/Virion/VirionAttack.wav");
	_loading->LoadSound("VirionCritical",	"SOUND/FE/Character/Virion/VirionCritical.wav");
	_loading->LoadSound("VirionDamage",		"SOUND/FE/Character/Virion/VirionDamage.wav");
	_loading->LoadSound("VirionDead",		"SOUND/FE/Character/Virion/VirionDead.wav");

}

//▼로딩방법
void LoadingScene::LoadScenes()
{
	_loading->LoadScene("TitleScene",new TitleScene);
	_loading->LoadScene("MapToolScene", new MapToolScene);
	_loading->LoadScene("test", new testScene);
	_loading->LoadScene("WorldScene", new WorldScene);
	_loading->LoadScene("Battle01", new Battle01);
}


//▼로딩 이미지 그리기
void LoadingScene::Render()
{
	float num = static_cast<float>(_loading->GetLoadItem().size());
	float cur = static_cast<float>(_loading->GetCurrentGauge());
	per = cur / num * 100.f;

	INT LoadingframeY = static_cast<INT> (per / 2.f);
	if (LoadingframeY > 43) {LoadingframeY = 43;}

	IMAGEMANAGER->FindImage("Loading")->SetSize(IMAGEMANAGER->FindImage("Loading")->GetFrameSize());
	IMAGEMANAGER->FindImage("Loading")->FrameRender(WINSIZEX / 2+ 80, WINSIZEY-200, 0, LoadingframeY);
}