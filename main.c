#include <stdio.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 20

struct Sing
{
	char name[10];
	double judgerScoreAvg;
	double audienceScoreAvg;
};
typedef struct Sing Sing;

struct Singer
{
	char name[5];
	Sing* sings;
	int singAmount;
};
typedef struct Singer Singer;

struct Position
{
	int x, y;
};
typedef struct Position Position;



void initSystem();// 初始化系统设定，设置控制台大小 
void displayMenu();// 显示系统主菜单 

void drawWindow(int x, int y, int width, int height);

int main()
{
	initSystem();
	displayMenu();
	return 0;
}

void displayMenu()
{
	system("CLS");
	drawWindow(0, 0, WIDTH, HEIGHT);
	
	char c = getchar();
	
}

void initSystem()
{
	char tmp[50];
	sprintf(tmp, "mode con cols=%d lines=%d", WIDTH, HEIGHT);
	system(tmp);
}

void drawWindow(int x, int y, int width, int height)
{
	for (int i = 0; i < )
}
