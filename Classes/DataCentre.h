#pragma once


//�嵥���� ����� �� �ΰ��� ������ ������ �̱���
class DataCentre
{
public:
	static DataCentre& Getinstance() //���̾� ��� C++11�̻� ������ �̱���
	{
		static DataCentre instance;
		return instance;
	}
private:
	DataCentre();		//�̱����̶� ������ �Ҹ��� ����
	~DataCentre() {};	//���Ҵ� ������ ����

//�嵥������ �ΰ��� ���� ����ִ� ��Ʈ 
private:
	std::map<std::string, class GameObject*> playerArmy;
	std::map<std::string, class GameObject*> playerItem;


//�嵥������ ������Ʈ �����̳� ��Ʈ
private:
	//��׿�����Ʈ���� Ű��(��Ʈ��)���� ������, �� �����̳ʸ� ������Ʈ Ÿ�Կ� ���� ����
	std::map <ObjType, std::map<std::string, class GameObject*>> objContainer;

//�嵥������ ���� �ε�������Ʈ 
private:
	
	
	





public:
	void Init();
	void Update();
	void Release();
	void Render();


	//����� ������Ʈ ����
	void AddObj(ObjType _type, std::string _name, class GameObject* _obj);
	void RemoveObj(ObjType _type, std::string _name, GameObject* _obj);
	GameObject* GetCertainObject(ObjType _type, std::string _name);
	std::map<std::string, GameObject*> & GetObjects(ObjType _type);
	
	//������ ������ ����
	const std::map<std::string, class GameObject*> & RefPlayerArmy() const { return playerArmy; }
	const std::map<std::string, class GameObject*> * GetPlayerArmyPtr() const { return &playerArmy; }
	const std::map<std::string, class GameObject*>& RefPlayerItem() const { return playerItem; }
	const std::map<std::string, class GameObject*>* GetPlayerItemPtr() const { return &playerItem; }
	void SetPlayerArmy(std::map<std::string, class GameObject*> _changedArmy) { playerArmy = _changedArmy; }
	void SetPlayerItem(std::map<std::string, class GameObject*> _changedInven) { playerItem = _changedInven; }

	//�弼�̺�ε����
	HRESULT SavetoFile();
	HRESULT LoadFromFile();
};

