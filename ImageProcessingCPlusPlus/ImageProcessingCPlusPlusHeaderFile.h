#pragma once

class ImageProcessor
{
private:
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
public:
	ImageProcessor(WCHAR* location);
};
