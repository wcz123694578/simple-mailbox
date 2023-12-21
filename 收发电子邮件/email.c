#include "email.h"
#include <wchar.h>


int line = 18;
char c1 = 'a', c2 = 'a', ch = 'a';
char user[255], password[255];
char from[255], to[255];
int lastline = 0;
char email_server[10000];
char email_pop_server[10000];

#define BASE_TAB \
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
  "abcdefghijklmnopqrstuvwxyz" "0123456789+/"

union base64_t {
	struct {
		unsigned c : 8;
		unsigned b : 8;
		unsigned a : 8;
	} src;
	struct {
		unsigned d : 6;
		unsigned c : 6;
		unsigned b : 6;
		unsigned a : 6;
	} res;
};




void _base64(char* res, char* src) {
	union base64_t data;
	data.src.a = (unsigned)src[0];
	data.src.b = (unsigned)src[1];
	data.src.c = (unsigned)src[2];
	res[0] = BASE_TAB[data.res.a];
	res[1] = BASE_TAB[data.res.b];
	res[2] = BASE_TAB[data.res.c];
	res[3] = BASE_TAB[data.res.d];
}

void base64(char* res, char* src) {
	int len = strlen(src);
	// src 中的每 3 个字符做一次操作
	while (*src != 0) {
		_base64(res, src);
		// 若 len 不是 3 的倍数, 则 res 末尾会有若干个 0
		src += 3, res += 4;
	}
	// 填充 '='
	if (len % 3 != 0) {
		// 先回退
		res -= (3 - len % 3);
		// 再填充
		memset(res, '=', 3 - len % 3);
	}
}

void gotoxy(int x, int y)//光标定位函数
{
	COORD p;//定义结构体变量p
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取当前函数句柄
	p.X = x; p.Y = y;//将光标的目标移动位置传递给结构体
	SetConsoleCursorPosition(handle, p);//移动光标
}
void COLOR_PRINT(const char* s, int color)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color);
	printf(s);
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
}
void PrintNChar(char ch, int n) {
	for (int i = 0; i < n; i++) {
		printf("%c", ch);
	}

}

