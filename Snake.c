#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
//
//
//宏定义
#define COL 40   //列数(x)
#define ROW 24   //行数(y)
#define NONE 0   //空
#define WALL 1   //墙
#define FOOD 2   //食物
#define HEAD 3   //蛇头
#define BODY 4   //蛇身
#define UP 72    //上
#define DOWN 80  //下
#define LEFT 75  //左
#define RIGHT 77 //右
#define SPACE 32 //暂停
#define ESC 27   //退出
//
//全局变量
int tick = 500;           //游戏刻（数字越大越慢）
int level = 9;            //游戏难度
int length = 3;           //蛇身体长度（不包含头）
int Direction = RIGHT;    //蛇方向(默认向右)
int num = 0;              //食物个数
int coordinate[COL][ROW]; //坐标状态
int r1 = 0, r2 = 0;       //中间量，在Run()与Judge()储存坐标

struct Snake //蛇（默认body[0]为蛇头）
{
    int x;
    int y;
} body[COL * ROW]; //数组创建时各元素默认为0

void HideCursor();             //隐藏光标
void CursorJump(int x, int y); //光标跳转
void difficulty();             //难度选择
void welcome();                //欢迎界面
void information();            //游戏说明    完成50%
void screen(int a);            // UI
void word(const char a[]);     //字体弹幕
void color(int c);             //颜色
void Init();                   //初始化
void Run();                    //移动蛇(核心)
void Put(int x);               //食物与障碍物    完成50%
void Game();                   //玩家动作代码
void Judge();                  //判断
void End();                    //结束游戏
int main()
{
    srand(time(0));        //随机数
    system("title Snake"); // cmd名称
    HideCursor();
    welcome();
    Init();
    screen(2);
    Sleep(1000);
    screen(3);
    Sleep(1000);
    Game();
    return 0;
}

