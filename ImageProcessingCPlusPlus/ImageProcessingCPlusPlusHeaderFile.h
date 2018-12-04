#pragma once

class ImageProcessor
{
private:
	Gdiplus::Bitmap currentImage;
	Gdiplus::BitmapData BMD;
	Gdiplus::PixelFormat pixelFormat;
	Gdiplus::Rect GetRekt(Gdiplus::Bitmap &image, Gdiplus::BitmapData &data);
	byte* GetPixel(int x, int y, Gdiplus::BitmapData BMD);
	void EndProcess(Gdiplus::Bitmap &image, Gdiplus::BitmapData &BMD);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
public:
	ImageProcessor(const WCHAR* location, Gdiplus::PixelFormat pf);
	void GrayScale();
	void HorizontalMirror();
	void Rotate();
};
