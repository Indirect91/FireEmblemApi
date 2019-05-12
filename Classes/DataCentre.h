#pragma once

//▼컨테이너 타입들
enum class ObjType : UINT
{
	UI,
	Tile,
	Gold,
	Battle,
	FoeTiles,
	BlueTiles,
	EnemyArmy,
	AllyTiles,
	PlayerArmy,
	PlayerItem,
	ClippedTile,
	//CurrentEnemyArmy,
	CurrentPlayerArmy,
	ObjSize //이건 항시 마지막에 두자
};

//▼데이터 입출력 및 인게임 데이터 보유용 싱글톤
class DataCentre
{
public:
	static DataCentre& Getinstance() //마이어 방식 C++11이상 버전용 싱글톤
	{
		static DataCentre instance;
		return instance;
	}
private:
	struct EnemyData //로드용
	{
		INT occupation;
		POINT realPos;
	};

	DataCentre();		//싱글톤이라 생성자 소멸자 숨김
	~DataCentre() {};	//뉴할당 받은거 없음

//▼데이터중 오브젝트 컨테이너 파트
private:
	//▼겜오브젝트들은 키값(스트링)으로 보관됨, 그 컨테이너를 오브젝트 타입에 따라 관리
	std::map <ObjType, std::map<std::string, class GameObject*>> objContainer;

public:
	void Init();
	void Update();
	void Release();
	void Render();

	//▼게임 오브젝트 관련
	void ClearObjects(ObjType _toClear); //타입을 지정해준 컨테이너를 비움
	void AddObj(ObjType _type, std::string _name, class GameObject* _obj); //지정해준 컨테이너에 이름주고 등록
	void RemoveObj(ObjType _type, std::string _name, GameObject* _obj); //지정된 컨테이너 속, 이름의 대상을 지움
	GameObject* GetCertainObject(ObjType _type, std::string _name); //지정된 컨테이너에 이름주고 반환받음
	BOOL CheckObjectExistance(ObjType _type, std::string _name); //지정된 컨테이너에 존재하는지 체크
	std::map<std::string, GameObject*> & RefObjects(ObjType _type); //지정된 컨테이너 전부를 반환받음

	//▼세이브로드관련
	HRESULT SavetoFile();
	HRESULT LoadFromFile();
};

