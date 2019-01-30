#pragma once

class ImageProcessor
{
private:
	Gdiplus::Bitmap currentImage;
	Gdiplus::BitmapData BMD;
	Gdiplus::PixelFormat pixelFormat;
	const wchar_t* saveLocation;

	Gdiplus::Rect GetRekt(Gdiplus::Bitmap &image, Gdiplus::BitmapData &data);
	byte* GetPixel(int x, int y, Gdiplus::BitmapData BMD);

	uint16_t GetRed(int x, int y);
	uint16_t GetGreen(int x, int y);
	uint16_t GetBlue(int x, int y);
	uint16_t PaccPixel(uint16_t red, uint16_t green, uint16_t blue);
	uint32_t PaccPixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
	uint8_t PaccPixelDestegosaurus323ARGB(int x, int y);
	
	void SetPixel(int x, int y, uint32_t setTo);
	void SetPixel(int x, int y, uint16_t setTo);
	
	void EndProcess(Gdiplus::Bitmap &image, Gdiplus::BitmapData &BMD);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
public:
	ImageProcessor(const WCHAR* location, Gdiplus::PixelFormat pf, const WCHAR* saveLocation);
	void GrayScale();
	void HorizontalMirror();
	void Rotate();
	void Stegosaurus(const char* text, int textLength);
	void Destegosaurus();
};
