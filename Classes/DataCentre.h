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



//�嵥������ ������Ʈ �����̳� ��Ʈ
private:
	typedef std::map < ObjType, std::map<std::string, class GameObject*>> objCtr; //�̸��� �ʹ� �� ������
	objCtr objContainer; //�׿�����Ʈ���� Ű��(��Ʈ��)���� ������, �� �����̳ʸ� ������Ʈ Ÿ�Կ� ���� ����

//�嵥������ ���� �ε�������Ʈ 
private:
	

//�嵥������ �ΰ��� ���� ����ִ� ��Ʈ 
private:
//�÷��̾� ���� ����, ������ ���� �ʿ�




public:
	void Init();
	void Update();
	void Release();
	void Render();



	void AddObj(ObjType _type, std::string _name, class GameObject* _obj);
	void RemoveObj(ObjType _type, std::string _name, GameObject* _obj);
	GameObject* GetCertainObject(ObjType _type, std::string _name);
	std::map<std::string, GameObject*> & GetObjects(ObjType _type);
	
};

