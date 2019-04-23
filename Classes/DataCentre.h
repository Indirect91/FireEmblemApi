#pragma once

//�������̳� Ÿ�Ե�
enum class ObjType : UINT
{
	Tile,
	ClippedTile,
	PlayerArmy,
	CurrentPlayerArmy,
	PlayerItem,
	EnemyArmy,
	CurrentEnemyArmy,
	UI,
	Gold,
	ObjSize
};

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

//�嵥������ ������Ʈ �����̳� ��Ʈ
private:
	//��׿�����Ʈ���� Ű��(��Ʈ��)���� ������, �� �����̳ʸ� ������Ʈ Ÿ�Կ� ���� ����
	std::map <ObjType, std::map<std::string, class GameObject*>> objContainer;

public:
	void Init();
	void Update();
	void Release();
	void Render();


	//����� ������Ʈ ����
	void ClearObjects(ObjType _toClear); //Ÿ���� �������� �����̳ʸ� ���
	void AddObj(ObjType _type, std::string _name, class GameObject* _obj); //�������� �����̳ʿ� �̸��ְ� ���
	void RemoveObj(ObjType _type, std::string _name, GameObject* _obj); //������ �����̳� ��, �̸��� ����� ����
	GameObject* GetCertainObject(ObjType _type, std::string _name); //������ �����̳ʿ� �̸��ְ� ��ȯ����
	std::map<std::string, GameObject*> & RefObjects(ObjType _type); //������ �����̳� ���θ� ��ȯ����

	//�弼�̺�ε����
	HRESULT SavetoFile();
	HRESULT LoadFromFile();
};

