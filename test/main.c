#include<stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

#define UpKey 35
#define DownKey 43
#define LeftKey 38
#define RightKey 40
char c1 = 'a', c2 = 'a', ch = 'a';//��ʼ��Ϊ��a��
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
	printf("���˵�\n");
	printf("*****\n");
	printf("ѡ��1\n");
	printf("ѡ��2\n");
	printf("�˳�3\n");
	printf("*****\n");
}

int main() {
	Menu();//ԭ���Ѿ��е����˵�
	gotoxy(0, line);
	printf("\033[4;31;42mѡ��1\033[0m");
	while ((c1 = getch()) != '\r')
	{
		c2 = getch();
		if (c1 == -32)
		{
			ch = c2 - 37;
		}
		//�����ƶ�
		if (ch == UpKey)
		{
			line -= 1;//�����ƶ�							
			gotoxy(0, line);
			printf("\033[4;31;42mѡ��1\033[0m");
		}
		//�����ƶ�
		if (ch == DownKey)
		{
			line += 1;
			gotoxy(0, line);
			printf("\033[4;31;42mѡ��2\033[0m");
		}
	}

	return 0;
}