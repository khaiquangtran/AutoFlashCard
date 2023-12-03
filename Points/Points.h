#include <vector>
#include <iostream>

struct Point
{
	int x;
	int y;
};

struct ColRaw
{
    Point col;
    Point raw;
};

std::vector<Point> coor =
		{
			{460, 418},
			{896, 426},
			{1303, 436},
			{1729, 424},
			{489, 643},
			{843, 648},
			{1242, 651},
			{1685, 647},
			{487, 893},
			{850, 897},
			{1310, 898},
			{1688, 891},
		};
int distance = 25;
int height = 205;
int width = 390;
int startCol = 335;
int startRaw = 275;

Point Col1{startCol, startCol + height}; 
Point Raw1{startRaw, startRaw + width}; 

Point Col2{Col1.x , Col1.y}; 
Point Raw2{Raw1.y + distance, Raw1.y + distance + width}; 

Point Col3{Col2.x , Col2.y}; 
Point Raw3{Raw2.y + distance, Raw2.y + distance + width}; 

Point Col4{Col3.x , Col3.y}; 
Point Raw4{Raw3.y + distance, Raw3.y + distance + width}; 

Point Col5{Col1.y + distance, Col1.y + distance + height}; 
Point Raw5{Raw1.x , Raw1.y}; 

Point Col6{Col5.x , Col5.y}; 
Point Raw6{Raw5.y + distance, Raw5.y + distance + width}; 

Point Col7{Col6.x , Col6.y}; 
Point Raw7{Raw6.y + distance, Raw6.y + distance + width}; 

Point Col8{Col7.x , Col7.y}; 
Point Raw8{Raw7.y + distance, Raw7.y + distance + width}; 

Point Col9{Col5.y + distance, Col5.y + distance + height}; 
Point Raw9{Raw5.x , Raw5.y}; 

Point Col10{Col9.x , Col9.y}; 
Point Raw10{Raw9.y + distance, Raw9.y + distance + width}; 

Point Col11{Col10.x , Col10.y}; 
Point Raw11{Raw10.y + distance, Raw10.y + distance + width}; 

Point Col12{Col11.x , Col11.y}; 
Point Raw12{Raw11.y + distance, Raw11.y + distance + width}; 

std::vector<ColRaw> vecColRaw
{
    {Col1, Raw1},
    {Col2, Raw2},
    {Col3, Raw3},
    {Col4, Raw4},
    {Col5, Raw5},
    {Col6, Raw6},
    {Col7, Raw7},
    {Col8, Raw8},
    {Col9, Raw9},
    {Col10, Raw10},
    {Col11, Raw11},
    {Col12, Raw12},
};
