#include <stdio.h>

#define WIDTH 80
#define HEIGHT 20

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
