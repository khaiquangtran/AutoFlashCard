#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types.hpp>
#include "ScreenShot.h"
#include "Points.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <regex>
#include <unistd.h>
extern "C"
{
#include <xdo.h>
}

int main(int argc, char *argv[])
{
	int duration = 500;
	std::vector<std::string> alpha;
	std::vector<std::string> strComp;
	for (int i = 1; i < argc; i++)
	{
		// alpha.push_back(argv[i]);
		strComp.push_back(argv[i]);
	}

	// for (std::string str : strComp)
	// {
	// 	std::cout << str << std::endl;
	// }
	
	xdo_t *xdo = xdo_new(NULL);
	if (!xdo)
	{
		std::cerr << "Không thể khởi tạo thư viện xdo." << std::endl;
		return 1;
	}
	int x = 1517;
	int y = 231;
	xdo_move_mouse(xdo, x, y, 0);
	xdo_click_window(xdo, CURRENTWINDOW, 1);
	usleep(500000);
	int Width = 0;
	int Height = 0;
	int Bpp = 0;
	std::vector<std::uint8_t> Pixels;
	// Tạo một đối tượng Tesseract OCR
	tesseract::TessBaseAPI tess;
	if (tess.Init(NULL, "vie+jpn"))
	{ // Sử dụng ngôn ngữ tiếng Anh ("eng") - thay đổi thành ngôn ngữ bạn cần
		std::cerr << "Không thể khởi tạo Tesseract OCR!" << std::endl;
		return -1;
	}

	ImageFromDisplay(Pixels, Width, Height, Bpp);
	cv::Mat image0;
	if (Width && Height)
	{
		image0 = cv::Mat(Height, Width, Bpp > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]);
	}
	if (image0.empty())
	{
		std::cerr << "Không thể đọc được ảnh!" << std::endl;
		return -1;
	}
	cv::Mat image;
	std::vector<std::string> result;
	for (int i = 0; i < 12; i++)
	{
		image = image0(cv::Range(vecColRaw.at(i).col.x, vecColRaw.at(i).col.y), cv::Range(vecColRaw.at(i).raw.x, vecColRaw.at(i).raw.y));
		cv::imshow("Image1", image);
		cv::waitKey(0);
		cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
		tess.SetImage(image.data, image.cols, image.rows, 1, image.cols);
		std::string text1 = tess.GetUTF8Text();
		std::cout << text1 << std::endl;
		std::string text;
		for (char c : text1)
		{
			if (c != '\n')
			{
				if (containsJapanese(text) == true)
				{
					if (c != ' ')
					{
						text += c;
					}
				}
				else
				{
					text += c;
				}
			}
		}
		std::string text2;
		if (containsJapanese(text) == false)
		{
			size_t index = text.find(" ");
			if (index != std::string::npos)
			{
				size_t index2 = text.find(" ", index + 1);
				if (index2 != std::string::npos)
				{
					text = text.substr(0, index2);
				}
			}
			for (char c : text)
			{
				if (c != ' ')
				{
					text2 += c; 
				}
			}
		}
		else
		{
			for (char c : text)
			{
				if (c != '(' && c != ')')
				{
					text2 += c; 
				}
			}
		}
		alpha.push_back(text2);
	}
	for (std::string str : alpha)
	{
		std::cout << str << std::endl;
	}

	for (int loc = 0; loc < strComp.size(); loc++)
	{
		for (int x = 0; x < alpha.size(); x++)
		{
			if (strComp.at(loc) == alpha.at(x))
			{
				// std::cout << strComp.at(loc) << " : x = " << coor.at(x).x << " y = " << coor.at(x).y << std::endl;
				xdo_move_mouse(xdo, coor.at(x).x, coor.at(x).y, 0);
				xdo_click_window(xdo, CURRENTWINDOW, 1);
				usleep(duration);
				for (int y = 0; y < alpha.size(); y++)
				{
					if (strComp.at(loc + 1) == alpha.at(y))
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
