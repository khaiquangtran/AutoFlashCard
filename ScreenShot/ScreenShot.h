#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>
#include <cstring>
#include <vector>
#include <iostream>
#include <codecvt>
#include <locale>

void ImageFromDisplay(std::vector<uint8_t> &Pixels, int &Width, int &Height, int &BitsPerPixel);
bool containsJapanese(const std::string &str);
void sort(std::vector<size_t> &index, std::vector<std::string> &str);