 #pragma once
#include "GameObject.h"
class UserInteface : public GameObject
{
private:
	struct ShowData
	{
		std::string toShowName = "";		//누구 보여줄건지 이름
		INT rating = 0;
		INT currentHealth = 0;
		INT maxHealth = 0;
	};


private:

	class Character* toShow = nullptr;  //누구를 보여줄지. 자주 null로 바뀌는 공용 자료
	FLOAT imageAlpha = 0;				//초상화 알파값. 자주 0~1로 바뀌는 공용 자료

	ShowData showData;					//지정된 대상에 대해서만 적용될 자료


public:
	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	UserInteface();
	~UserInteface() {};
};

