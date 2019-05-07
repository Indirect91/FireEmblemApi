 #pragma once
#include "GameObject.h"
class UserInteface : public GameObject
{
private:
	struct ShowData
	{
		std::string toShowName = "";		//���� �����ٰ��� �̸�
		INT rating = 0;
		INT currentHealth = 0;
		INT maxHealth = 0;
	};


private:

	class Character* toShow = nullptr;  //������ ��������. ���� null�� �ٲ�� ���� �ڷ�
	FLOAT imageAlpha = 0;				//�ʻ�ȭ ���İ�. ���� 0~1�� �ٲ�� ���� �ڷ�

	ShowData showData;					//������ ��� ���ؼ��� ����� �ڷ�


public:
	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	UserInteface();
	~UserInteface() {};
};