void DisplayTitle() {
	system("cls");
	int choice;
	printf("qq请输1，163请输2 \033[0;31m(选择1/2)\033[0m >");
	scanf("%d", &choice);
	system("cls");
	int line = 0;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 80; j++) {
			char str[255];  str[0] = '#';    str[1] = '\0';
			COLOR_PRINT(str, 2);
		}
		printf("\n");
	}
	ShowBox();
	gotoxy(0, 20);
	//char email_server[100];
	switch (choice)
	{
	case 1:
		strcpy(email_server, "smtp.qq.com");
		strcpy(email_pop_server, "pop.qq.com");
		break;
	case 2:
		strcpy(email_server, "smtp.163.com");
		strcpy(email_pop_server, "pop.163.com");
		break;
	default:
		break;
	}
	system("cls");
	system("cls");
	system("mode con cols=101 lines=40");
	char buf[MAX_LINE];
	FILE* fp;
	int len;
	if ((fp = fopen("title.txt", "r")) == NULL) {
		perror("未能读取标题画面");
		exit(-1);
	}
	PrintNChar('*', 101);
	printf("\n");
	while (fgets(buf, MAX_LINE, fp) != NULL) {
		len = strlen(buf);
		buf[len - 1] = '\n\0';
		PrintNChar(' ', 9);
		//printf("%s", buf);
		COLOR_PRINT(buf, 2);
	}
	PrintNChar('*', 101);
	fclose(fp);
	printf("\n");
}
void DisplayMenu() {

	PrintNChar(' ', 40);
	printf("\033[0;31;42m     \033[0m");
	printf("\033[0;31;42m1.  发 送 邮 件\033[0m");
	printf("\033[0;31;42m     \033[0m\n");
	PrintNChar(' ', 40);
	printf("     ");
	printf("2.  接 收 邮 件");
	printf("     ");
}
void Select() {
	gotoxy(40, 18);
	int choice = 1;
	while ((c1 = getch()) != '\r')
	{
		c2 = getch();
		if (c1 == -32)
		{
			ch = c2 - 37;
		}
		//向上移动
		if (ch == UpKey && line != 18)
		{
			line -= 1;//向上移动		
			choice -= 1;
			gotoxy(40, line);
			printf("\033[0;31;42m     \033[0m");
			printf("\033[0;31;42m1.  发 送 邮 件\033[0m");
			printf("\033[0;31;42m     \033[0m");
			gotoxy(40, line + 1);
			printf("     ");
			printf("2.  接 收 邮 件");
			printf("     ");
			gotoxy(40, line);
		}
		//向下移动
		if (ch == DownKey && line != 19)
		{
			line += 1;
			choice += 1;
			gotoxy(40, line);
			printf("\033[0;31;42m     \033[0m");
			printf("\033[0;31;42m2.  接 收 邮 件\033[0m");
			printf("\033[0;31;42m     \033[0m");
			gotoxy(40, line - 1);
			printf("     ");
			printf("1.  发 送 邮 件");
			printf("     ");
			gotoxy(40, line);
		}
	}
	switch (choice)
	{
	case 1:
		SendEmail();
		break;
	case 2:
		ReceiveEmail();
		break;
	default:
		break;
	}
}
//创建套接字并连接smtp服务器
int CreateSocketAndConnect(char* smtp_server)
{
	struct sockaddr_in server_addr;
	struct hostent* Addr;
	int ret;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		printf("套接字创建失败！\n");
		return -1;
	}
	printf("套接字创建成功！\n");

	Addr = gethostbyname(smtp_server); //用域名获取对应IP 
	if (Addr == NULL) {
		printf("get ip error\n");
		return -1;
	}
	printf("成功获取域名的ip地址！\n");

	//连接smtp服务器
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(25);
	server_addr.sin_addr.S_un.S_addr = (*(struct in_addr*)Addr->h_addr_list[0]).S_un.S_addr;
	ret = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (ret < 0) {
		printf("connect server failed\n");
		return -1;
	}
	printf("成功连接smtp服务器！\n");
	return sock;
}
int CreateSocketAndConnectToPopServer(char* popserver) {
	struct sockaddr_in server_addr;
	struct hostent* Addr;
	int ret;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		printf("套接字创建失败！\n");
		return -1;
	}
	printf("套接字创建成功！\n");

	Addr = gethostbyname(popserver); //用域名获取对应IP 
	if (Addr == NULL) {
		printf("get ip error\n");
		return -1;
	}
	printf("成功获取域名的ip地址！\n");
	//连接pop3服务器
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(110);
	server_addr.sin_addr.S_un.S_addr = (*(struct in_addr*)Addr->h_addr_list[0]).S_un.S_addr;
	ret = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (ret < 0) {
		printf("connect server failed\n");
		return -1;
	}
	printf("成功连接pop3服务器！\n");
	return sock;
}

int server_send_recv_display(int sock, char* buff)
{
	int ret = 0;
	char recvbuff[320] = { 0 };

	send(sock, buff, strlen(buff), 0);
	ret = recv(sock, recvbuff, 320, 0);
	printf("服务器返回消息:\nlen = %d \n recv:[%s] \n", ret, recvbuff);

	return ret;
}

int email_send_helo_fun(int sock)
{
	char buff[320] = { 0 };
	int ret;

	sprintf(buff, "HELO qq.com\r\n");
	ret = server_send_recv_display(sock, buff);
	if (ret <= 0)
	{
		return -1;
	}

	return 0;
}

/*AUTH LOGIN 身份认证*/
int email_send_authlogin_fun(int sock)
{
	char buff[320] = { 0 };
	int ret;

	sprintf(buff, "%s", "AUTH LOGIN\r\n");
	ret = server_send_recv_display(sock, buff);
	if (ret <= 0)
	{
		return -1;
	}

	return 0;
}

int email_send_from_fun(int sock, char* from)
{
	puts(from);
	char buff[320] = { 0 };
	int ret;
	memset(buff, 0x00, sizeof(buff));
	//strcpy(buff, from);
	//puts(from);
	sprintf(buff, "MAIL FROM: <%s>\r\n", from);
	ret = server_send_recv_display(sock, buff);
	puts(buff);
	if (ret < 0) {
		return -1;
	}

	return 0;
}

