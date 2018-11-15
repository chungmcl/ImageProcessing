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
	bool programRunning = true;
	GdiplusInit init;

	while (programRunning)
	{
		system("cls");
		cout << "Conversion Software Version 7.0" << endl;
		cout << "Enter Q for Quit" << endl;
		cout << "Enter file name: ";

		wstring fileInput;
		wcin >> fileInput;
		if (fileInput == (wstring)L"Q")
		{
			break;
		}
		ImageProcessor imageProcessor(fileInput.c_str());

		cout << "G for Grayscale" << endl;
		cout << "H for Horizontal Mirror" << endl;
		cout << "Q for Quit" << endl;
		cout << "Choose an option: ";
			
		string editInput;
		cin >> editInput;

		if (editInput == "G")
		{
			imageProcessor.GrayScale();
		}
		if (editInput == "H")
		{
			imageProcessor.HorizontalMirror();
		}
		if (editInput == "Q")
		{
			break;
		}
	}
}


ImageProcessor::ImageProcessor(const WCHAR* location) : currentImage(location, false)
{

}

Rect ImageProcessor::GetRekt()
{
	RectF getRekt;
	RectF* getRektPointer = &getRekt;
	Unit unit;
	currentImage.GetBounds(getRektPointer, &unit);
	Rect rekt(getRekt.X, getRekt.Y, getRekt.Width, getRekt.Height);

	currentImage.LockBits(&rekt, ImageLockModeWrite, PixelFormat32bppARGB, &BMD);
	return rekt;
}

void ImageProcessor::HorizontalMirror()
{
	Rect rect = GetRekt();

	for (int y = 0; y < BMD.Height / 2; y++)
	{
		for (int x = 0; x < BMD.Width; x++)
		{
			for (int sub = 0; sub < (BMD.Stride / BMD.Width); sub++)
			{
				*(GetPixel(x, BMD.Height - y) + sub) = *(GetPixel(x, y) + sub);
			}
		}
	}
	EndProcess();
}

void ImageProcessor::Rotate()
{
	Rect rect = GetRekt();
	byte* rotatedArray = new byte[BMD.Height * BMD.Width];

	delete[] rotatedArray;
}

void ImageProcessor::GrayScale()
{
	Rect rect = GetRekt();
	byte* viewPixel = (byte*)BMD.Scan0;

	for (int y = 0; y < BMD.Height; y++)
	{
		for (int x = 0; x < BMD.Width; x++)
		{
			byte avg = ((*(GetPixel(x, y)) * .114) + ((*(GetPixel(x, y)) + 1) * .587) + ((*(GetPixel(x, y)) + 2) * .299) / 3);
			for (int sub = 0; sub < (BMD.Stride / BMD.Width) - 1; sub++)
				*(GetPixel(x, y) + sub) = avg;
		}
	}
	EndProcess();
}

byte* ImageProcessor::GetPixel(int x, int y)
{
	return (byte*)BMD.Scan0 + (BMD.Stride * y) + (x * (BMD.Stride / BMD.Width));
}

void ImageProcessor::EndProcess()
{
	currentImage.UnlockBits(&BMD);
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
