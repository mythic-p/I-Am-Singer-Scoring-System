#include <stdio.h>

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

void initSystem();

int main()
{
	initSystem();
	return 0;
}


void initSystem()
{
	char tmp[50];
	sprintf(tmp, "mode con cols=%d lines=%d", WIDTH, HEIGHT);
	system(tmp);
}
