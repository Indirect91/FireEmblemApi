#pragma once
#include "SingletonBase.h"

class ImageManager final : public SingletonBase<ImageManager>
{
private:
	typedef std::unordered_map<std::string, class Image*>::iterator ImageIter;
private:
	std::unordered_map<std::string, class Image*> mImageList;

	IWICImagingFactory* mWicFactory;
public:
	ImageManager();
	virtual ~ImageManager();

	class Image* AddImage(const std::string& key, const std::wstring& file);
	class Image* AddFrameImage(const std::string& key, const std::wstring& file, const int& maxFrameX, const int& maxFrameY);
	class Image* FindImage(const std::string& key);
private:
	ID2D1Bitmap* CreateD2DBitmapFromFile(const std::wstring& file);
	void DeleteAll();
};
