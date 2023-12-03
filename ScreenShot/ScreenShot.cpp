#include "ScreenShot.h"

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

bool containsJapanese(const std::string &str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide = converter.from_bytes(str);

	for (wchar_t wc : wide)
	{
		// Kiểm tra các khoảng mã Unicode cho tiếng Nhật
		if ((wc >= 0x3000 && wc <= 0x9FFF) || // Phạm vi chữ Hán và Katakana
			(wc >= 0xFF66 && wc <= 0xFF9F) || // Phạm vi bổ sung Katakana
			(wc >= 0x4E00 && wc <= 0x9FBF) || // Phạm vi Kanji
			(wc >= 0xF900 && wc <= 0xFAFF))
		{
			return true;
		}
	}
	return false;
}

void sort(std::vector<size_t> &index, std::vector<std::string> &str)
{
	for (int i = 0; i < index.size(); i++)
	{
		std::cout << str.at(i) << " " << index.at(i) << std::endl;
	}

	size_t temp = 0;
	std::string strTemp;
	for (int i = 0; i < index.size() - 1; i++)
	{
		for (int j = i; j < index.size(); j++)
		{
			if (index.at(i) > index.at(j))
			{
				temp = index.at(i);
				index.at(i) = index.at(j);
				index.at(j) = temp;

				strTemp = str.at(i);
				str.at(i) = str.at(j);
				str.at(j) = strTemp;
			}
		}
	}
}