//发送收件人
int email_send_to_fun(int sock, char* to)
{
	char buff[320] = { 0 };
	int ret;

	sprintf(buff, "RCPT TO: <%s>\r\n", to);
	ret = server_send_recv_display(sock, buff);
	if (ret < 0) {
		return -1;
	}

	return 0;
}

void SendEmail() {

	int sock;
	char buff[MAX_BUFF_SIZE];
	WSADATA wsa;
	int ret;

	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0) {
		printf("WSA startup failed\n");
		return 1;
	}

	sock = CreateSocketAndConnect(email_server);
	memset(buff, 0x00, sizeof(buff));
	while (1) {
		ret = recv(sock, buff, MAX_BUFF_SIZE, 0);
		if (ret < 0) {
			printf("recv error\n");
			continue;
		}

		printf("服务器返回消息：\n%s\n", buff);
		break;
	}
	while (1) {
		if (email_send_helo_fun(sock) < 0) {
			return 1;
		}

		if (email_send_authlogin_fun(sock) < 0) {
			return 1;
		}

		if (email_send_username_fun(sock, user) < 0) {
			return 1;
		}

		if (email_send_password_fun(sock, password) < 0) {
			return 1;
		}
		system("cls");
		printf("\033[0;31m登陆成功！\033[0m 欢迎%s\n", user);

		system("pause");
		system("cls");
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 80; j++) {
				char str[255];  str[0] = '#';    str[1] = '\0';
				COLOR_PRINT(str, 2);
			}
			printf("\n");
		}
		ShowMailInfoBox();
		//puts(from);
		lastline = 20;
		gotoxy(0, lastline);
		if (email_send_from_fun(sock, from) < 0) {
			return 1;
		}
		if (email_send_to_fun(sock, to) < 0) {
			return 1;
		}
		if (email_send_ready_fun(sock) < 0) {
			return 1;
		}
		createFile();
		printf("已为您创建好文本文件，请填写好内容后保存按回车及可发送。\n");
		system("pause");


		char buf[MAX_LINE];
		char mailtext[MAX_LINE];
		memset(mailtext, 0x00, sizeof(mailtext));
		FILE* fp;
		int len;
		if ((fp = fopen("mail.txt", "r")) == NULL) {
			perror("未能读取邮件");
			exit(-1);
		}
		while (fgets(buf, MAX_LINE, fp) != NULL) {
			len = strlen(buf);
			if (buf[len - 1] == '\r')   strcat(buf, "\n");
			else if (buf[len - 1] != '\r' && buf[len - 1] != '\n')
			{
				strcat(mailtext, "\r\n");
			}
			else if (buf[len - 1] == '\n') {
				buf[len - 1] = '\r', buf[len] = '\n', buf[len + 1] = '\0';
			}
			strcat(mailtext, buf);
			//printf("%s", buf);
		}
		mailtext[strlen(mailtext)] = '\0';
		puts(mailtext);
		/*for (int i = 0; i < strlen(mailtext); i++) {
			putchar(mailtext[i]);
		}*/
		fclose(fp);
		if (email_send_maintext_fun(sock, mailtext) < 0) {
			return 1;
		}
		gotoxy(5, 3);
		int pos = 3;
		for (int i = 0; i < 9; i++) {
			gotoxy(5, pos++);
			for (int j = 0; j < 54; j++) {
				printf("\033[0;37;47m \033[0m");
			}
		}
		gotoxy(15, 5); printf("\033[0;37;42m  回到标题页  \033[0m");
		gotoxy(32, 5); printf("\033[0;30;47m  退     出  \033[0m");
		int choice = 1;
		while ((ch = getch()) != '\r') {
			if (ch == -32) {
				c2 = getch();
				ch = c2 - 37;
			}
			switch (ch)
			{
			case LeftKey:
				if (choice != 1) {
					choice--;
					gotoxy(15, 5); printf("\033[0;37;42m  回到标题页  \033[0m");
					gotoxy(32, 5); printf("\033[0;30;47m  退     出  \033[0m");
				}
				break;
			case RightKey:
				if (choice == 1) {
					choice++;
					gotoxy(15, 5); printf("\033[0;30;47m  回到标题页  \033[0m");
					gotoxy(32, 5); printf("\033[0;37;42m  退     出  \033[0m");
				}
				break;
			}
		}
		if (choice == 1) {
			system("cls");
			continue;
		}
		else
		{
			system("cls");
			return;
		}
	}

}


