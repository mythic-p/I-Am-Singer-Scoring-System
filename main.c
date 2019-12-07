#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#define WIDTH 80
#define HEIGHT 31

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

enum State
{
	MainMenu,
	DisplayMessage,
	InputData,
	DisplaySingerData,
	DisplaySortingResult
} curState = MainMenu, prevState = MainMenu;
typedef enum State State;

HANDLE hOut;

Singer *singers, *sortedSingers;
int singerAmount = 0;

char messages[100];

void initSystem();// 初始化系统设定，设置控制台大小
void displayMenu();// 显示系统主菜单
void inputSingerData();// 录入歌手数据
void displaySingerData();// 显示歌手数据 
void displayMessage(char* message);
void toCurState();

void setCursorVisible(int visible);
void setCursorPosition(int x, int y);

void drawBorder();// 绘制软件画面的边框 
void drawWindow(int x, int y, int width, int height);// 在控制台上绘制一个窗口
void drawText(int x, int y, char* text);

int main()
{
	initSystem();
	displayMenu();
	return 0;
}
// 显示主菜单 
void displayMenu()
{
	drawBorder();
	drawWindow(24, 2, 35, 3);
	drawText(32, 3, "我是歌手比赛打分系统");
	drawWindow(26, 6, 30, 3);
	drawText(36, 7, "1.输入信息");
	drawWindow(26, 10, 30, 3);
	drawText(36, 11, "2.显示信息");
	drawWindow(26, 14, 30, 3);
	drawText(36, 15, "3.结果排序");
	drawWindow(26, 18, 30, 3);
	drawText(36, 19, "4.保存信息");
	drawWindow(26, 22, 30, 3);
	drawText(36, 23, "5.显示结果");
	drawWindow(26, 26, 30, 3);
	drawText(36, 27, "6.退出系统");
	char c = getch();
	switch (c)
	{
		case '1':
			prevState = MainMenu;
			curState = InputData;
			inputSingerData();
			break;
	}
}
void inputSingerData()
{
	drawBorder();
	printf("请输入歌手的数量：");
	setCursorVisible(1);
	scanf("%d", &singerAmount);
	if (singerAmount <= 0)
	{
		prevState = MainMenu;
		displayMessage("歌手数量只能为正整数！");
	}
	else if (singerAmount >= 20)
	{
		prevState = MainMenu;
		displayMessage("歌手数量不能大于20！");
	}
	else
	{
		singers = (Singer*)calloc(singerAmount, sizeof(Singer));
		sortedSingers = (Singer*)calloc(singerAmount, sizeof(Singer));
		drawBorder();
		printf("请输入歌手所唱的歌曲数：");
		setCursorVisible(1);
		int num;
		scanf("%d", &num);
		if (num <= 0)
		{
			prevState = MainMenu;
			displayMessage("歌曲数只能为正整数！");
		}
		else if (num > 10)
		{
			prevState = MainMenu;
			displayMessage("歌曲数不能超过10首！");
		}
		else
		{
			for (int i = 0; i < singerAmount; i++)
			{
				drawBorder();
				setCursorVisible(1);
				singers[i].sings = (Sing*)calloc(num, sizeof(Sing));
				singers[i].singAmount = num;
				sortedSingers[i].sings = (Sing*)calloc(num, sizeof(Sing));
				sortedSingers[i].singAmount = num;
				printf("请输入第%d位歌手的名字（不超过5个字符）：", i + 1);
				scanf("%s", singers[i].name);
				strcpy(sortedSingers[i].name, singers[i].name);
				for (int j = 0; j < num; j++)
				{
					drawBorder();
					setCursorVisible(1);
					printf("请输入第%d首歌曲的名称，评委打分平均分和大众评分，用空格隔开：", j + 1);
					scanf("%s %lf %lf", singers[i].sings[j].name, &(singers[i].sings[j].judgerScoreAvg), &(singers[i].sings[j].audienceScoreAvg));
					strcpy(sortedSingers[i].sings[j].name, singers[i].sings[j].name);
					sortedSingers[i].sings[j].judgerScoreAvg = singers[i].sings[j].judgerScoreAvg;
					sortedSingers[i].sings[j].audienceScoreAvg = singers[i].sings[j].audienceScoreAvg;
				}
			}
			displayMessage("输入完毕！"); 
		}
	}
}
void displaySingerInfo()
{
	
}
// 显示一条信息
// message: char* 用作显示的字符串 
void displayMessage(char* message)
{
	drawBorder();
	int len = strlen(message) / 2;
	setCursorPosition(WIDTH / 2 - len, 2);
	printf("%s", message);
	setCursorPosition(WIDTH / 2 - 3, 4);
	printf("按任意键返回");
	getch();
	curState = prevState;
	prevState = DisplayMessage;
	toCurState();
}
void toCurState()
{
	switch (curState)
	{
		case MainMenu:
			displayMenu();
			break;
		case DisplayMessage:
			displayMessage("");
			break;
		case InputData:
			inputSingerData();
			break;
	}
}
// 初始化系统设置 
void initSystem()
{
	char tmp[50];
	sprintf(tmp, "mode con cols=%d lines=%d", WIDTH, HEIGHT + 1);
	system(tmp);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	setCursorVisible(0);
}
void drawBorder()
{
	system("CLS");
	setCursorVisible(0);
	drawWindow(0, 0, WIDTH, HEIGHT);
}
// 在控制台画面上绘制一个窗口
// x: int 从x坐标开始往右绘制
// y: int 从y坐标开始往下绘制
// width: int 绘制的窗口的宽度
// height: int 绘制的窗口的高度 
void drawWindow(int x, int y, int width, int height)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < width; j++)
		{
			setCursorPosition(x + j, y + (height - 1) * i);
			printf("*");
		}
		for (int j = 1; j < height - 1; j++)
		{
			setCursorPosition(x + (width - 1) * i, y + j);
			printf("*");
		}
	}
	setCursorPosition(1, 1);
}
void drawText(int x, int y, char* text)
{
	setCursorPosition(x, y);
	printf(text);
}
// 对于内置函数SetConsoleCursorInfo的一个封装
// visible: int 是否显示输入光标，1为显示，0为不显示 
void setCursorVisible(int visible)
{
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(hOut, &info);
	info.bVisible = visible;
	SetConsoleCursorInfo(hOut, &info);
}
// 对于内置函数SetConsoleCursorPosition的一个封装
// position: COORD 设置光标在控制台上的坐标 
void setCursorPosition(int x, int y)
{
	COORD tempCoord = {x, y};
	SetConsoleCursorPosition(hOut, tempCoord);
}
