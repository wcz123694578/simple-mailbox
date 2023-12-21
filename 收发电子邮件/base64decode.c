#include "email.h"
char str8[9] = "00000000"; //两个辅助用字符串
char str6[7] = "000000";
char* to_bin8(int a)//转化为八位的二进制数
{
    memset(str8, 48, 9);
    str8[8] = 0;
    int i = 7;
    while (i >= 0)
    {
        str8[i] = a % 2 + 48;
        a = a / 2;
        i--;
    }
    return str8;
}
char* to_bin6(int a)//转化为六位的二进制数
{
    memset(str8, 48, 6);
    str6[6] = 0;
    int i = 5;
    while (i >= 0)
    {
        str6[i] = a % 2 + 48;
        a = a / 2;
        i--;
    }
    return str6;
}

void decode(char res[], char str[])//解码
{
    int length = strlen(str), i = 0;//获取需要解码的字符串长度
    char decode1[10000], decode2[9];
    memset(decode1, 0, length * 6 + 1);
    while (i < length)//将编码后的字符串的每一位还原为二进制数
    {
        int a;
        if (str[i] >= 'A' && str[i] <= 'Z')
            a = str[i] - 65;
        else if (str[i] >= 'a' && str[i] <= 'z')
            a = str[i] - 71;
        else if (str[i] >= '0' && str[i] <= '9')
            a = str[i] + 4;
        else if (str[i] == '+')
            a = 62;
        else if (str[i] == '/')
            a = 63;
        else break;
        strcat(decode1, to_bin6(a));
        i++;
    }
    i = 0;
    while (i < length * 6 && i + 2 < length * 6 && i + 4 < length * 6)//按每八位二进制数输出字符
    {
        decode2[0] = decode1[i];
        decode2[1] = decode1[i + 1];
        decode2[2] = decode1[i + 2];
        decode2[3] = decode1[i + 3];
        decode2[4] = decode1[i + 4];
        decode2[5] = decode1[i + 5];
        decode2[6] = decode1[i + 6];
        decode2[7] = decode1[i + 7];
        decode2[8] = 0;
        int a = atoi(decode2);//转化为二进制数
        a = a % 10 + a / 10 % 10 * 2 + a / 100 % 10 * 4 + a / 1000 % 10 * 8 + a / 10000 % 10 * 16 + a / 100000 % 10 * 32 + a / 1000000 % 10 * 64 + a / 10000000 % 10 * 128;//转化为十进制数
        strcat(res, a);
        i += 8;
    }
    strcat(res, '\0');
    printf("\n");
}