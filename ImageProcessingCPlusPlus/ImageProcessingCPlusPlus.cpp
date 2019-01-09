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

		cout << "Enter Pixel Format ('1' for 32 bit, '2' for 16 bit): ";
		string pixelFormatInput;
		cin >> pixelFormatInput;
		Gdiplus::PixelFormat pixelFormat;
		if (pixelFormatInput == "1")
		{
			pixelFormat = PixelFormat32bppARGB;
		}
		if (pixelFormatInput == "2")
		{
			pixelFormat = PixelFormat16bppRGB565;
		}

		ImageProcessor imageProcessor(fileInput.c_str(), pixelFormat);

		cout << "G for Grayscale" << endl;
		cout << "H for Horizontal Mirror" << endl;
		cout << "Q for Quit" << endl;
		cout << "R for Rotate" << endl;
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
		if (editInput == "R")
		{
			imageProcessor.Rotate();
		}
		if (editInput == "Q")
		{
			break;
		}
	}
}

ImageProcessor::ImageProcessor(const WCHAR* location, Gdiplus::PixelFormat pf) : currentImage(location, false)
{
	pixelFormat = pf;
}

Rect ImageProcessor::GetRekt(Gdiplus::Bitmap &image, Gdiplus::BitmapData &data)
{
	RectF getRekt;
	RectF* getRektPointer = &getRekt;
	Unit unit;
	image.GetBounds(getRektPointer, &unit);
	Rect rekt(getRekt.X, getRekt.Y, getRekt.Width, getRekt.Height);

	image.LockBits(&rekt, ImageLockModeWrite, pixelFormat, &data);
	return rekt;
}

void ImageProcessor::HorizontalMirror()
{
	Rect rect = GetRekt(currentImage, BMD);

	for (int y = 0; y < BMD.Height / 2; y++)
	{
		for (int x = 0; x < BMD.Width; x++)
		{
			for (int sub = 0; sub < (BMD.Stride / BMD.Width); sub++)
			{
				*(GetPixelLocation(x, BMD.Height - y, BMD) + sub) = *(GetPixelLocation(x, y, BMD) + sub);
			}
		}
	}
	EndProcess(currentImage, BMD);
}

void ImageProcessor::Rotate()
{
	Rect rect = GetRekt(currentImage, BMD);

	Gdiplus::BitmapData rotatedBMD;
	Gdiplus::Bitmap rotatedImage(BMD.Height, BMD.Width, BMD.PixelFormat);
	Rect rotatedRect = GetRekt(rotatedImage, rotatedBMD);

	for (int x = 0; x < BMD.Width; x++) // column 
	{
		for (int y = 0; y < BMD.Height; y++) // row
		{
			for (int sub = 0; sub < 4; sub++)
			{
				*(GetPixelLocation(rotatedBMD.Width - y - 1, x, rotatedBMD) + sub) = *(GetPixelLocation(x, y, BMD) + sub);
			}
		}
	}
	EndProcess(rotatedImage, rotatedBMD);
}

void ImageProcessor::GrayScale()
{
	Rect rect = GetRekt(currentImage, BMD);
	byte* viewPixel = (byte*)BMD.Scan0;

	for (int y = 0; y < BMD.Height; y++)
	{
		for (int x = 0; x < BMD.Width; x++)
		{
			/*byte avg = ((*(GetPixel(x, y, BMD)) * .114) + ((*(GetPixel(x, y, BMD)) + 1) * .587) + ((*(GetPixel(x, y, BMD)) + 2) * .299) / 3);
			for (int sub = 0; sub < (BMD.Stride / BMD.Width) - 1; sub++)
				*(GetPixel(x, y, BMD) + sub) = avg;*/
			if (pixelFormat == PixelFormat32bppARGB)
			{
				uint8_t oldAlpha = *(GetPixelLocation(x, y, BMD) + 3);
				uint8_t avg = (*(GetPixelLocation(x, y, BMD)) * .114) + (*(GetPixelLocation(x, y, BMD) + 1) * .587) + (*(GetPixelLocation(x, y, BMD) + 2) * .299);
				uint32_t setTo = (oldAlpha << 24) | (avg << 16) | (avg << 8) | (avg);
				SetPixel(x, y, setTo);
			}
			else if (pixelFormat == PixelFormat16bppRGB565)
			{


				float average = (((GetRed(x, y) / 31.0) * .299) + ((GetGreen(x, y) / 63.0) * .587) + ((GetBlue(x, y) / 31.0) * .114));
				uint16_t setTo = PaccPixel((uint16_t)(average * 31), (uint16_t)(average * 63), (uint16_t)(average * 31));

				SetPixel(x, y, setTo);
			}
		}
	}
	EndProcess(currentImage, BMD);
}