void screen(int a)
{
    if (a == 1) //初始界面32×16
    {
        system("cls");
        color(249);                          //中蓝
        system("mode con cols=64 lines=16"); // cmd尺寸
        printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
        for (int r = 14; r != 0; r--)
        {
            printf("■");
            for (int c = 60; c != 0; c--)
            {
                printf(" ");
            }
            printf("■\n");
        }
        printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
    }
    else if (a == 2) //初始游戏界面40×24
    {
        system("cls");
        system("mode con cols=80 lines=24"); // cmd尺寸
        color(248);                          //棕灰
        printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
        for (int r = 22; r != 0; r--)
        {
            printf("■");
            for (int c = 76; c != 0; c--)
            {
                printf(" ");
            }
            printf("■\n");
        }
        printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
    }
    else if (a == 3) //游戏界面40×24(坐标法)
    {
        for (int a = 0; a < COL; a++)
        {
            for (int b = 0; b < ROW; b++)
            {
                CursorJump(a * 2, b);
                switch (coordinate[a][b])
                {
                case WALL:
                    color(248);
                    printf("■");
                    break;
                case NONE:
                    printf("  ");
                    break;
                case FOOD:
                    color(244);
                    printf("●");
                    break;
                case HEAD:
                    color(242);
                    printf("■");
                    break;
                case BODY:
                    color(242);
                    printf("□");
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void welcome()
{
    int n = 0;
    screen(1);
    color(242);
    CursorJump(27, 6);
    printf("贪吃蛇游戏");
    CursorJump(26, 8);
    color(243);
    word("按任意键开始");
    CursorJump(28, 10);
    color(247);
    word("按ESC退出");
    n = getch();
    if (n != ESC)
        difficulty();
    else
        exit(0);
}

void difficulty()
{
    int n = 0, m = 0;
    system("cls");
    screen(1);
    CursorJump(27, 3);
    printf("贪吃蛇游戏");
    CursorJump(28, 5);
    color(246);
    word("选择难度");
    CursorJump(16, 6);
    color(248);
    word("1.    BORING       速度较慢");
    CursorJump(16, 7);
    color(242);
    word("2.    NORMAL       速度中等");
    CursorJump(16, 8);
    color(241);
    word("3.    DIFFICULT    速度较快");
    CursorJump(16, 9);
    color(244);
    word("4.    HELL         高挑战性");
    CursorJump(16, 10);
    color(246);
    word("5.    ZEN          由易到难");
    CursorJump(28, 12);
    color(247);
    word("按ESC退出");
    for (;;)
    {
        m = 0;
        n = getch();
        switch (n) //数字1 2 3 4分别对应49 50 51 52
        {
        case ESC:
            exit(0);
            break;
        case 49:
            level = 1;
            tick = 150;
            break;
        case 50:
            level = 2;
            tick = 60;
            break;
        case 51:
            level = 3;
            tick = 15;
            break;
        case 52:
            level = 4;
            tick = 3;
            break;
        case 53:
            level = 5;
            tick = 120;
            break;
        default:
            m = 1;
            break;
        }
        if (m == 0)
            break;
    }
}

void Init()
{
    //初始化边框
    for (int a = 0; a != COL; a++)
    {
        for (int b = 0; b != ROW; b++)
        {
            if (a == 0 || b == 0)
                coordinate[a][b] = WALL;
            else if (a == COL - 1 || b == ROW - 1)
                coordinate[a][b] = WALL;
            else
                coordinate[a][b] = NONE;
        }
    }
    //初始化蛇
    length = 3;
    num = 0;
    Direction = RIGHT;
    body[0].x = 10; //单位（方格）
    body[0].y = ROW / 2;
    coordinate[body[0].x][body[0].y] = HEAD;
    for (int a = 1; a <= length; a++)
    {
        body[a].x = body[a - 1].x - 1;
        body[a].y = ROW / 2;
        coordinate[body[a].x][body[a].y] = BODY;
    }
    Put(FOOD);
}

void Run()
{
    r1 = body[length].x; //储存原蛇尾坐标
    r2 = body[length].y;
    coordinate[body[length].x][body[length].y] = NONE; //将原蛇尾坐标设为空白
    for (int a = length; a != 0; a--)                  //位移身体(“从蛇尾开始动”)
    {
        body[a].x = body[a - 1].x;
        body[a].y = body[a - 1].y;
        coordinate[body[a].x][body[a].y] = BODY;
    }
    switch (Direction) //蛇头开始移动
    {
    case UP:
        body[0].y--;
        break;
    case DOWN:
        body[0].y++;
        break;
    case LEFT:
        body[0].x--;
        break;
    case RIGHT:
        body[0].x++;
        break;
    }
    Judge();                                 //判断新蛇头坐标内容
    coordinate[body[0].x][body[0].y] = HEAD; //将新蛇头坐标设为蛇头
    screen(3);
    Sleep(tick);
}

void Put(int x)
{
    int n = 0;
    for (;;)
    {
        n = rand() % (COL * ROW); //生成随机数
        if (coordinate[0][n] == 0)
            break;
    }
    if (x == FOOD) //生成食物
        coordinate[0][n] = FOOD;
}

void Game()
{
    int m = 0;         // m为中间量
    int n = Direction; // n为状态量
    for (;;)
    {
        for (;;)
        {
            Run();
            if (kbhit())
                break;
        }
        for (; kbhit() != 0;) //清空缓冲区取最后一位键值
            m = getch();
        switch (m)
        {
        case RIGHT:
            if (n == UP || n == DOWN)
            {
                n = m;
            }
            break;
        case LEFT:
            if (n == UP || n == DOWN)
            {
                n = m;
            }
            break;
        case UP:
            if (n == LEFT || n == RIGHT)
            {
                n = m;
            }
            break;
        case DOWN:
            if (n == LEFT || n == RIGHT)
            {
                n = m;
            }
            break;
        case ESC:
            exit(0);
        case SPACE:
            system("pause>nul");
            break;
        }
        Direction = n;
    }
}

void Judge()
{
    switch (coordinate[body[0].x][body[0].y]) //判断移动坐标类型
    {
    case NONE:
        break;
    case WALL:
        End();
        break;
    case FOOD:
        length++;
        body[length].x = r1; //定位
        body[length].y = r2;
        coordinate[body[length].x][body[length].y] = BODY;
        num++;
        Put(FOOD);
        if (level == 5)
            tick -= 5;
        break;
    case BODY:
        End();
        break;
    }
}

void End()
{
    int n = 0, m = 0;
    CursorJump(body[0].x * 2, body[0].y);
    color(252);
    printf("■");
    Sleep(1500);
    screen(1);
    system("COLOR 08");
    CursorJump(27, 6);
    color(7);
    word("GAME OVER");
    Sleep(500);
    CursorJump(23, 8);
    color(3);
    word("空格键     再来一局");
    CursorJump(24, 9);
    color(8);
    word("ESC键     退出游戏");
    for (;;)
    {
        m = 0;
        n = getch();
        switch (n)
        {
        case SPACE:
            Init();
            difficulty();
            screen(2);
            Sleep(1000);
            screen(3);
            Sleep(1000);
            Game();
            break;
        case ESC:
            exit(0);
            break;
        default:
            m = 1;
            break;
        }
        if (m == 0)
            break;
    }
}

void word(const char a[]) //注意const常量的用法
{
    for (int i = 0; a[i] != 0; i++)
    {
        printf("%c", a[i]);
        Sleep(5);
    }
    printf("\n");
}

//以下代码来自CSDN
void HideCursor()
{
    CONSOLE_CURSOR_INFO curInfo;                     //定义光标信息的结构体变量
    curInfo.dwSize = 1;                              //如果没赋值的话，光标隐藏无效
    curInfo.bVisible = FALSE;                        //将光标设置为不可见
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //获取控制台句柄
    SetConsoleCursorInfo(handle, &curInfo);          //设置光标信息
}

void CursorJump(int x, int y)
{
    COORD pos;                                       //定义光标位置的结构体变量
    pos.X = x;                                       //横坐标
    pos.Y = y;                                       //纵坐标
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //获取控制台句柄
    SetConsoleCursorPosition(handle, pos);           //设置光标位置
}

void color(int c)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); //颜色设置

    //注：SetConsoleTextAttribute是一个API（应用程序编程接口）
}