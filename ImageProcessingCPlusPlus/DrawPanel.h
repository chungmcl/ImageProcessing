#pragma once

// The DrawPanel class shows a window that displays an image when it's instantiated. 
// It can be instantiated multiple times. The windows can be closed with the "X" button
// or by pressing ESC.
class DrawPanel
{
public:
    // Create an empty DrawPanel.
    DrawPanel();

    // Creates a DrawPanel with the initial image.
    DrawPanel(Gdiplus::Image *image);

    virtual ~DrawPanel();

    // Sets the image to be displayed. Ownership of the Image remains with the caller.
    void SetImage(Gdiplus::Image *image);

    // The main program should call this function to enter the message loop and make
    // the draw panels interactive. When any of the windows are closed, the message
    // loop ends and the main program can choose to exit (or not).
    static WPARAM MessageLoop();

protected:
    // Draws the image on to the window.
    // Override if you want to draw something else.
    virtual void OnPaint(HDC hdc);

private:
    static bool RegisterWindowClassOnce();
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

    HWND hwnd_;
    Gdiplus::Image *image_;
};

