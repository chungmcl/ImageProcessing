#pragma once

class ImageProcessor
{
private:
	Gdiplus::Bitmap currentImage;
	Gdiplus::BitmapData BMD;
	Gdiplus::PixelFormat pixelFormat;

	Gdiplus::Rect GetRekt(Gdiplus::Bitmap &image, Gdiplus::BitmapData &data);
	byte* GetPixelLocation(int x, int y, Gdiplus::BitmapData BMD);

	uint16_t GetRed(int x, int y);
	uint16_t GetGreen(int x, int y);
	uint16_t GetBlue(int x, int y);
	uint16_t PaccPixel(uint16_t red, uint16_t green, uint16_t blue);

	
	void SetPixel(int x, int y, uint32_t setTo);
	void SetPixel(int x, int y, uint16_t setTo);
	
	void EndProcess(Gdiplus::Bitmap &image, Gdiplus::BitmapData &BMD);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
public:
	ImageProcessor(const WCHAR* location, Gdiplus::PixelFormat pf);
	void GrayScale();
	void HorizontalMirror();
	void Rotate();
};
