#include <graphics.h>      
#include <conio.h>
#include<time.h>
#include<stdio.h>
#include <stdlib.h>
#define OUTWALL 2
#define INWALL 0
#define ROAD 1
#define START 3
#define HIDDEN 5

//IMAGE a(520,720);
IMAGE levels(167,152);				//等级评价
IMAGE shuoming(352,190);			//控制说明
IMAGE dividedline(521,11);			//分割线
IMAGE calculagraph(168,36);			//计时器
IMAGE hiddenwall;					//隐藏砖
IMAGE mainroad;						//道路
IMAGE outwall;						//外墙
IMAGE inwall;						//内墙
IMAGE player;						//玩家
IMAGE exit1;						//出口
IMAGE sight(520,520);				//游戏视野遮挡范围
POINT Player;						//玩家位置
RECT Sight;							//游戏视野
int **map;							//用于生成地图的二级指针
int t = 0;							//模式选择
enum CMD { CMD_QUIT = 1, CMD_UP = 2, CMD_DOWN = 4, CMD_LEFT = 8, CMD_RIGHT = 16, CMD_REMAKE = 32};		//控制移动


void Welcome();													//初始化开始界面
void ImageItem();												//初始化游戏元素
void DFS(int x, int y, int m1, int n1);							//深度优先搜索算法
void Map_Make(int m, int n);									//初始化普通难度游戏地图
void Map_Make2(int m, int n);									//初始化困难难度游戏地图
int GetCmd();													//获取用户输入的命令		
void DispatchCmd(int cmd, int m, int n, time_t* t);				//处理用户输入的命令
void OnUp();													//向上移动
void OnDown(int n);												//向下移动
void OnLeft();													//向左移动
void OnRight(int m);											//向右移动
void Paint();													//绘制视野范围内的地图
int check(int m, int n, time_t* t, int m1);								//检查是否到达出口
void Timing(time_t start);										//计时器函数1
int Timing2(time_t* start, int m, int n);						//计时器函数2
void InitGame(int m, int n);									//初始化普通难度游戏
void InitGame2(int m, int n);									//初始化困难难度游戏
void remake(int m, int n, time_t* t);							//重置游戏
void level(time_t start, time_t end);							//普通难度等级评价
int level2(time_t* start, time_t end, int m, int n);			//困难难度等级评价

int main()
{
	int m = 41, n = 41;
	int c;

	initgraph(520, 720);
	setbkcolor(WHITE);
	cleardevice();
	Welcome();
	ImageItem();
	
	if (t == 1)
	{
		InitGame(m, n);
		time_t end, start = time(NULL);
		while( !((c = GetCmd()) & CMD_QUIT) )
		{
			end = time(NULL);
			DispatchCmd(c, m, n, &start);
			Paint();
			Sleep(105);
			if (check(m, n, &start, t))
			{
				break;
			}
			else
			{
				Timing(start);
			}
		}
	}
	else if (t == 2)
	{
		InitGame2(m, n);
		time_t end, start = time(NULL);
		while( !((c = GetCmd()) & CMD_QUIT) )
		{
			end = time(NULL);
			DispatchCmd(c, m, n, &start);
			Paint();
			Sleep(105);
			if (check(m, n, &start, t))
			{
				break;
			}
			else
			{
				if (Timing2(&start, m, n))
					break;
			}	
		}
	}
	getch();
	closegraph();
	return 0;
}

//绘制开始界面
void Welcome()
{
	
	MOUSEMSG m;	
	int x, y;

	//SetWorkingImage(&a);
	setbkcolor(WHITE);
	cleardevice();
	settextcolor(BLACK);
	settextstyle(70, 0, _T("楷体"));
	outtextxy(140, 60, _T("迷  宫"));
	settextstyle(30, 0, _T("楷体"));
	outtextxy(40, 200, _T("请选择游戏难度："));
	settextstyle(25, 0, _T("楷体"));
	outtextxy(160, 300, _T("1.普通难度"));
	outtextxy(160, 375, _T("2.困难难度"));
	settextstyle(22, 0, _T("楷体"));
	outtextxy(28, 560, _T("注意：    1.请使用鼠标选择难度！"));
	outtextxy(28, 590, _T("          2.进游戏会马上计时所以"));
	outtextxy(28, 615, _T("            进入困难模式要有准备！"));
	outtextxy(28, 500, _T("难度说明：1.普通难度下走出迷宫即可"));
	outtextxy(28, 525, _T("          2.困难难度下需在10秒内走出迷宫！"));
	outtextxy(28, 650, _T("made by 孙浩元"));
	setlinecolor(BLACK);
	//rectangle(159, 294, 288, 329);
	//rectangle(159, 369, 288, 404);
	while(1)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			x = m.x;
		    y = m.y;
		}
		if (x >= 159 && x <= 288 && y >= 294 && y <= 329) 
		{
			t = 1;
			break;
		}
		if (x >= 159 && x <= 288 && y >= 369 && y <= 404) 
		{
			t = 2;
			break;
		}
	}
