#pragma once

#ifndef ImageProcessing
#define ImageProcessing
class ImageProcessing
{
private:
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	void Initialize(const WCHAR* location);
public:

};
#endif
