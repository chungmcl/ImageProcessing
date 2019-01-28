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

// A FEW NOTES:
// RGBA is ABGR in memory when 32-bit is used
int main()
{
	GdiplusInit init;
	bool isRunning = true;

	while (isRunning)
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
		cout << "R for Rotate" << endl;
		cout << "Q for Quit" << endl;
		if (pixelFormat == PixelFormat32bppARGB)
		{
			cout << "S for Steg(osaurus)anography" << endl;
			cout << "D for De-Steg(osaurus)anography" << endl;
		}
		cout << "Choose an option: ";
			
		char editInput;
		cin >> editInput;

		switch (editInput)
		{
		case 'G':
			imageProcessor.GrayScale();
			break;
		case 'H':
			imageProcessor.HorizontalMirror();
			break;
		case 'R':
			break;
			imageProcessor.Rotate();
		case 'S':
			if (pixelFormat == PixelFormat32bppARGB)
			{
				system("cls");
				cout << "Enter text to hide: ";
				string input;
				cin >> input;
				const char* inputPointer = input.c_str();
				imageProcessor.Stegosaurus(inputPointer, input.length());
			}
			else
			{
				cout << "Unaccepted input - Ending program" << endl;
				isRunning = false;
			}
			break;
		case 'D':
			if (pixelFormat == PixelFormat32bppARGB)
			{
				imageProcessor.Destegosaurus();
			}
			else
			{
				cout << "Unaccepted input - Ending program" << endl;
				isRunning = false;
			}
			break;
		case 'Q':
			isRunning = false;
			break;
		default:
			cout << "Unaccepted input - Ending program" << endl;
			isRunning = false;
		}
	}
}

// Constructor
// Initializes the current image to be processed and in which pixelformat to process it
ImageProcessor::ImageProcessor(const WCHAR* location, Gdiplus::PixelFormat pf) : currentImage(location, false)
{
	pixelFormat = pf;
}

// Acquires the "rect" object which serves as the image
// and initializes BMD which contains data about the image
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

// Horizontally mirrors the image
void ImageProcessor::HorizontalMirror()
{
	Rect rect = GetRekt(currentImage, BMD);

	for (int y = 0; y < BMD.Height / 2; y++) // goes through each line
	{
		for (int x = 0; x < BMD.Width; x++) // goes through each pixel
		{
			for (int sub = 0; sub < (BMD.Stride / BMD.Width); sub++) // goes through each subpixel
			{
				*(GetPixel(x, BMD.Height - y, BMD) + sub) = *(GetPixel(x, y, BMD) + sub);
			}
		}
	}

	EndProcess(currentImage, BMD);
}

// Rotates image 90 degrees to the right
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
				*(GetPixel(rotatedBMD.Width - y - 1, x, rotatedBMD) + sub) = *(GetPixel(x, y, BMD) + sub);
			}
		}
	}
	EndProcess(rotatedImage, rotatedBMD);
}