//	SetWorkingImage();

//	putimage(0, 0, &a);

}

//初始化游戏所需元素图案
void ImageItem()
{
	//设置外墙的样式
	loadimage(&outwall, "outwall.jpg", 40, 40);
	//设置内墙的样式
	loadimage(&inwall, "inwall.jpg", 40, 40);
	//设置人物图案
	loadimage(&player, "player.jpg", 40, 40);
	//设置出口图案
	loadimage(&exit1, "exit.jpg", 40, 40);
	//设置道路图案
	loadimage(&mainroad, "road1.jpg", 40, 40);
	//设置隐藏转图案
	loadimage(&hiddenwall, "hiddenwall.jpg", 40, 40);

	//设置控制说明
	SetWorkingImage(&shuoming);
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(BLACK);
	line(0,36,0,190);
	settextcolor(BLACK);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(10, 1, _T("操作说明："));
	settextstyle(20, 0, _T("黑体"));
	outtextxy(18, 46, _T("1.W/A/S/D和方向键控制上下左右移动"));
	outtextxy(18, 76, _T("2.按ESC键退出游戏"));
	outtextxy(18, 106, _T("3.按R重置游戏地图"));
	SetWorkingImage();
	putimage(167, 530, &shuoming);

	//设置分割线
	SetWorkingImage(&dividedline);
	setfillstyle((BYTE*)"\x5a\x2d\x96\x4b\xa5\xd2\x69\xb4");
	setlinecolor(BLACK);
	rectangle(0, 0, 520, 10);
	solidrectangle(1, 1, 519, 9);
	SetWorkingImage();
	putimage(0, 520, &dividedline);
}

//初始化生成地图(普通难度)
void Map_Make(int m, int n)
{
	int i, j, x, y;

	srand((unsigned)time(NULL));
	map = (int **) malloc (sizeof(int) * m);
	for (i = 0; i < m; i++)
	{
		*(map + i) = (int *) malloc (sizeof(int) * n);
	}
	for (i = 0 ;i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (i == 0 || i == m - 1 || j == 0 || j == n - 1 )
				map[i][j] = 2;
			else
				map[i][j] =0;
		}
	}
	map[0][1] = 2;
	map[m-2][n-1] = 4;
	x = rand() % (m-2) + 1;
	y = rand() % (n-2) + 1;
	if (x % 2 == 0)
	{
		x--;
	}
	if (y % 2 == 0)
	{
		y--;
	}
	DFS(x, y, m, n);
}

//初始化生成地图(困难难度)
void Map_Make2(int m, int n)
{
	int i, j, x, y;

	srand((unsigned)time(NULL));
	map = (int **) malloc (sizeof(int) * m);
	for (i = 0; i < m; i++)
	{
		*(map + i) = (int *) malloc (sizeof(int) * n);
	}
	for (i = 0 ;i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (i == 0 || i == m - 1 || j == 0 || j == n - 1 )
				map[i][j] = 2;
			else
				map[i][j] =0;
		}
	}
	map[0][1] = 5;
	map[m-2][n-1] = 4;
	x = rand() % (m-2) + 1;
	y = rand() % (n-2) + 1;
	if (x % 2 == 0)
	{
		x--;
	}
	if (y % 2 == 0)
	{
		y--;
	}
	DFS(x, y, m, n);
}

//深度优先搜索算法
void DFS(int x, int y, int m1, int n1)
{
	int dir[4][2] = {0, 1, 1, 0, 0, -1, -1, 0};
	int i, t, n;

	for (i = 0; i < 4; i++)
	{
		n = rand() % 4;
		t = dir[i][0], dir[i][0] = dir[n][0], dir[n][0] = t;
		t = dir[i][1], dir[i][1] = dir[n][1], dir[n][1] = t;
	}
	map[x][y] = 1;
	for (i = 0; i < 4; i++)
	{
		if ((x + dir[i][0] * 2) < 0 || (x + dir[i][0] * 2) > (m1 - 1))
		{
			continue;
		}
		if ((y + dir[i][1] * 2) < 0 || (y + dir[i][1] * 2) > (n1 - 1))
		{
			continue;
		}
		if (map[x+dir[i][0]*2][y+dir[i][1]*2] == 0)
		{
			map[x + dir[i][0]][y + dir[i][1]] = 1;
			DFS((x + (dir[i][0] * 2)), (y + (dir[i][1] * 2)), m1, n1);
		}	
	}
}

