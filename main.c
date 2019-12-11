#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#define WIDTH 108 // 定义控制台宽度 
#define HEIGHT 39 // 定义控制台高度 
#define MAX_SINGERS 100 // 设置初始歌手数组的容量 

// 定义歌曲结构体 
struct Song
{
	char name[5]; // 歌曲名称，4个字节 
	int judgerScores[5]; // 五位评委的打分
	int audienceScore; // 大众打分
	int totalScore; // 总分 
};
typedef struct Song Song;// 重新定义歌曲结构体的声明方式，更加便捷。 
// 定义歌手结构体 
struct Singer
{
	int id; // 歌手的编号 
	char name[5]; // 歌手的名称 
	Song song; // 歌手所唱的歌曲 
};
typedef struct Singer Singer;// 重新定义歌手结构体的声明方式，更加便捷。 
// 定义程序状态枚举 
enum State
{
	MainMenu, // 程序在主菜单 
	DisplayMessage, // 程序在显示信息窗口 
	InputData, // 程序在输入歌手信息 
	DisplaySingerData, // 程序在显示歌手信息 
	InsertData, // 程序在添加歌手信息
	DeleteData, // 程序在删除歌手信息 
} curState = MainMenu, prevState = MainMenu; // 定义当前程序状态变量和上一个程序状态 
typedef enum State State;// 重新定义状态枚举的声明方式，更加便捷。 

// 输出方向用的枚举类型 
enum Direction
{
	HorizontalLeft, // 从右往左输出 
	HorizontalRight, // 从左往右输出 
	VerticalUp, // 从下往上输出 
	VerticalDown // 从上往下输出 
};
typedef enum Direction Direction; // 重命名方向枚举 使用时更加方便 

HANDLE hOut; // 声明控制台句柄变量
// 将主菜单选项的文本存到一个数组中，用循环来显示主菜单，更加简洁 
char menuChoicesStr[][15] = {"1.输入歌手信息", "2.输出歌手信息", "3.成绩排序", "4.保存歌手信息", "5.添加歌手信息", "6.查询歌手信息", "7.删除歌手信息", "8.退出系统"};

Singer singers[MAX_SINGERS]; // 声明歌手数组，由MAX_SINGERS决定初始化数组的大小，如果超过该大小，会重新申请内存空间，保证不会数组越界。 
int singerAmount = 0; // 当前已经被记录的歌手数量  
int curPage = 0, maxPage = 0;// 显示歌手信息的时候用到的变量，分别表示当前是第几页，总共有多少页，因为歌手可能很多，一次显示不全，所以采用分页 

void initSystem();// 初始化系统设定，设置控制台大小
void displayMenu();// 显示系统主菜单
void inputSingerData();// 录入歌手数据
void displaySingerInfo();// 显示歌手数据 
void displayMessage(char* message);// 将程序转到显示信息窗口，显示一条信息，内容为message 
void insertSingerInfo(); // 添加歌手信息 
void searchSingerInfo(); // 查询歌手信息 
void deleteSingerInfo(); // 删除歌手信息 
void toCurState(); // 让程序转到当前的状态所在的画面 
void sortSingers(); // 对已有的歌手信息进行排序 
void saveSingersInfo(); // 储存歌手的信息 
void sendWarn(char* warnMsg); // 发送一个警告，但不返回主菜单 
int checkSingerExists(int id); // 寻找一个指定编号的歌手是否在数据库中 如果存在则返回其在数据库中的索引 不存在则返回-1 
int getMin(int a, int b);// 比大小函数，返回小的那一个 

void setCursorVisible(int visible);// 设置控制台的光标是否可见 
void setCursorPosition(int x, int y);// 设置控制台光标的坐标 

void drawBorder();// 绘制软件画面的边框 
void drawWindow(int x, int y, int width, int height);// 在控制台上绘制一个窗口
void drawText(int x, int y, char* text);// 以从左往右的顺序在(x, y)处输出一段文本text
void drawChar(int x, int y, char c);// 在(x, y)处输出一个字符c 
void drawLine(int x, int y, Direction direction, int len, char c);// 在(x, y)处，横向或纵向，输出len个相同的字符 
void drawGridHeader(); // 将表格的表头绘制到控制台上 
void drawGridSingerInfo(int row, Singer singer); // 将歌手的全部信息输出到表格中 

