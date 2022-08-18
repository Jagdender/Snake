#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
//
//
//�궨��
#define COL 40   //����(x)
#define ROW 24   //����(y)
#define NONE 0   //��
#define WALL 1   //ǽ
#define FOOD 2   //ʳ��
#define HEAD 3   //��ͷ
#define BODY 4   //����
#define UP 72    //��
#define DOWN 80  //��
#define LEFT 75  //��
#define RIGHT 77 //��
#define SPACE 32 //��ͣ
#define ESC 27   //�˳�
//
//ȫ�ֱ���
int tick = 500;           //��Ϸ�̣�����Խ��Խ����
int level = 9;            //��Ϸ�Ѷ�
int length = 3;           //�����峤�ȣ�������ͷ��
int Direction = RIGHT;    //�߷���(Ĭ������)
int num = 0;              //ʳ�����
int coordinate[COL][ROW]; //����״̬
int r1 = 0, r2 = 0;       //�м�������Run()��Judge()��������

struct Snake //�ߣ�Ĭ��body[0]Ϊ��ͷ��
{
    int x;
    int y;
} body[COL * ROW]; //���鴴��ʱ��Ԫ��Ĭ��Ϊ0

void HideCursor();             //���ع��
void CursorJump(int x, int y); //�����ת
void difficulty();             //�Ѷ�ѡ��
void welcome();                //��ӭ����
void information();            //��Ϸ˵��    ���50%
void screen(int a);            // UI
void word(const char a[]);     //���嵯Ļ
void color(int c);             //��ɫ
void Init();                   //��ʼ��
void Run();                    //�ƶ���(����)
void Put(int x);               //ʳ�����ϰ���    ���50%
void Game();                   //��Ҷ�������
void Judge();                  //�ж�
void End();                    //������Ϸ
int main()
{
    srand(time(0));        //�����
    system("title Snake"); // cmd����
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
    if (a == 1) //��ʼ����32��16
    {
        system("cls");
        color(249);                          //����
        system("mode con cols=64 lines=16"); // cmd�ߴ�
        printf("����������������������������������������������������������������\n");
        for (int r = 14; r != 0; r--)
        {
            printf("��");
            for (int c = 60; c != 0; c--)
            {
                printf(" ");
            }
            printf("��\n");
        }
        printf("����������������������������������������������������������������");
    }
    else if (a == 2) //��ʼ��Ϸ����40��24
    {
        system("cls");
        system("mode con cols=80 lines=24"); // cmd�ߴ�
        color(248);                          //�ػ�
        printf("��������������������������������������������������������������������������������\n");
        for (int r = 22; r != 0; r--)
        {
            printf("��");
            for (int c = 76; c != 0; c--)
            {
                printf(" ");
            }
            printf("��\n");
        }
        printf("��������������������������������������������������������������������������������");
    }
    else if (a == 3) //��Ϸ����40��24(���귨)
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
                    printf("��");
                    break;
                case NONE:
                    printf("  ");
                    break;
                case FOOD:
                    color(244);
                    printf("��");
                    break;
                case HEAD:
                    color(242);
                    printf("��");
                    break;
                case BODY:
                    color(242);
                    printf("��");
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
    printf("̰������Ϸ");
    CursorJump(26, 8);
    color(243);
    word("���������ʼ");
    CursorJump(28, 10);
    color(247);
    word("��ESC�˳�");
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
    printf("̰������Ϸ");
    CursorJump(28, 5);
    color(246);
    word("ѡ���Ѷ�");
    CursorJump(16, 6);
    color(248);
    word("1.    BORING       �ٶȽ���");
    CursorJump(16, 7);
    color(242);
    word("2.    NORMAL       �ٶ��е�");
    CursorJump(16, 8);
    color(241);
    word("3.    DIFFICULT    �ٶȽϿ�");
    CursorJump(16, 9);
    color(244);
    word("4.    HELL         ����ս��");
    CursorJump(16, 10);
    color(246);
    word("5.    ZEN          ���׵���");
    CursorJump(28, 12);
    color(247);
    word("��ESC�˳�");
    for (;;)
    {
        m = 0;
        n = getch();
        switch (n) //����1 2 3 4�ֱ��Ӧ49 50 51 52
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
    //��ʼ���߿�
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
    //��ʼ����
    length = 3;
    num = 0;
    Direction = RIGHT;
    body[0].x = 10; //��λ������
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
    r1 = body[length].x; //����ԭ��β����
    r2 = body[length].y;
    coordinate[body[length].x][body[length].y] = NONE; //��ԭ��β������Ϊ�հ�
    for (int a = length; a != 0; a--)                  //λ������(������β��ʼ����)
    {
        body[a].x = body[a - 1].x;
        body[a].y = body[a - 1].y;
        coordinate[body[a].x][body[a].y] = BODY;
    }
    switch (Direction) //��ͷ��ʼ�ƶ�
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
    Judge();                                 //�ж�����ͷ��������
    coordinate[body[0].x][body[0].y] = HEAD; //������ͷ������Ϊ��ͷ
    screen(3);
    Sleep(tick);
}

void Put(int x)
{
    int n = 0;
    for (;;)
    {
        n = rand() % (COL * ROW); //���������
        if (coordinate[0][n] == 0)
            break;
    }
    if (x == FOOD) //����ʳ��
        coordinate[0][n] = FOOD;
}

void Game()
{
    int m = 0;         // mΪ�м���
    int n = Direction; // nΪ״̬��
    for (;;)
    {
        for (;;)
        {
            Run();
            if (kbhit())
                break;
        }
        for (; kbhit() != 0;) //��ջ�����ȡ���һλ��ֵ
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
    switch (coordinate[body[0].x][body[0].y]) //�ж��ƶ���������
    {
    case NONE:
        break;
    case WALL:
        End();
        break;
    case FOOD:
        length++;
        body[length].x = r1; //��λ
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
    printf("��");
    Sleep(1500);
    screen(1);
    system("COLOR 08");
    CursorJump(27, 6);
    color(7);
    word("GAME OVER");
    Sleep(500);
    CursorJump(23, 8);
    color(3);
    word("�ո��     ����һ��");
    CursorJump(24, 9);
    color(8);
    word("ESC��     �˳���Ϸ");
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

void word(const char a[]) //ע��const�������÷�
{
    for (int i = 0; a[i] != 0; i++)
    {
        printf("%c", a[i]);
        Sleep(5);
    }
    printf("\n");
}

//���´�������CSDN
void HideCursor()
{
    CONSOLE_CURSOR_INFO curInfo;                     //��������Ϣ�Ľṹ�����
    curInfo.dwSize = 1;                              //���û��ֵ�Ļ������������Ч
    curInfo.bVisible = FALSE;                        //���������Ϊ���ɼ�
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //��ȡ����̨���
    SetConsoleCursorInfo(handle, &curInfo);          //���ù����Ϣ
}

void CursorJump(int x, int y)
{
    COORD pos;                                       //������λ�õĽṹ�����
    pos.X = x;                                       //������
    pos.Y = y;                                       //������
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //��ȡ����̨���
    SetConsoleCursorPosition(handle, pos);           //���ù��λ��
}

void color(int c)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); //��ɫ����

    //ע��SetConsoleTextAttribute��һ��API��Ӧ�ó����̽ӿڣ�
}