//获取用户输入命令
int GetCmd()
{

	int c = 0;

	if (GetAsyncKeyState(VK_LEFT)	& 0x8000)		c |= CMD_LEFT;
	if (GetAsyncKeyState(VK_RIGHT)	& 0x8000)		c |= CMD_RIGHT;
	if (GetAsyncKeyState(VK_UP)		& 0x8000)		c |= CMD_UP;
	if (GetAsyncKeyState(VK_DOWN)	& 0x8000)		c |= CMD_DOWN;
	if (GetAsyncKeyState('A')		& 0x8000)		c |= CMD_LEFT;
	if (GetAsyncKeyState('D')		& 0x8000)		c |= CMD_RIGHT;
	if (GetAsyncKeyState('W')		& 0x8000)		c |= CMD_UP;
	if (GetAsyncKeyState('S')		& 0x8000)		c |= CMD_DOWN;
	if (GetAsyncKeyState('R')		& 0x8000)		c |= CMD_REMAKE;
	if (GetAsyncKeyState(VK_ESCAPE)	& 0x8000)		c |= CMD_QUIT;

	return c;
}

//处理用户输入命令
void DispatchCmd(int cmd, int m, int n, time_t* t)
{
	if (cmd & CMD_UP) OnUp();
	if (cmd & CMD_DOWN) OnDown(n);
	if (cmd & CMD_LEFT)	OnLeft();
	if (cmd & CMD_RIGHT) OnRight(m);
	if (cmd & CMD_REMAKE) remake(m, n, t);
}

//向上移动
void OnUp()
{
	int x, y;

	x = Player.x;
	y = Player.y;
	if (y > 1 && map[x][y-1] != OUTWALL && map[x][y-1] != INWALL)
	{
		if (Player.y - Sight.top < 4 && Sight.top > 0)
		{
			Player.y--;
			Sight.top--;
			Sight.bottom--;
		}
		else
			Player.y--;
	}
}

//向下移动
void OnDown(int n)
{
	int x, y;

	x = Player.x;
	y = Player.y;
	if (map[x][y+1] != OUTWALL && map[x][y+1] != INWALL)
	{
		if (Sight.bottom - Player.y < 4 && Sight.bottom <= (n-2))
		{
			Player.y++;
			Sight.top++;
			Sight.bottom++;
		}
		else
			Player.y++;
	}
}

//向左移动
void OnLeft()
{
	int x, y;

	x = Player.x;
	y = Player.y;
	if (Player.x > 1 && map[x-1][y] != OUTWALL && map[x-1][y] != INWALL)
	{
		if (Player.x - Sight.left < 4 && Sight.left > 0)
		{
			Player.x--;
			Sight.left--;
			Sight.right--;
		}
		else
			Player.x--;
			
	}
	else if (map[x-1][y] == HIDDEN)
		Player.x--;
}

//向右移动
void OnRight(int m)
{
	int x, y;

	x = Player.x;
	y = Player.y;
	if (map[x+1][y] != OUTWALL && map[x+1][y] != INWALL)
	{
		if (Sight.right - Player.x < 4 && Sight.right <= (m-2))
		{
			Player.x++;
			Sight.left++;
			Sight.right++;
		}
		else 
			Player.x++;
	}
}

//绘制视野范围内的地图
void Paint()
{
	int x1, y1;

	SetWorkingImage(&sight);

	
	for(int x = Sight.left; x <= Sight.right; x++)
		for(int y = Sight.top; y <= Sight.bottom; y++)
		{
			if (map[x][y] == 2)
				putimage((x-Sight.left)*40, (y-Sight.top)*40, &outwall);
			else if (map[x][y] == 0)
				putimage((x-Sight.left)*40, (y-Sight.top)*40, &inwall);
			else if (map[x][y] == 4)
				putimage((x-Sight.left)*40, (y-Sight.top)*40, &exit1);
			else if (map[x][y] == 1)
				putimage((x-Sight.left)*40, (y-Sight.top)*40, &mainroad);
			else if (map[x][y] == 5)
				putimage((x-Sight.left)*40, (y-Sight.top)*40, &hiddenwall);
		}
	x1 = (Player.x - Sight.left) * 40;
	y1 = (Player.y - Sight.top) * 40;
	putimage(x1, y1, &player);
	SetWorkingImage();
	putimage(0, 0, &sight);
}

