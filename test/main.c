#include<stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

#define UpKey 35
#define DownKey 43
#define LeftKey 38
#define RightKey 40
char c1 = 'a', c2 = 'a', ch = 'a';//初始化为‘a’
int line = 3;

void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}

void Menu()
{
	printf("*****\n");
	printf("主菜单\n");
	printf("*****\n");
	printf("选项1\n");
	printf("选项2\n");
	printf("退出3\n");
	printf("*****\n");
}

int main() {
	Menu();//原本已经有的主菜单
	gotoxy(0, line);
	printf("\033[4;31;42m选项1\033[0m");
	while ((c1 = getch()) != '\r')
	{
		c2 = getch();
		if (c1 == -32)
		{
			ch = c2 - 37;
		}
		//向上移动
		if (ch == UpKey)
		{
			line -= 1;//向上移动							
			gotoxy(0, line);
			printf("\033[4;31;42m选项1\033[0m");
		}
		//向下移动
		if (ch == DownKey)
		{
			line += 1;
			gotoxy(0, line);
			printf("\033[4;31;42m选项2\033[0m");
		}
	}

	return 0;
}