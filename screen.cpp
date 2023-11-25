#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>
#include <cstring>
#include <vector>
#include <xdo.h>


void ImageFromDisplay(std::vector<uint8_t> &Pixels, int &Width, int &Height, int &BitsPerPixel)
{
    Display *display = XOpenDisplay(nullptr);
    Window root = DefaultRootWindow(display);

    XWindowAttributes attributes = {0};
    XGetWindowAttributes(display, root, &attributes);

    Width = attributes.width;
    Height = attributes.height;

    XImage *img = XGetImage(display, root, 0, 0, Width, Height, AllPlanes, ZPixmap);
    BitsPerPixel = img->bits_per_pixel;

    int channels = (BitsPerPixel + 7) / 8; // Số lượng kênh màu

    Pixels.resize(Width * Height * channels);

    // Chuyển đổi dữ liệu từ BGRA sang RGBA (hoặc BGR sang RGB)
    for (int i = 0; i < Height; ++i)
    {
        for (int j = 0; j < Width; ++j)
        {
            unsigned long pixel = XGetPixel(img, j, i);
            for (int k = 0; k < channels; ++k)
            {
                Pixels[i * Width * channels + j * channels + k] = (pixel >> (k * 8)) & 0xFF;
            }
        }
    }

    XDestroyImage(img);
    XCloseDisplay(display);
}

int main()
{
    std::string jap =  
}