uint8_t* ImageProcessor::GetPixelLocation(int x, int y, Gdiplus::BitmapData ImageBMD)
{
	return (uint8_t*)ImageBMD.Scan0 + (ImageBMD.Stride * y) + (x * (ImageBMD.Stride / ImageBMD.Width));
}

uint16_t ImageProcessor::GetRed(int x, int y)
{
	uint16_t mask = 0b1111100000000000;
	return (*((uint16_t*)GetPixelLocation(x, y, BMD)) & mask) >> 11;
}

uint16_t ImageProcessor::GetGreen(int x, int y)
{
	uint16_t mask = 0b0000011111100000;
	return (*((uint16_t*)GetPixelLocation(x, y, BMD)) & mask) >> 5;
}

uint16_t ImageProcessor::GetBlue(int x, int y)
{
	uint16_t mask = 0b0000000000011111;
	return *((uint16_t*)GetPixelLocation(x, y, BMD)) & mask;
}

void ImageProcessor::SetPixel(int x, int y, uint32_t setTo)
{
	*((uint32_t*)(GetPixelLocation(x, y, BMD))) = setTo;
}

void ImageProcessor::SetPixel(int x, int y, uint16_t setTo)
{
	*((uint16_t*)(GetPixelLocation(x, y, BMD))) = setTo;
}

uint16_t ImageProcessor::PaccPixel(uint16_t red, uint16_t green, uint16_t blue)
{
	return (red << 11) | (green << 5) | blue;
}

uint32_t ImageProcessor::PaccPixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	uint32_t setTo = 0b00000000000000000000000000000000;
	return ((setTo | blue) << 24) | ((setTo | green) << 16) | ((setTo | green) << 8) | ((setTo | alpha));
}

void ImageProcessor::Stegosaurus(char* text, int textLength)
{
	// 3 2 3
	int pixelCount = BMD.Width * BMD.Height;
	int possibleSize = pixelCount * 8;
	int textByteSize = textLength * 8;

	if (textByteSize <= possibleSize)
	{
		//for (int x = 0; x < BMD.Width &&  x < textByteSize; x++)
		//{
		//	for (int y = 0; y < BMD.Height; y++)
		//	{
		//		// Backwards in 32 bit!
		//		// Consider writing helper fucntions
		//		uint8_t red = *(GetPixelLocation(x, y, BMD) + 2) | 0b11111000;
		//		uint8_t green = *(GetPixelLocation(x, y, BMD) + 1) | 0b11111100;
		//		uint8_t blue = *(GetPixelLocation(x, y, BMD)) | 0b11111000;

		//		uint8_t character = *(text + (x * y));
		//		red = red | ((character | 0b11100000) >> 5);
		//		green = green | ((character | 0b00011000) >> 3);
		//		blue = blue | ((character | 0b00000111));
		//		uint8_t setTo = PaccPixel(red, green, blue);

		//		*(GetPixelLocation(x, y, BMD)) = setTo;
		//	}
		//}
		byte* 
		while ()

		CLSID encoder;
		GetEncoderClsid(L"image/png", &encoder);
		currentImage.Save(L"C:\\Users\\1014051\\Desktop\\stegosaurus.png", &encoder);
	}
	// Stegosaurus that bad boi
}

void ImageProcessor::Destegosaurus(const wchar_t fileLocation)
{
	
}

void ImageProcessor::EndProcess(Gdiplus::Bitmap &image, Gdiplus::BitmapData &ImageBMD)
{
	image.UnlockBits(&ImageBMD);
	DrawPanel drawPanel(&image);
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
