#pragma once

class ImageProcessor
{
private:
	Gdiplus::Bitmap currentImage;
	Gdiplus::BitmapData BMD;
	Gdiplus::Rect GetRekt();
	void Rotate();
	byte* GetPixel(int x, int y);
	void EndProcess();
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
public:
	ImageProcessor(const WCHAR* location);
	void GrayScale();
	void HorizontalMirror();
};
