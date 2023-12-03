#include <iostream>
#include <vector>

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
    for (std::string str : strComp)
	{
		std::cout << str << std::endl;
	}
}