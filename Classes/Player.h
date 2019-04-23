#pragma once
#include "Lords.h"
class Player final : public Lords //플레이어 더 상속받지 못하게 파이널 박아넣음
{
private:

	GameObject * playerGold = DATACENTRE.GetCertainObject(ObjType::Gold, "playerGold"); //골드는 게임 내에 플레이어만 들고있는다
	std::map<std::string, GameObject*> &inven = DATACENTRE.RefObjects(ObjType::PlayerItem); //인벤토리 목록은 게임 내에 플레이어만 가지고 있는다
	std::map<std::string, GameObject*> &playerTroop = DATACENTRE.RefObjects(ObjType::PlayerArmy); //플레이어 부대
	std::map<std::string, GameObject*> &battleTroop = DATACENTRE.RefObjects(ObjType::CurrentPlayerArmy); //현 전투에서 사용할 플레이어 부대

	RECT toMove = { 0,0,800,WINSIZEY };
public:

	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	Player();
	~Player() {};
};

