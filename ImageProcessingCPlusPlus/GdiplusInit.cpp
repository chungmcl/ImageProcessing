#include "pch.h"
#include "GdiplusInit.h"

using namespace Gdiplus;

GdiplusInit::GdiplusInit()
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken_, &gdiplusStartupInput, NULL);
}

GdiplusInit::~GdiplusInit()
{
    GdiplusShutdown(gdiplusToken_);
}
