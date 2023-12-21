#pragma once
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <wchar.h>

#pragma comment(lib, "ws2_32.lib")
#define MAX_BUFF_SIZE 512
#define MAX_LINE 1024
#define MAX_BOX_SIZE 1024

//·½Ïò¼ü
#define UpKey 35
#define DownKey 43
#define LeftKey 38
#define RightKey 40

#define BASE_TAB \
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
  "abcdefghijklmnopqrstuvwxyz" "0123456789+/"
#ifndef stack

typedef struct node* ptr;
struct node
{
    struct mailinfo* data;
    int top;
    int capacity;
};
typedef ptr stack;

struct mailinfo
{
    int index;
    char* subject;
};
#endif // !stack


void _base64(char* res, char* src);
void base64(char* res, char* src);
void gotoxy(int x, int y);
void DisplayTitle();
void DisplayMenu();
void Select();
void PrintNChar(char ch, int n);
void COLOR_PRINT(const char* s, int color);
int CreateSocketAndConnect(char* smtp_server);
int CreateSocketAndConnectToPopServer(char* popserver);
int server_send_recv_display(int sock, char* buff);
int email_send_helo_fun(int sock);
int email_send_authlogin_fun(int sock);
void SendEmail();
int email_send_username_fun(int sock, char* username);
int email_send_password_fun(int sock, char* password);
int email_send_from_fun(int sock, char* from);
int email_send_to_fun(int sock, char* to);
int email_send_ready_fun(int sock);
int email_send_maintext_fun(int sock, char* text);
int email_receive_username_fun(int sock, char* username);
int email_receive_password_fun(int sock, char* password);
int email_receive_list_fun(int sock);
int email_receive_retr_fun(char *buf, int sock);
int GetTop(int sock, int n, int nLine);
void ReceiveEmail();
void ShowBox();
stack CreateStack(int size);
void push(stack s, struct mailinfo x);
void createFile();
void ShowMailInfoBox();
void ShowMailBox();
int enter();