void ReceiveEmail()
{
	int sock;
	char buff[MAX_BUFF_SIZE];
	WSADATA wsa;
	int ret;

	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0) {
		printf("WSA startup failed\n");
		return;
	}

	sock = CreateSocketAndConnectToPopServer(email_pop_server);
	memset(buff, 0x00, sizeof(buff));
	while (1) {
		ret = recv(sock, buff, MAX_BUFF_SIZE, 0);
		if (ret < 0) {
			printf("recv error\n");
			continue;
		}

		printf("服务器返回消息：\n%s\n", buff);
		break;
	}
	if (email_receive_username_fun(sock, user) < 0) {
		return;
	}
	if (email_receive_password_fun(sock, password) < 0) {
		return;
	}
	ShowMailBox();
	//if (email_receive_list_fun(sock) < 0) {
	//	return -1;
	//}
	//printf("\033[0;32;46m内容\033[0m\n");
	char buf[10000];
	if (email_receive_retr_fun(buf, sock) < 0) {
		return;
	}
	buf[700] = '\0';
	int cnt = 0;
	int pos = 3;
	gotoxy(3, 2);
	int line = 2;
	while (buf[cnt++] == ' ');
	for (int i = cnt; i < 200; i++) {
		printf("\033[0;30;47m%c\033[0m\n", buf[i]);
		gotoxy(pos++, line);
		if ((i + 1) % 76 == 0) {
			gotoxy(3, ++line);
			pos = 3;
		}
	}
	/*if (GetTop(sock, i, 5)) {
		return;
	}*/
}

int email_receive_list_fun(int sock) {
	char buff[320] = { 0 };
	char encbuff[120] = { 0 };
	int ret, len = 0;

	//sprintf(encbuff, username);    
	//Base64Encod(buff, encbuff, strlen(buff), &len);
	memset(buff, 0x00, sizeof(buff));
	//base64(buff, username);

	sprintf(buff, "list\r\n");
	ret = server_send_recv_display(sock, buff);
	if (ret <= 0)
	{
		return -1;
	}

	return 0;
}

int email_receive_retr_fun(char* str, int sock) {
	char sRetr[100] = { 0 };
	//system("chcp 65001");
	sprintf(sRetr, "RETR %d\r\n", 2);

	char response_buf[10000] = { 0 };
	send(sock, sRetr, strlen(sRetr), 0);
	Sleep(1000);     //必须添加延迟,以便能够完整的接收数据
	if (recv(sock, response_buf, 10000, 0) == SOCKET_ERROR)
		return -1;

	strcpy(str, response_buf);
	return 0;
}

int email_receive_username_fun(int sock, char* username) {
	char buff[320] = { 0 };
	char encbuff[120] = { 0 };
	int ret, len = 0;

	//sprintf(encbuff, username);    
	//Base64Encod(buff, encbuff, strlen(buff), &len);
	memset(buff, 0x00, sizeof(buff));
	//base64(buff, username);

	sprintf(buff, "user %s\r\n", username);
	ret = server_send_recv_display(sock, buff);
	if (ret <= 0)
	{
		return -1;
	}

	return 0;
}

int GetTop(int sock, int n, int nLine)
{
	char szTop[100] = { 0 };
	sprintf(szTop, "TOP %d %d\r\n", n, nLine);

	char response_buf[10000] = { 0 };
	send(sock, szTop, strlen(szTop), 0);
	Sleep(nLine * 20);     //   延迟,等待数据完全接收完毕
	if (recv(sock, response_buf, 10000, 0) == SOCKET_ERROR)
		return -1;

	printf("Receive Data:%s\n", response_buf);
	return 0;
}

int email_receive_password_fun(int sock, char* password) {
	char buff[320] = { 0 };
	char encbuff[120] = { 0 };
	int ret, len = 0;

	//sprintf(encbuff, password);          
	//Base64Encod(buff, encbuff, strlen(buff), &len);
	//puts("授权码");
	//puts(buff);
	memset(buff, 0x00, sizeof(buff));
	//base64(buff, password);
	sprintf(buff, "pass %s\r\n", password);
	puts(buff);
	ret = server_send_recv_display(sock, buff);
	if (ret < 0) {
		return -1;
	}

	return 0;
}

