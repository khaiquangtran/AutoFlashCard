#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>
#include <cstring>
#include <vector>
#include <iostream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <locale>
#include <codecvt>
#include <regex>
#include <unistd.h>
extern "C"
{
#include <xdo.h>
}

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

struct Point
{
	std::uint16_t x;
	std::uint16_t y;
};

void sort(std::vector<size_t> &index, std::vector<std::string> &str)
{
	for (int i = 0; i < index.size(); i++)
	{
		std::cout <<str.at(i)<< " "<<index.at(i)<< std::endl;
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

int main(int argc, char *argv[])
{
	int duration = 500;

	std::vector<std::string> alpha;
	std::vector<std::string> alpha2;
	std::vector<std::string> strComp;
	for (int i = 1; i < argc; i++)
	{
		alpha.push_back(argv[i]);
		strComp.push_back(alpha.at(i - 1));
	}
	xdo_t *xdo = xdo_new(NULL);
	if (!xdo)
	{
		std::cerr << "Không thể khởi tạo thư viện xdo." << std::endl;
		return 1;
	}
	int x = 1747;
	int y = 293;
	xdo_move_mouse(xdo, x, y, 0);
	xdo_click_window(xdo, CURRENTWINDOW, 1);
	usleep(800000);
	int Width = 0;
	int Height = 0;
	int Bpp = 0;
	std::vector<std::uint8_t> Pixels;
	std::vector<Point> coor =
		{
			{557, 516},
			{938, 506},
			{1330, 502},
			{1710, 516},
			{568, 731},
			{944, 713},
			{1342, 698},
			{1701, 709},
			{567, 929},
			{932, 921},
			{1259, 943},
			{1684, 941},
			};

	ImageFromDisplay(Pixels, Width, Height, Bpp);
	cv::Mat image0;
	if (Width && Height)
	{
		image0 = cv::Mat(Height, Width, Bpp > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]);
	}

	cv::Mat image = image0(cv::Range(400, 1050), cv::Range(370, 1900));

	if (image.empty())
	{
		std::cerr << "Không thể đọc được ảnh!" << std::endl;
		return -1;
	}

	// Chuyển đổi ảnh sang ảnh xám để dễ dàng xử lý
	cv::Mat gray;
	// cv::GaussianBlur(image, image, cv::Size(5, 5), 0); 
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	// threshold(image, gray, 150, 255, 0);
	//  cv::imshow("hi", gray);
	// cv::waitKey(0);
	// Tạo một đối tượng Tesseract OCR
	tesseract::TessBaseAPI tess;
	if (tess.Init(NULL, "jpn"))
	{ // Sử dụng ngôn ngữ tiếng Anh ("eng") - thay đổi thành ngôn ngữ bạn cần
		std::cerr << "Không thể khởi tạo Tesseract OCR!" << std::endl;
		return -1;
	}
	// Đưa ảnh vào Tesseract để nhận diện văn bản
	tess.SetImage(gray.data, gray.cols, gray.rows, 1, gray.cols);
	// Chuyển đổi chuỗi UTF-8 sang chuỗi std::string
	std::string text = tess.GetUTF8Text();
	std::string result;
	for (char c : text)
	{
		if (c != ' ' && c != '\n')
		{
			result += c;
		}
	}

	std::string tempResult;
	for (char c : result)
	{
		switch (c)
		{
		// case '9':
		// case 'd': {tempResult += 'g';break;}
		case 'U':
		{
			tempResult += 'u';
			break;
		}
		case 'S':
		{
			tempResult += 's';
			break;
		}
		case 'Z':
		{
			tempResult += 'z';
			break;
		}
		case 'P':
		{
			tempResult += 'p';
			break;
		}
		case 'D':
		{
			tempResult += 'p';
			break;
		}
		case 'T':
		{
			tempResult += 'r';
			break;
		}
		case 'Y':
		{
			tempResult += 'y';
			break;
		}
		case 'W':
		{
			tempResult += 'w';
			break;
		}
		case '0':
		{
			tempResult += 'o';
			break;
		}
		case 'O':
		{
			tempResult += 'o';
			break;
		}
		default:
		{
			tempResult += c;
			break;
		}
		}
	}
	result = tempResult;

	std::vector<size_t> vecIndex;
	int count = 0;
	for (size_t i = 0; i < alpha.size() ; i++)
	{
		 if(result.find(alpha.at(i)) != std::string::npos && count < 12)
		{
			size_t index = result.find(alpha.at(i));
			alpha2.push_back(alpha.at(i));
			vecIndex.push_back(index);
			count ++;
		}
		
	}
	sort(vecIndex, alpha2);
	// std::cout << "alpha " << std::endl;
	// for (std::string c : alpha)
	// {
	// 	std::cout << c << std::endl;
	// }
	// std::cout << "\n strComp " << std::endl;
	// for (std::string c : strComp)
	// {
	// 	std::cout << c << std::endl;
	// }
	std::cout << result << std::endl;
	for (int loc = 0; loc < strComp.size(); loc++)
	{
		for (int x = 0; x < alpha2.size(); x++)
		{
			if (strComp.at(loc) == alpha2.at(x))
			{
				// std::cout << strComp.at(loc) << " : x = " << coor.at(x).x << " y = " << coor.at(x).y << std::endl;
				xdo_move_mouse(xdo, coor.at(x).x, coor.at(x).y, 0);
				xdo_click_window(xdo, CURRENTWINDOW, 1);
				usleep(duration);
				for (int y = 0; y < alpha2.size(); y++)
				{
					if (strComp.at(loc + 1) == alpha2.at(y))
					{
						// std::cout << strComp.at(loc + 1) << " : x = " << coor.at(y).x << " y = " << coor.at(y).y << std::endl;
						xdo_move_mouse(xdo, coor.at(y).x, coor.at(y).y, 0);
						xdo_click_window(xdo, CURRENTWINDOW, 1);
						usleep(duration);
						break;
					}
				}
			}
		}
		loc++;
	}
	
	tess.End();
	xdo_free(xdo);
	return 0;
}