// Display image in grayscale
void ImageProcessor::GrayScale()
{
	Rect rect = GetRekt(currentImage, BMD);
	byte* viewPixel = (byte*)BMD.Scan0;

	for (int y = 0; y < BMD.Height; y++)
	{
		for (int x = 0; x < BMD.Width; x++)
		{
			if (pixelFormat == PixelFormat32bppARGB)
			{
				uint8_t oldAlpha = *(GetPixel(x, y, BMD) + 3);
				uint8_t avg = (*(GetPixel(x, y, BMD)) * .114) + (*(GetPixel(x, y, BMD) + 1) * .587) + (*(GetPixel(x, y, BMD) + 2) * .299);
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

// Returns a reference to the byte of the specified pixel and position x and y of the image
byte* ImageProcessor::GetPixel(int x, int y, Gdiplus::BitmapData ImageBMD)
{
	return (byte*)ImageBMD.Scan0 + (ImageBMD.Stride * y) + (x * (ImageBMD.Stride / ImageBMD.Width));
}

// Returns the red component of a pixel in an image of a 16 bit format
uint16_t ImageProcessor::GetRed(int x, int y)
{
	uint16_t mask = 0b1111100000000000;
	return (*((uint16_t*)GetPixel(x, y, BMD)) & mask) >> 11;
}

// Returns the green component of a pixel in an image of a 16 bit format
uint16_t ImageProcessor::GetGreen(int x, int y)
{
	uint16_t mask = 0b0000011111100000;
	return (*((uint16_t*)GetPixel(x, y, BMD)) & mask) >> 5;
}

// Returns the blue component of a pixel in an image of a 16 bit format
uint16_t ImageProcessor::GetBlue(int x, int y)
{
	uint16_t mask = 0b0000000000011111;
	return *((uint16_t*)GetPixel(x, y, BMD)) & mask;
}

// Sets the pixel at position x and y of an image in a 32 bit format
void ImageProcessor::SetPixel(int x, int y, uint32_t setTo)
{
	*((uint32_t*)(GetPixel(x, y, BMD))) = setTo;
}

// Sets the pixel at position x and y of an image in a 16 bit format
void ImageProcessor::SetPixel(int x, int y, uint16_t setTo)
{
	*((uint16_t*)(GetPixel(x, y, BMD))) = setTo;
}

// Combines the red, green, and blue components of three separate 16 bit ints into a single 16 bit int
uint16_t ImageProcessor::PaccPixel(uint16_t red, uint16_t green, uint16_t blue)
{
	return (red << 11) | (green << 5) | blue;
}

// Combines the red, green, and blue components of three separate 32 bit ints into a single 32 bit int
uint32_t ImageProcessor::PaccPixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	// It could be declared as just 'setTo = 0' but for the code reader's sake we left in 32 zeros hehe_xD.RAR
	uint32_t setTo = 0b00000000000000000000000000000000;
	return ((setTo | blue) << 24) | ((setTo | green) << 16) | ((setTo | red) << 8) | ((setTo | alpha));
}

// Given a string of text and its length,
// Hides the text in an image loaded in a 32 bit format
// Each byte of the text is split into the R, G, and B components of eaxh pixel
// 3 bits for red, 2 bits for green, and 3 bits for blue
// A header that spans 4 pixels details the length of the text as a 32 bit integer, so that
// when the image is "Desteganographized," it can know when to stop reading
//
// "Steganography"
void ImageProcessor::Stegosaurus(const char* text, const int textLength)
{
	// 4 pixels (32 bits) of header data detailing size of text
	Rect rect = GetRekt(currentImage, BMD);
	// 3 2 3
	int pixelCount = BMD.Width * BMD.Height;
	int possibleSize = pixelCount * 8;
	int textByteSize = textLength * 8;

	uint32_t headerData = textByteSize;
	byte* textWithHeader = new byte[textLength + 4];
	textWithHeader[0] = headerData & 0b11111111;
	textWithHeader[1] = headerData & 0b1111111100000000;
	textWithHeader[2] = headerData & 0b111111110000000000000000;
	textWithHeader[3] = headerData & 0b11111111000000000000000000000000;
	for (int i = 0; i < textLength; i++)
	{
		textWithHeader[i + 4] = text[i];
	}

	if (textByteSize + headerData <= possibleSize)
	{
		int x = 0;
		int y = 0;
		for (int i = 0; i < textByteSize + headerData; i++)
		{
			if (x == BMD.Width)
			{
				y++;
				x = 0;
			}

			// Declare the RGBA and remove the first three least significant bits
			uint8_t red = *(GetPixel(x, y, BMD) + 2) & 0b11111000;
			uint8_t green = *(GetPixel(x, y, BMD) + 1) & 0b11111100;
			uint8_t blue = *(GetPixel(x, y, BMD)) & 0b11111000;
			// Alpha is unused for storing hidden bits
			// Declare for the PaccPixel method
			uint8_t alpha = *(GetPixel(x, y, BMD) + 3);


			uint8_t byteData = *(textWithHeader + i);

			// Add the current byteData (aka the character) to RGB,
			// Take the 3 left-most bits of the 8 bit character and push it to the far right, and add it to red
			// Take the middle 2 bits of the 8 bit character and push it to the far right, and add it to blue
			// Take the 3 right-most bits of the 8 bit character (don't need to move since it's already in the far right), and add it to green
			red = red | ((byteData & 0b11100000) >> 5);
			green = green | ((byteData & 0b00011000) >> 3);
			blue = blue | ((byteData & 0b00000111));
			uint32_t setTo = PaccPixel(red, green, blue, alpha);

			SetPixel(x, y, setTo);

			x++;
		}

		CLSID encoder;
		GetEncoderClsid(L"image/png", &encoder);
		(currentImage).Save(L"C:\\Users\\chung\\Desktop\\stegosaurus.png", &encoder);
	}
	// Stegosaurus that bad boi
}

// Reads hidden text in an image that is hidden in the format specified in Stegosaurus (Steganography)
//
// "De-Steganography" 
void ImageProcessor::Destegosaurus()
{
	Rect rect = GetRekt(currentImage, BMD);
	uint32_t size = 0;
	// Loop through four bytes of data
	// Stored a 32 bit integer describing the size of the text over four bytes
	// 8 bits per pixel

	for (int i = 0; i < 4; i++)
	{
		uint8_t quarterOfSizeData = PaccPixelDestegosaurus323ARGB(i, 0);
		size = size | (quarterOfSizeData << (i * 8));
	}

	char* text = new char[size];
	int x = 3;
	int y = 0;
	for (int i = 0; i < size; i++)
	{
		if (x == BMD.Width)
		{
			y++;
			x = 0;
		}
		uint8_t characterBytes = PaccPixelDestegosaurus323ARGB(x, y);
		text[i] = characterBytes;
		x++;
	}
	cout << text << endl;
}

// Concatenates the byte (char) back together in pixel at location x, y
// for Destegasaurus (De-Steganography)
uint8_t ImageProcessor::PaccPixelDestegosaurus323ARGB(int x, int y)
{
	uint8_t red = *(GetPixel(x, y, BMD) + 2) & 0b00000111;
	uint8_t green = *(GetPixel(x, y, BMD) + 1) & 0b00000011;
	uint8_t blue = *(GetPixel(x, y, BMD)) & 0b00000111;
	return (blue << 5) | (green << 3) | (red);
}

// Unlocks bits of image and draws image in drawpanel
void ImageProcessor::EndProcess(Gdiplus::Bitmap &image, Gdiplus::BitmapData &ImageBMD)
{
	image.UnlockBits(&ImageBMD);
	DrawPanel drawPanel(&image);
	drawPanel.MessageLoop();
}

// MSDN magic code that helps encode image to specific format (.jpg, .png, etc)
// When image is saved
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