//发送账号
int email_send_username_fun(int sock, char* username)
{
	char buff[320] = { 0 };
	char encbuff[120] = { 0 };
	int ret, len = 0;

	//sprintf(encbuff, username);    
	//Base64Encod(buff, encbuff, strlen(buff), &len);
	memset(buff, 0x00, sizeof(buff));
	base64(buff, username);

	sprintf(buff, "%s\r\n", buff);
	ret = server_send_recv_display(sock, buff);
	if (ret <= 0)
	{
		return -1;
	}

	return 0;
}

//发送密码
int email_send_password_fun(int sock, char* password)
{
	char buff[320] = { 0 };
	char encbuff[120] = { 0 };
	int ret, len = 0;

	//sprintf(encbuff, password);          
	//Base64Encod(buff, encbuff, strlen(buff), &len);
	//puts("授权码");
	//puts(buff);
	memset(buff, 0x00, sizeof(buff));
	base64(buff, password);
	sprintf(buff, "%s\r\n", buff);
	puts(buff);
	ret = server_send_recv_display(sock, buff);
	if (ret < 0) {
		return -1;
	}

	return 0;
}

//通知服务器准备发送邮件
int email_send_ready_fun(int sock)
{
	char buff[320] = { 0 };
	int ret;

	sprintf(buff, "DATA\r\n");
	ret = server_send_recv_display(sock, buff);
	if (ret < 0) {
		return -1;
	}

	return 0;
}

//发送邮件
int email_send_maintext_fun(int sock, char* text)
{
	char buff[320] = { 0 };
	int ret;
	int cnt = 0;
	//char username[255], to[255], subject[255];

	//puts(text);

	sprintf(buff, "%s.\r\n", text);
	//puts(str);
	//sprintf(buff, str);
	ret = server_send_recv_display(sock, buff);
	if (ret < 0) {
		return -1;
	}

	return 0;
}





int enter(char* text, int left, int x, int y) {
	gotoxy(x, y);
	int xx = x, yy = y;
	//puts("pos");
	char ch;
	int cnt = strlen(text), pos = 0;
	pos = 0;
	while (ch = getch()) {
		text[strlen(text)] = '\0';
		char c2;
		if (ch == '\r')  break;
		else if (ch == -32) {
			c2 = getch();
			ch = c2 - 37;
		}
		else if (ch == 9)
		{
			return y;
		}
		switch (ch)
		{
		case '\b':
			if (pos > 0) {
				text[cnt--] = '\0';
				for (int i = pos - 1; i <= cnt; i++) {
					text[i] = text[i + 1];
				}
				gotoxy(--pos + left, y);
				for (int i = pos; i <= cnt; i++) {
					printf("\033[0;37;42m%c\033[0m", text[i]);
				}
				gotoxy(left + cnt, y);
				printf("\033[0;37;42m \033[0m");
				gotoxy(pos + left, y);
			}
			break;
		case LeftKey:
			if (pos >= 0) {
				pos--;
				gotoxy(left + pos, y);
			}
			break;
		case RightKey:
			if (pos < cnt) {
				pos++;
				gotoxy(left + pos, y);
			}
			break;
		default:
			cnt++;
			for (int i = cnt; i >= pos + 1; i--) {
				text[i] = text[i - 1];
			}
			text[pos++] = ch;

			printf("\033[0;37;42m%c\033[0m", ch);
			for (int i = cnt; i >= pos; i--) {

				printf("\033[0;37;42m%c\033[0m", text[i]);
			}
			gotoxy(left + pos, y);
			break;
		}
	}
	text[cnt] = '\0';
	//puts(user);
	//puts(password);
	return 0;
}

