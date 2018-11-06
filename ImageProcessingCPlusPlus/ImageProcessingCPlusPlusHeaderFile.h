#pragma once

class ImageProcessor
{
private:
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	Gdiplus::Bitmap currentImage;
public:
	ImageProcessor(const WCHAR* location);
	void GrayScale();
};
