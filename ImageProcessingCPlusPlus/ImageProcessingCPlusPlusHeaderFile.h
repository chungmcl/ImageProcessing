#pragma once

class ImageProcessor
{
private:
	Gdiplus::Bitmap currentImage;
	Gdiplus::BitmapData BMD;
	Gdiplus::Rect GetRekt();
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
public:
	ImageProcessor(const WCHAR* location);
	void GrayScale();
	void HorizontalMirror();
};
