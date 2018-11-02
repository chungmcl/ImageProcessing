// ImageProcessingCPlusPlus.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <gdiplus.h>
#include <string>
#pragma comment(lib,"gdiplus.lib")
#include "ImageProcessingCPlusPlusHeaderFile.h"

using namespace Gdiplus;
using namespace std;

int main()
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	cout << "Enter file name: ";


	wchar_t input;
	wcin >> input;
	ImageProcessor imageProcessor(&input);


	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	Gdiplus::GdiplusShutdown(gdiplusToken);
}


ImageProcessor::ImageProcessor(WCHAR* location)
{
	{
		Image image(location, false);
		image.RotateFlip(Rotate180FlipX);
		CLSID encoder;
		GetEncoderClsid(L"image/png", &encoder);
		image.Save(location, &encoder);
	}
}

int ImageProcessor::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
