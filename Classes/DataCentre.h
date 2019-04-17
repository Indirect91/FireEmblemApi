#pragma once


//▼데이터 입출력 및 인게임 데이터 보유용 싱글톤
class DataCentre
{
public:
	static DataCentre& Get() //마이어 방식 C++11이상 버전용 싱글톤
	{
		static DataCentre instance;
		return instance;
	}
private:
	DataCentre();		//싱글톤이라 생성자 소멸자 숨김
	~DataCentre() {};	//싱글톤이라 생성자 소멸자 숨김



//▼데이터중 오브젝트 컨테이너 파트
private:
	//typedef std::map < ObjType, std::map<std::string, GameObject*>> objCtr; //이름이 너무 길어서 재정의
	//objCtr objContainer; //겜오브젝트들은 키값(스트링)으로 보관됨, 그 컨테이너를 오브젝트 타입에 따라 관리

//▼데이터중 파일 로드저장파트 
private:
	

//▼데이터중 인게임 정보 들고있는 파트 
private:
//플레이어 현재 군단, 아이템 저장 필요




public:
	void AddObj(ObjType _type, std::string _name, class GameObject* _obj);

	
};