void ShowBox() {
	int line = 2;

	memset(user, 0x00, sizeof(user));
	gotoxy(9, line);
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 60; j++) {
			printf("\033[0;37;47m \033[0m");
		}
		gotoxy(9, ++line);
	}
	line = 5;
	gotoxy(36, line);
	printf("\033[0;36;47m登录邮箱\033[0m");
	line = 10;
	gotoxy(13, line);
	printf("\033[0;30;47m用户名: \033[0m");
	for (int i = 0; i < 40; i++) {
		printf("\033[0;37;42m \033[0m");
	}
	line = 13;
	gotoxy(13, line);
	printf("\033[0;30;47m  密码: \033[0m");
	for (int i = 0; i < 40; i++) {
		printf("\033[0;37;42m \033[0m");
	}
	memset(user, 0x00, sizeof(user));
	memset(password, 0x00, sizeof(password));
	int flag = enter(user, 21, 21, 10);
	while (flag != 0) {
		if (flag == 10) {
			flag = enter(password, 21, 21, 13);
		}
		else if (flag == 13) {
			flag = enter(user, 21, 21, 10);
		}
	}
	//puts(buff);
}

void ShowMailInfoBox() {
	int line = 2;
	gotoxy(9, line);
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 60; j++) {
			printf("\033[0;37;47m \033[0m");
		}
		gotoxy(9, ++line);
	}
	line = 5;
	gotoxy(36, line);
	printf("\033[0;36;47m邮件信息\033[0m");
	line = 10;
	gotoxy(13, line);
	printf("\033[0;30;47m发件人: \033[0m");
	for (int i = 0; i < 40; i++) {
		printf("\033[0;37;42m \033[0m");
	}
	line = 13;
	gotoxy(13, line);
	printf("\033[0;30;47m收件人: \033[0m");
	for (int i = 0; i < 40; i++) {
		printf("\033[0;37;42m \033[0m");
	}
	memset(user, 0x00, sizeof(from));
	memset(password, 0x00, sizeof(to));
	int flag = enter(from, 21, 21, 10);
	while (flag != 0) {
		if (flag == 10) {
			flag = enter(to, 21, 21, 13);
		}
		else if (flag == 13) {
			flag = enter(from, 21, 21, 10);
		}
	}
	//puts(from);
}

void ShowMailBox() {
	stack mailstack;
	mailstack = CreateStack(MAX_BOX_SIZE);
	system("pause");
	system("cls");
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 80; j++) {
			printf("\033[0;37;47m \033[0m");
		}
		printf("\n");
	}
	gotoxy(0, 0);
	printf("\033[0;30;44m  \033[0m");
	gotoxy(78, 0);
	printf("\033[0;30;44m  \033[0m");
	gotoxy(0, 24);
	printf("\033[0;30;44m  \033[0m");
	gotoxy(78, 24);
	printf("\033[0;30;44m  \033[0m");
	int pos = 2;
	for (int i = 0; i < 76; i++) {

		gotoxy(pos, 0);
		printf("\033[0;34;47m=\033[0m");
		gotoxy(pos++, 24);
		printf("\033[0;34;47m=\033[0m");
	}
	pos = 1;
	for (int i = 0; i < 23; i++) {
		gotoxy(0, pos);
		printf("\033[0;34;47m|\033[0m");
		gotoxy(1, pos);
		printf("\033[0;34;47m:\033[0m");
		gotoxy(78, pos);
		printf("\033[0;34;47m:\033[0m");
		gotoxy(79, pos++);
		printf("\033[0;34;47m|\033[0m");
	}
	gotoxy(0, 30);
}

stack CreateStack(int size) {
	stack s;
	s = (stack)malloc(sizeof(struct node));
	s->data = (struct mailinfo*)malloc(size * sizeof(struct mailinfo));
	s->top = -1;
	s->capacity = size;
	printf("成功初始化邮箱！");
	return s;
}

void push(stack s, struct mailinfo x) {
	s->data[++(s->top)] = x;
}

void createFile() {
	FILE* fpWrite = fopen("mail.txt", "w");
	if (fpWrite == NULL) {
		puts("创建文件失败！");
		return;
	}
	fprintf(fpWrite, "from: **your name** <%s>\r\n", from);
	fprintf(fpWrite, "to: **xxxxxx** <%s>\r\n", to);
	fprintf(fpWrite, "subject: **the title of your email**\r\n\r\n");
	fprintf(fpWrite, "**content**\r\n");
	fclose(fpWrite);
}
