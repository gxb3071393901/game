#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<time.h>
#include<conio.h>

void welcome();			//开始界面
void finish();			//结束界面
void game();			//游戏界面

void creat_food();      //生成食物

void snake_move();      //蛇的移动
void eating();          //蛇的变长
void change(int x,int y);          //蛇的坐标变换


int click_ontrol();     // 获取键盘信号
int judge();           //游戏结束判断
/*这三个不怎么懂*/
void gotoxy(int x, int y);    // 光标跳转，横为X 0,1,2..
void gotoprint(int x, int y); // 跳转打印
void gotodelete(int x, int y);// 跳转删除

typedef struct snake       //声明蛇，以链表形式建立
{
    int x;
    int y;
    struct snake* next;
}snake;

snake* head;           //声明蛇头指针

struct food           //声明食物 
{
    int x;
    int y;
}food;

char name[20];       //名字
int score = 0;       //成绩
int click = 1;       //记录敲击键盘按键
int speed;           //速度，即延迟的毫秒数

int main()
{
    system("color 0B"); // 设置控制台字体颜色 
	welcome();          // 欢迎界面
    game();             //创造游戏界面
    creat_food();       //创造食物
    // 捕获鼠标按键 ClickControl
    if (click_ontrol() == 0) 
        return 0;
    return 0;
    
}

void gotoxy(int x, int y)
{
    // 更新光标位置 
    COORD pos;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hOutput, pos);
    // 隐藏光标 
    CONSOLE_CURSOR_INFO cursor;
    cursor.bVisible = FALSE;
    cursor.dwSize = sizeof(cursor);
    SetConsoleCursorInfo(hOutput, &cursor);
}

/**********************************************************/
void gotoprint(int x, int y)
{
    gotoxy(x, y);
    printf("■");
}

/**********************************************************/
void gotodelete(int x, int y)
{
    gotoxy(x, y);
    printf("  ");
}

/**********************************************************/

void welcome()
{
    gotoxy(20,5);
    printf("---------------------------------------------------");
    gotoxy(20, 23);
    printf("---------------------------------------------------");
    gotoxy(25, 8);
    printf("hello world");
    gotoxy(29, 12);
    printf("注意：请在英文输入法中操作");
    gotoxy(29, 14);
    printf("\t   反向移动等同于吃到自己");
    gotoxy(29, 16);
    printf("\t   wasd控制方向 p暂停");
    gotoxy(25, 20);
    printf("请输入你的名字：");
    scanf_s("%s", &name, 20);
    system("cls");
}

void game()
{
    int i;
    /*
    注意这里横坐标是每次+2 因为控制台字符宽高比为1：2
    */
    //emmm,其实我也不知道，看大佬的代码知道的
    for (i = 0;i < 58;i += 2)
    {
        gotoprint(i,0);         //高为26
        gotoprint(i,26);
    }
    for (i = 1;i < 26;i++)
    {
        gotoprint(0,i);
        gotoprint(56, i);      //宽为56
    }
    gotoxy(63, 10);
    printf("你好 %s，欢迎来到贪吃蛇！",name);
    gotoxy(63, 15);
    printf("你目前的成绩为: %d  ",score);
    gotoxy(63, 16);
    printf("加油！");
    gotoxy(63, 20);
    printf("游戏不错吧，hahaha！");

    head = (snake*)malloc(sizeof(snake));
    snake* p = (snake*)malloc(sizeof(snake));
    snake* q = (snake*)malloc(sizeof(snake));

    head->x = 16;                  //创造一条蛇，三的长度，用链表表示
    head->y = 15;
    p->x = 16;
    p->y = 16;
    q->x = 16;
    q->y = 17;
    head->next = p;
    p->next = q;
    q->next = NULL;
}
//创造食物
void creat_food()
{
    // 随机产生一个食物 
    int flag = 0;
    while (!flag)
    {
        flag = 1;
        srand((int)time(NULL));    //这句话真心不懂
        food.x = rand() % (54 - 2 + 1) + 2;
        food.y = rand() % (25 - 1 + 1) + 1;
        if (food.x % 2 != 0)         //因为长一直是2的增长，所以排除奇数
        {
            food.x = food.x + 1;
        }
        //判断是否与蛇身体重复
        snake* p1 = head;
        while (1)
        {
            if (p1->next == NULL)
                break;
            if (food.x == p1->x && food.y == p1->y)
                flag = 0;
            p1 = p1->next;
        }
    }
    gotoxy(food.x, food.y);
    printf("⊙");
}
//获取键盘
int click_ontrol()
{
    while (1)
    {

        char c;
        if (judge()== 0)
            return 0;
        if (_kbhit())
        {
            click = _getch();
        }
        snake_move();
        eating();
    }
    return 1;
}
//蛇的移动
void snake_move()
{
    int x, y,S;
    x = head->x;
    y = head->y;
    snake* p = head;
    // 通过先清空后打印实现动画效果,即先清除尾节点，后打印头节点；
    while (p->next != NULL)
    {
        p = p->next;
    }
    gotodelete(p->x, p->y);//清除尾节点
    switch (click)
    {
    case 'w':y = y - 1;break;            //上下左右
    case 's':y = y + 1;break;
    case 'a':x = x - 2;break;
    case 'd':x = x + 2;break;
    default:break;
    }
    if (x != head->x || y != head->y)
        change(x, y);
    p = head;
    //打印蛇头
    gotoprint(p->x, p->y);
    //控制蛇的速度
    S = score / 10;
    if (S <= 10)
        speed = 250;
    else if (S > 10 && S <= 20)
        speed = 150;
    else if (S > 20 && S <= 40)
        speed = 100;
    else
        speed = 50;
    Sleep(speed);           //睡眠时间越短，速度越快
}

void eating()         //蛇的变长
{
    if (head->x == food.x && head->y == food.y)
    {
        creat_food();
        snake *_new = (snake*)malloc(sizeof(snake));
        snake* p;
        p = head;
        while (1)
        {
            if (p->next == NULL)
                break;
            p = p->next;
        }
        p->next = _new;
        _new->next = NULL;
        score += 10;
        gotoxy(77, 15);
        printf("%d", score);
    }
}

// 更新蛇体坐标 只需要消除尾结点 然后把新坐标结点置为头结点即可 
void change(int x, int y)
{
    snake* p = head;
    while (p->next->next != NULL)
    {
        p = p->next;
    }
    free(p->next);
    p->next = NULL;
    snake* new_head = (snake*)malloc(sizeof(snake));
    new_head->x = x;
    new_head->y = y;
    new_head->next = head;
    head = new_head;
}

//判断游戏是否结束
int judge()
{
    if (head->x == 0 || head->x == 56 || head->y == 0 || head->y == 26)//撞墙
    {
        finish();
        return 0;
    }
    snake* p = head->next;
    while(1)
    {
        if (p == NULL)
            break;
        if (head->x == p->x && head->y == p->y)//撞到自己身上
        {
            finish();
            return 0;
        }
        p = p->next;
    }
    return 1;
}
void finish()
{
    system("cls");
    gotoxy(15, 10);
    printf("/**********************************************/");
    gotoxy(15, 20);
    printf("/**********************************************/");
    gotoxy(18, 14);
    printf("游戏结束！");
    gotoxy(20, 16);
    printf("你的成绩为 %d",score);
    gotoxy(18, 18);
    printf("还不错哦!     继续努力O(∩_∩)O");
    gotoxy(0, 27);
    //释放空间
    snake* p = head, * q;
    while (p != NULL) {
        q = p->next;
        free(p);
        p = q;
    }
    system("pause");


}