int main()
{
	initSystem();
	displayMenu();
	return 0;
}
// 显示主菜单 
void displayMenu()
{
	// 每次回到主菜单 会根据已有歌手的数量重新计算总共有多少页 
	maxPage = singerAmount / 17 + 1;
	int offset = 13;
	drawBorder();
	drawWindow(24 + offset, 2, 35, 3);
	drawText(32 + offset, 3, "我是歌手比赛打分系统");
	// 输出7个命令按钮 
	for (int i = 0; i < 8; i++)
	{
		drawWindow(24 + offset, 6 + 4 * i, 35, 3);
		drawText(34 + offset, 7 + 4 * i, menuChoicesStr[i]);
	}
	// 获取用户按键 
	char c = getch();
	switch (c)
	{
		case '1':
			prevState = MainMenu;
			curState = InputData;
			inputSingerData();
			break;
		case '2':
			prevState = MainMenu;
			curState = DisplaySingerData;
			displaySingerInfo();
			break;
		case '3':
			if (singerAmount > 0)
			{
				sortSingers();
				prevState = MainMenu;
				displayMessage("排序成功！");
			}
			else
			{
				prevState = MainMenu;
				displayMessage("当前没有歌手信息！");
			}
			break;
		case '4':
			if (singerAmount > 0)
			{
				saveSingersInfo();
				prevState = MainMenu;
				displayMessage("保存成功！");
			}
			else
			{
				prevState = MainMenu;
				displayMessage("当前没有歌手信息！");
			}
			break;
		case '5':
			insertSingerInfo();
			break;
		case '6':
			if (singerAmount > 0)
			{
				searchSingerInfo();
			}
			else
			{
				prevState = MainMenu;
				displayMessage("当前没有歌手信息！"); 
			}
			break;
		case '7':
			if (singerAmount > 0)
			{
				deleteSingerInfo();
			}
			else
			{
				prevState = MainMenu;
				displayMessage("当前没有歌手信息！"); 
			}
			break;
		case '8':
			break;
		default:
			displayMenu();
	}
}
// 输入歌手信息 
void inputSingerData()
{
	drawBorder();
	printf("请输入歌手的数量：");
	setCursorVisible(1);
	scanf("%d", &singerAmount);
	// 输入数据是否合法 
	if (singerAmount <= 0)
	{
		prevState = MainMenu;
		displayMessage("输入的歌手数量只能为正整数！");
	}
	else if (singerAmount >= MAX_SINGERS)
	{
		prevState = MainMenu;
		char tmp[30];
		sprintf(tmp, "一次输入的歌手数量不能大于%d！", MAX_SINGERS);
		displayMessage(tmp);
	}
	else
	{
		for (int i = 0; i < singerAmount; i++)
		{
			int _ok = 0;
			// 这里如果输入的所有数据都合法，会进入下一次循环，否则会要求重新输入该歌手的数据一遍 
			while (!_ok)
			{
				_ok = 1;
				int cnt = 0;
				int _id, _audScore, _judgerScores[5];
				char _name[5] = {'\0'}, _songName[5] = {'\0'};
				drawBorder();
				setCursorVisible(1);
				printf("请输入第%d位歌手的编号，姓名（不超过4个字节），参赛曲目名称（不超过4个字节），大众评委投票数，五位专业评委", i + 1);
				setCursorPosition(1, 2);
				printf("打分：");
				scanf("%d %s %s %d", &_id, _name, _songName, &_audScore);
				cnt += _audScore;
				for (int j = 0; j < 5; j++)
				{
					scanf("%d", &_judgerScores[j]);
					cnt += _judgerScores[j];
				}
				cnt /= 6;
				if (_id <= 0)
				{
					sendWarn("编号必须为正整数！");
					_ok = 0;
				}
				else if (_audScore < 0 || _audScore > 100)
				{
					sendWarn("大众投票必须为0~100的整数！");
					_ok = 0;
				}
				else
				{
					for (int j = 0; j < 5; j++)
					{
						if (_judgerScores[j] < 0 || _judgerScores[j] > 100)
						{
							char tmpWarn[50];
							sprintf(tmpWarn, "第%d位评委的分数有误！分数必须在0~100之间，且为整数！", j + 1);
							sendWarn(tmpWarn);
							_ok = 0;
							break;
						}
					}
				}
				if (!_ok)
				{
					continue;
				}
				// 如果数据合法 将数据赋值给歌手 
				singers[i].id = _id;
				singers[i].song.audienceScore = _audScore;
				singers[i].song.totalScore = cnt;
				strcpy(singers[i].name, _name);
				strcpy(singers[i].song.name, _songName);
				for (int j = 0; j < 5; j++)
				{
					singers[i].song.judgerScores[j] = _judgerScores[j];
				}
			}
		}
		// 返回主菜单 
		prevState = MainMenu;
		displayMessage("输入完毕！"); 
	}
}
// 显示歌手信息 
void displaySingerInfo()
{
	drawBorder();
	// 必须有歌手在数据库里，才显示信息，否则不显示 
	if (singerAmount > 0)
	{
		// 绘制表头 
		drawGridHeader();
		// 绘制歌手信息 
		for (int i = 17 * curPage; i < singerAmount; i++)
		{
			drawGridSingerInfo(i, singers[i]);
		}
		char tmp[50];
		sprintf(tmp, "左右方向键换页，当前第%d/%d页，按其他键返回主界面", curPage + 1, maxPage);
		drawText(WIDTH / 2 - 24, HEIGHT - 1, tmp);
		// 获取用户按键，如果用户按在方向键，会重新获取一次按键keyCode 
		int key = getch();
		if (key == 224)
		{
			key = getch();
			if (key == 75)
			{
				if (curPage - 1 >= 0)
				{
					curPage--;
				}
			}
			else if (key == 77)
			{
				if (curPage + 1 < maxPage)
				{
					curPage++;
				}
			}
			displaySingerInfo();
		}
		else
		{
			displayMenu();
		}
	}
	else
	{
		prevState = MainMenu;
		displayMessage("暂无数据！");
	}
}
// 储存已有歌手的信息到singer.dat文件中 
void saveSingersInfo()
{
	FILE *filePtr;
	filePtr = fopen("singer.dat", "w");
	if (filePtr != NULL)
	{
		for (int i = 0; i < singerAmount; i++)
		{
			fprintf(filePtr, "%d %s %s %d ", singers[i].id, singers[i].name, singers[i].song.name, singers[i].song.audienceScore);
			for (int j = 0; j < 5; j++)
			{
				fprintf(filePtr, "%d ", singers[i].song.judgerScores[j]);
			}
			fprintf(filePtr, "%d\n", singers[i].song.totalScore);
		}
		fclose(filePtr);
	}
}
// 添加一个新的歌手信息 
void insertSingerInfo()
{
	int ok = 0;
	int cnt = 0;
	while (!ok)
	{
		int id, audScore, judgerScore[5];
		char name[5], songName[5];
		ok = 1;
		cnt = 0;
		drawBorder();
		setCursorVisible(1);
		printf("请输入被添加歌手的编号，姓名（不超过4个字节），参赛曲目名称（不超过4个字节），大众评委投票数，五位专业评委");
		setCursorPosition(1, 2);
		printf("打分：");
		scanf("%d", &id);
		if (checkSingerExists(id) > -1)
		{
			sendWarn("添加歌手的编号于已有歌手的编号重复！");
			ok = 0;
		}
		else
		{
			if (id < 0)
			{
				sendWarn("编号必须为正整数！");
				ok = 0;
			}
			else
			{
				scanf("%s %s %d", name, songName, &audScore);
				if (audScore < 0 || audScore > 100)
				{
					sendWarn("大众投票必须为0~100的整数！");
					ok = 0;
				}
				else
				{
					cnt += audScore;
					for (int i = 0; i < 5; i++)
					{
						scanf("%d", &judgerScore[i]);
						if (judgerScore[i] < 0 || judgerScore[i] > 100)
						{
							char tmpWarn[50];
							sprintf(tmpWarn, "第%d位评委的分数有误！分数必须在0~100之间，且为整数！", i + 1);
							sendWarn(tmpWarn);
							ok = 0;
							break;
						}
						cnt += judgerScore[i];
					}
					if (!ok)
					{
						continue;
					}
					cnt /= 6;
					singers[singerAmount].id = id;
					singers[singerAmount].song.audienceScore = audScore;
					strcpy(singers[singerAmount].name, name);
					strcpy(singers[singerAmount].song.name, songName);
					for (int i = 0; i < 5; i++)
					{
						singers[singerAmount].song.judgerScores[i] = judgerScore[i];
					}
					singers[singerAmount++].song.totalScore = cnt;
				}
			}
		}
	}
	prevState = MainMenu;
	displayMessage("添加完成！");
}
// 查询歌手信息 
void searchSingerInfo()
{
	drawBorder();
	setCursorVisible(1);
	printf("请输入要查询的歌手编号：");
	int id, index;
	scanf("%d", &id);
	index = checkSingerExists(id);
	// 如果查到了，就显示歌手的信息 查不到就返回主界面 
	if (index > -1)
	{
		drawBorder();
		drawGridHeader();
		drawGridSingerInfo(0, singers[index]); 
		drawText(WIDTH / 2 - 4, HEIGHT - 1, "按下任意键返回");
		getch();
		displayMenu();
	}
	else
	{
		prevState = MainMenu;
		displayMessage("暂无该编号的歌手！");
	}
}
// 删除歌手信息 
void deleteSingerInfo()
{
	drawBorder();
	setCursorVisible(1);
	printf("请输入要删除的歌手的编号：");
	int id, index;
	scanf("%d", &id);
	index = checkSingerExists(id);
	// 如果歌手存在 就删除 否则返回主界面 
	if (index > -1)
	{
		// 让该歌手索引后面的所有歌手的数据都向前移动一个存储单元 这样就达到了删除的作用 
		for (int i = index; i < singerAmount; i++)
		{
			singers[i] = singers[i + 1];
		}
		singerAmount--;
		prevState = MainMenu;
		displayMessage("删除成功！");
	}
	else
	{
		prevState = MainMenu;
		displayMessage("歌手不存在！");
	}
}
// 显示一条信息
// message: char* 用作显示的字符串 
void displayMessage(char* message)
{
	drawBorder();
	int len = strlen(message) / 2;
	drawWindow(WIDTH / 2 - 32, 1, 65, 7);
	setCursorPosition(WIDTH / 2 - len, 3);
	puts(message);
	setCursorPosition(WIDTH / 2 - 7, 6);
	puts("按任意键返回");
	getch();
	curState = prevState;
	prevState = DisplayMessage;
	toCurState();
}
// 将程序转到当前的状态所在的画面 
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
// 对当前已有的歌手信息进行排序，使用了冒泡排序算法 
void sortSingers()
{
	for (int i = 0; i < singerAmount - 1; i++)
	{
		for (int j = 0; j < singerAmount - 1 - i; j++)
		{
			if (singers[j].song.totalScore < singers[j + 1].song.totalScore)
			{
				Singer tmp = singers[j];
				singers[j] = singers[j + 1];
				singers[j + 1] = tmp;
			}
		}
	}
}
// 显示一条警告，但不会回到上一个画面
// warnMsg: char* 用于警告的字符串 
void sendWarn(char* warnMsg)
{
	drawBorder();
	int len = strlen(warnMsg) / 2;
	drawWindow(WIDTH / 2 - 32, 1, 65, 7);
	setCursorPosition(WIDTH / 2 - len, 3);
	puts(warnMsg);
	setCursorPosition(WIDTH / 2 - 7, 6);
	puts("按任意键重新输入数据");
	getch();
}
// 判断数据库中是否已有该编号的歌手信息，如果有则返回在数据库中的位置 
// id: int 要查找歌手的编号 
int checkSingerExists(int id)
{
	int exists = -1;
	for (int i = 0; i < singerAmount; i++)
	{
		if (singers[i].id == id)
		{
			exists = i;
			break;
		}
	}
	return exists;
}
// 取两个变量中较小的那个
// a: int 变量1
// b: int 变量2 
int getMin(int a, int b)
{
	if (a < b)
	{
		return a;
	}
	else
	{
		return b;
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
// 绘制程序的边框，同时清屏，重置光标的位置 
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
// 从左往右在(x, y)处输出一段文本，内容为text
// x: 控制台的第几列
// y: 控制台的第几行
// text: 所要输出的字符串 
void drawText(int x, int y, char* text)
{
	setCursorPosition(x, y);
	puts(text);
}
// 在(x, y)处输出一个字符
// x: 控制台的第几列
// y: 控制台的第几行
// c: 所要输出的字符 
void drawChar(int x, int y, char c)
{
	setCursorPosition(x, y);
	putchar(c);
}
// 在(x, y)处，横向或纵向，输出len个字符c
// x: 控制台的列
// y: 控制台的行
// direction: 输出的方向
// len: 输出的字符数量
// c: 要输出的字符 
void drawLine(int x, int y, Direction direction, int len, char c)
{
	setCursorPosition(x, y);
	for (int i = 0; i < len; i++)
	{
		switch (direction)
		{
			case HorizontalLeft:
				setCursorPosition(x - i, y);
				break;
			case HorizontalRight:
				setCursorPosition(x + i, y);
				break;
			case VerticalUp:
				setCursorPosition(x, y - i);
				break;
			case VerticalDown:
				setCursorPosition(x, y + i);
				break;
		}
		putchar(c);
	}
}
// 绘制表格的表头 
void drawGridHeader()
{
	// 绘制表格
	drawWindow(0, 0, 8, 3);
	drawText(2, 1, "编号"); 
	drawWindow(7, 0, 12, 3);
	drawText(9, 1, "歌手名称");
	drawWindow(18, 0, 12, 3);
	drawText(20, 1, "曲目名称");
	drawWindow(29, 0, 12, 3);
	drawText(31, 1, "大众评分");
	drawWindow(40, 0, 13, 3);
	drawText(42, 1, "评委1得分");
	drawWindow(52, 0, 13, 3);
	drawText(54, 1, "评委2得分");
	drawWindow(64, 0, 13, 3);
	drawText(66, 1, "评委3得分");
	drawWindow(76, 0, 13, 3);
	drawText(78, 1, "评委4得分");
	drawWindow(88, 0, 13, 3);
	drawText(90, 1, "评委5得分");
	drawWindow(100, 0, 8, 3);
	drawText(102, 1, "总分");
}
// 将歌手信息绘制到表格中
// row: int 将使用第几行来输出歌手信息，每行之间有一行分隔
// singer: Singer 被输出的歌手信息 
void drawGridSingerInfo(int row, Singer singer)
{
	int i = row;
	char tmp[20] = {0};
	Direction dir = HorizontalRight;
	sprintf(tmp, "%d", singer.id);
	setCursorPosition(1, 3 + 2 * i);
	drawText(2, 3 + 2 * i, tmp);
	drawChar(7, 3 + 2 * i, '*');
	strcpy(tmp, singer.name);
	drawText(9, 3 + 2 * i, tmp);
	drawChar(18, 3 + 2 * i, '*');
	strcpy(tmp, singer.song.name);
	drawText(20, 3 + 2 * i, tmp);
	drawChar(29, 3 + 2 * i, '*');
	sprintf(tmp, "%d", singer.song.audienceScore);
	drawText(31, 3 + 2 * i, tmp);
	drawChar(40, 3 + 2 * i, '*');
	for (int j = 0; j < 5; j++)
	{
		sprintf(tmp, "%d", singer.song.judgerScores[i]);
		drawText(42 + 12 * j, 3 + 2 * i, tmp);
		drawChar(52 + 12 * j, 3 + 2 * i, '*');
	}
	sprintf(tmp, "%d", singer.song.totalScore);
	drawText(102, 3 + 2 * i, tmp);
	drawLine(1, 3 + 2 * i + 1, dir, WIDTH - 2, '*');
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
