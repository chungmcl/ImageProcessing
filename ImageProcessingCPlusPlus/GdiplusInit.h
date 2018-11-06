#pragma once

// This class starts up GDI+ in the constructor and shuts down GDI+ in the destructor.
// This should be used in RAII style (https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization).
class GdiplusInit
{
public:
    GdiplusInit();
    ~GdiplusInit();

private:
    ULONG_PTR gdiplusToken_;
};

