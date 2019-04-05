#pragma once
class Camera
{
public:
	static Camera& Get()
	{
		static Camera instance;
		return instance;
	}
private:
	Camera();
	~Camera();


	RECT cRc;
public:
	const RECT &GetCameraRc() const{ return cRc; }
	void SetCamera(RECT& _cRc) { cRc = _cRc; }
	RECT &RefCameraRc() { return cRc; }
};

