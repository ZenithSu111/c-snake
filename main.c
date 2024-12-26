#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <stdbool.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

#define SNAKE 1
#define FOOD 2
#define WALL 3

char map[17][17] = {0};
unsigned char snake[50] = {77}; // 設置起始座標 77 = 0100 1101 應射到4,0的位置上
                                //  4   0
unsigned char food = 68;
char len = 1;

void print_game()
{
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            if (map[i][j] == 0)
            {
                putchar(' ');
            }
            else if (map[i][j] == SNAKE)
            {
                putchar('o');
            }
            else if (map[i][j] == FOOD)
            {
                putchar('@');
            }
            else if (map[i][j] == WALL)
            {
                putchar('#');
            }
        }
        putchar('\n');
    }
    Sleep(500);
    system("cls");
}

int get_dir(int old_dir) // 獲取操作者的輸入
{
    int new_dir = old_dir;
    if (_kbhit())
    {
        getch(); // 第一個輸入單純是讓程式進入操作模式 所以第一個輸入放掉
        new_dir = getch();
        // printf("%d\n", new_dir);
    }
    if (len > 1)
    {
        if (abs(new_dir - old_dir) == 8 || abs(new_dir - old_dir) == 2) // UP - DOWN or DOWN -UP =8 當前狀態為UP或DOWN時，按下反方向就會觸發這段程式 而因為兩個相減為8 從而鎖定了當前狀態
            return old_dir;
    }
    return new_dir;
}

unsigned char generate_food()
{

    unsigned char food_, fx, fy;
    bool is_snake = false;

    srand((unsigned int)time(NULL));
    do
    {
        is_snake = false;
        food_ = (unsigned char)(rand() % 256); //0~255
        fx=food_ >> 4;
        fy=food_ & 0x0f;
        for(int i = 0; i < len; i++){
            if( food_ == snake[i]){
                is_snake = true;
            }
        }

    } 
    while (fx == 0 || fy == 0  || is_snake);

    return food_;
}

void move_snake(int dir)
{

    int last = snake[0], current; // 用途儲存蛇的頭跟尾巴的移動位置
    bool grow = false;            // 有沒有尾巴

    unsigned char fx, fy, x, y;
    fx = food >> 4;
    fy = food & 0x0F;
    x = snake[0] >> 4;
    y = snake[0] & 0x0F;
    switch (dir)
    {
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    }
    snake[0] = (x << 4) | y;
    if (snake[0] == food)
    { // 當蛇的頭跟食物座標重疊 代表吃到了
        grow = true;
        food = generate_food();
    }

    for (int i = 0; i < len; i++)
    {
        if (i == 0)
            continue;
        current = snake[i];
        snake[i] = last;
        last = current;
    }
    if (grow == true)
    {
        snake[len] = last;
        len++;
    }
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            if (i == 0 || i == 16 || j == 0 || j == 16)
            {
                map[i][j] = WALL;
            }
            else if (i == fy && j == fx)
            {
                map[i][j] = FOOD;
            }
            else
            {
                map[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < len; i++)
    {
        x = snake[i] >> 4;
        y = snake[i] & 0x0F;
        if (snake[i] > 0)
        {
            map[y][x] = SNAKE;
        }
    }
}

bool is_alive(){

    bool self_eat = false;
    unsigned char x,y;

    x = snake[0] >> 4;
    y = snake[0] & 0x0f;
    for(int i = 1; i < len; i++){
        if(snake[0] == snake[i]){
            self_eat = true;
        }
    }

    return ( x == 0 || x==16 || y == 0 || y == 16 || self_eat) ? false : true;
}

int main()
{
    // 確保map所有元素為0
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            map[i][j] = 0;
        }
    }
    // 確保snake除了起始座標其餘為0
    for (int i = 1; i < 50; i++)
    {
        snake[i] = 0;
    }

    int dir = UP;
    while (1)
    {
        print_game();
        dir = get_dir(dir);
        move_snake(dir);

        if(!is_alive()){
        break;
        }
    }

    printf("GAME OVER\n");
    getchar();

    return 0;
}
