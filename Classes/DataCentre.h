#pragma once


//�嵥���� ����� �� �ΰ��� ������ ������ �̱���
class DataCentre
{
public:
	static DataCentre& Get() //���̾� ��� C++11�̻� ������ �̱���
	{
		static DataCentre instance;
		return instance;
	}
private:
	DataCentre();		//�̱����̶� ������ �Ҹ��� ����
	~DataCentre() {};	//�̱����̶� ������ �Ҹ��� ����



//�嵥������ ������Ʈ �����̳� ��Ʈ
private:
	//typedef std::map < ObjType, std::map<std::string, GameObject*>> objCtr; //�̸��� �ʹ� �� ������
	//objCtr objContainer; //�׿�����Ʈ���� Ű��(��Ʈ��)���� ������, �� �����̳ʸ� ������Ʈ Ÿ�Կ� ���� ����

//�嵥������ ���� �ε�������Ʈ 
private:
	

//�嵥������ �ΰ��� ���� ����ִ� ��Ʈ 
private:
//�÷��̾� ���� ����, ������ ���� �ʿ�




public:
	void AddObj(ObjType _type, std::string _name, class GameObject* _obj);

	
};

