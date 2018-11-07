// ImageProcessingCPlusPlus.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "GdiplusInit.h"
#include "DrawPanel.h"
#include <iostream>
#include <gdiplus.h>
#include <string>
#pragma comment(lib,"gdiplus.lib")
#include "ImageProcessingCPlusPlusHeaderFile.h"

using namespace Gdiplus;
using namespace std;

int main()
{
	GdiplusInit init;
	cout << "Enter file name: ";

	wstring input;
	wcin >> input;
	ImageProcessor imageProcessor(input.c_str());
	imageProcessor.GrayScale();
}


ImageProcessor::ImageProcessor(const WCHAR* location) : currentImage(location, false)
{

}

void ImageProcessor::GrayScale()
{
	RectF getRekt;
	RectF* getRektPointer = &getRekt;
	Unit unit;
	currentImage.GetBounds(getRektPointer, &unit);
	Rect rekt(getRekt.X, getRekt.Y, getRekt.Width, getRekt.Height);

	BitmapData BMD; 
	// Stride - number of bytes in one line
	// Width - pixels per line (one pixel is four bytes in PixelFormat32bppARGB)
	// Scan0 - pointer to first pixel
	currentImage.LockBits(&rekt, ImageLockModeWrite, PixelFormat32bppARGB, &BMD);
	void* currentPixel = BMD.Scan0;

	for (int row = 0; row < BMD.Height; row++)
	{
		for (int pixel = 0; pixel < BMD.Stride; pixel += 4)
		{
			for (int x = 0; x < 3; x++)
			{
				byte* viewPixel = reinterpret_cast<byte*>(pixel);

				pixel++;
			}
		}
	}

	DrawPanel drawPanel(&currentImage);
	drawPanel.MessageLoop();
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
