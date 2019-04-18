 #pragma once
#include "GameObject.h"
class UserInteface : public GameObject
{
public:
	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	UserInteface();
	~UserInteface();
};