//检查是否到达出口
int check(int m, int n, time_t* t, int m1)
{
	int x, y;

	x = Player.x;
	y = Player.y;
	if (map[x][y] == 4 || map[x][y] == 5)
	{
		HWND hwnd = GetHWnd();
		if (MessageBox(hwnd, _T("牛逼啊！这都能走出来！\n想再来一把吗？"), _T("恭喜"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			if (m1 == 1) InitGame(m, n);
			else if (m1 == 2) InitGame2(m, n);
			
			*t = time(NULL);
			return 0;
		}
		else
			return 1;
	}
	return 0;
}

//计时器1
void Timing(time_t start)
{
	char str[3] = {0};
	time_t end;
	
	SetWorkingImage(&calculagraph);
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(BLACK);
	
	line(167,0,167,35);
	line(167,35,0,35);
	
	settextcolor(BLACK);
	settextstyle(25, 0, _T("黑体"));
	outtextxy(10, 5, _T("用时："));
	
	end = time(NULL);
	sprintf(str, "%d", end-start);
	outtextxy(105, 5, str);
	outtextxy(152, 5, _T("S"));

	SetWorkingImage();
	putimage(0, 531, &calculagraph);
	level(start, end);
	
	
}

//计时器2
int Timing2(time_t* start, int m, int n)
{
	char str[3] = {0};
	int flag;
	time_t end;
	
	SetWorkingImage(&calculagraph);
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(BLACK);
	
	line(167,0,167,35);
	line(167,35,0,35);
	
	settextcolor(BLACK);
	settextstyle(25, 0, _T("黑体"));
	outtextxy(10, 5, _T("用时："));
	
	end = time(NULL);
	sprintf(str, "%d", end-*start);
	outtextxy(105, 5, str);
	outtextxy(152, 5, _T("S"));

	SetWorkingImage();
	putimage(0, 531, &calculagraph);
	flag = level2(start, end, m, n);
	
	return flag;
}
//初始化游戏(普通难度)
void InitGame(int m, int n)
{

	Map_Make(m, n);
	Player.x = 1;
	Player.y = 1;
	Sight.left = 0;
	Sight.top = 0;
	Sight.right = 12;
	Sight.bottom = 12;
	Paint();
}
//初始化游戏(困难难度)
void InitGame2(int m, int n)
{
	Map_Make2(m, n);
	Player.x = 1;
	Player.y = 1;
	Sight.left = 0;
	Sight.top = 0;
	Sight.right = 12;
	Sight.bottom = 12;
	Paint();
}

//重置游戏地图
void remake(int m, int n, time_t* t)
{
	InitGame(m, n);
	*t = time(NULL);
}

//等级评分(普通难度)
void level(time_t start, time_t end)
{
	int a;

	a = end - start;
	SetWorkingImage(&levels);
	setbkcolor(WHITE);
	cleardevice();
	settextcolor(BLACK);
	settextstyle(23, 0, _T("黑体"));
	outtextxy(10, 10, _T("当前等级评价："));
	settextstyle(80, 0, _T("黑体"));
	
	if (a >= 0 && a <= 20)
	{
		outtextxy(56, 54, _T("A"));
	}
	else if (a > 20 && a <= 35)
	{
		outtextxy(56, 54, _T("B"));
	}
	else if (a > 35 && a <= 50)
	{
		outtextxy(56, 54, _T("C"));
	}
	else if (a > 50 && a <= 999)
	{
		outtextxy(56, 54, _T("D"));
	}
	SetWorkingImage();
	putimage(0, 567, &levels);
}

//等级评分(困难难度)
int level2(time_t* start, time_t end, int m, int n)
{
	int a;
	IMAGE biaoqing;
	
	loadimage(&biaoqing, "huaji.jpg", 80, 80);
	a = end - *start;
	SetWorkingImage(&levels);
	setbkcolor(WHITE);
	cleardevice();
	settextcolor(BLACK);
	settextstyle(23, 0, _T("黑体"));
	outtextxy(10, 10, _T("当前等级评价："));
	settextstyle(80, 0, _T("黑体"));
	putimage(50, 54, &biaoqing);
	if (a > 10)
	{
		HWND hwnd = GetHWnd();
		if (MessageBox(hwnd, _T("很遗憾未能完成目标\n想再次尝试吗？"), _T("提示"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			InitGame2(m, n);
			*start = time(NULL);
		}
		else
		{
			return 1;
		}
	}
	
	SetWorkingImage();
	putimage(0, 567, &levels);

	return 0;
}
