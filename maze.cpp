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
IMAGE levels(167,152);				//�ȼ�����
IMAGE shuoming(352,190);			//����˵��
IMAGE dividedline(521,11);			//�ָ���
IMAGE calculagraph(168,36);			//��ʱ��
IMAGE hiddenwall;					//����ש
IMAGE mainroad;						//��·
IMAGE outwall;						//��ǽ
IMAGE inwall;						//��ǽ
IMAGE player;						//���
IMAGE exit1;						//����
IMAGE sight(520,520);				//��Ϸ��Ұ�ڵ���Χ
POINT Player;						//���λ��
RECT Sight;							//��Ϸ��Ұ
int **map;							//�������ɵ�ͼ�Ķ���ָ��
int t = 0;							//ģʽѡ��
enum CMD { CMD_QUIT = 1, CMD_UP = 2, CMD_DOWN = 4, CMD_LEFT = 8, CMD_RIGHT = 16, CMD_REMAKE = 32};		//�����ƶ�


void Welcome();													//��ʼ����ʼ����
void ImageItem();												//��ʼ����ϷԪ��
void DFS(int x, int y, int m1, int n1);							//������������㷨
void Map_Make(int m, int n);									//��ʼ����ͨ�Ѷ���Ϸ��ͼ
void Map_Make2(int m, int n);									//��ʼ�������Ѷ���Ϸ��ͼ
int GetCmd();													//��ȡ�û����������		
void DispatchCmd(int cmd, int m, int n, time_t* t);				//�����û����������
void OnUp();													//�����ƶ�
void OnDown(int n);												//�����ƶ�
void OnLeft();													//�����ƶ�
void OnRight(int m);											//�����ƶ�
void Paint();													//������Ұ��Χ�ڵĵ�ͼ
int check(int m, int n, time_t* t, int m1);								//����Ƿ񵽴����
void Timing(time_t start);										//��ʱ������1
int Timing2(time_t* start, int m, int n);						//��ʱ������2
void InitGame(int m, int n);									//��ʼ����ͨ�Ѷ���Ϸ
void InitGame2(int m, int n);									//��ʼ�������Ѷ���Ϸ
void remake(int m, int n, time_t* t);							//������Ϸ
void level(time_t start, time_t end);							//��ͨ�Ѷȵȼ�����
int level2(time_t* start, time_t end, int m, int n);			//�����Ѷȵȼ�����

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

//���ƿ�ʼ����
void Welcome()
{
	
	MOUSEMSG m;	
	int x, y;

	//SetWorkingImage(&a);
	setbkcolor(WHITE);
	cleardevice();
	settextcolor(BLACK);
	settextstyle(70, 0, _T("����"));
	outtextxy(140, 60, _T("��  ��"));
	settextstyle(30, 0, _T("����"));
	outtextxy(40, 200, _T("��ѡ����Ϸ�Ѷȣ�"));
	settextstyle(25, 0, _T("����"));
	outtextxy(160, 300, _T("1.��ͨ�Ѷ�"));
	outtextxy(160, 375, _T("2.�����Ѷ�"));
	settextstyle(22, 0, _T("����"));
	outtextxy(28, 560, _T("ע�⣺    1.��ʹ�����ѡ���Ѷȣ�"));
	outtextxy(28, 590, _T("          2.����Ϸ�����ϼ�ʱ����"));
	outtextxy(28, 615, _T("            ��������ģʽҪ��׼����"));
	outtextxy(28, 500, _T("�Ѷ�˵����1.��ͨ�Ѷ����߳��Թ�����"));
	outtextxy(28, 525, _T("          2.�����Ѷ�������10�����߳��Թ���"));
	outtextxy(28, 650, _T("made by ���Ԫ"));
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

//��ʼ����Ϸ����Ԫ��ͼ��
void ImageItem()
{
	//������ǽ����ʽ
	loadimage(&outwall, "outwall.jpg", 40, 40);
	//������ǽ����ʽ
	loadimage(&inwall, "inwall.jpg", 40, 40);
	//��������ͼ��
	loadimage(&player, "player.jpg", 40, 40);
	//���ó���ͼ��
	loadimage(&exit1, "exit.jpg", 40, 40);
	//���õ�·ͼ��
	loadimage(&mainroad, "road1.jpg", 40, 40);
	//��������תͼ��
	loadimage(&hiddenwall, "hiddenwall.jpg", 40, 40);

	//���ÿ���˵��
	SetWorkingImage(&shuoming);
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(BLACK);
	line(0,36,0,190);
	settextcolor(BLACK);
	settextstyle(30, 0, _T("����"));
	outtextxy(10, 1, _T("����˵����"));
	settextstyle(20, 0, _T("����"));
	outtextxy(18, 46, _T("1.W/A/S/D�ͷ�����������������ƶ�"));
	outtextxy(18, 76, _T("2.��ESC���˳���Ϸ"));
	outtextxy(18, 106, _T("3.��R������Ϸ��ͼ"));
	SetWorkingImage();
	putimage(167, 530, &shuoming);

	//���÷ָ���
	SetWorkingImage(&dividedline);
	setfillstyle((BYTE*)"\x5a\x2d\x96\x4b\xa5\xd2\x69\xb4");
	setlinecolor(BLACK);
	rectangle(0, 0, 520, 10);
	solidrectangle(1, 1, 519, 9);
	SetWorkingImage();
	putimage(0, 520, &dividedline);
}

//��ʼ�����ɵ�ͼ(��ͨ�Ѷ�)
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

//��ʼ�����ɵ�ͼ(�����Ѷ�)
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

//������������㷨
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

//��ȡ�û���������
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

//�����û���������
void DispatchCmd(int cmd, int m, int n, time_t* t)
{
	if (cmd & CMD_UP) OnUp();
	if (cmd & CMD_DOWN) OnDown(n);
	if (cmd & CMD_LEFT)	OnLeft();
	if (cmd & CMD_RIGHT) OnRight(m);
	if (cmd & CMD_REMAKE) remake(m, n, t);
}

//�����ƶ�
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

//�����ƶ�
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

//�����ƶ�
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

//�����ƶ�
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

//������Ұ��Χ�ڵĵ�ͼ
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

//����Ƿ񵽴����
int check(int m, int n, time_t* t, int m1)
{
	int x, y;

	x = Player.x;
	y = Player.y;
	if (map[x][y] == 4 || map[x][y] == 5)
	{
		HWND hwnd = GetHWnd();
		if (MessageBox(hwnd, _T("ţ�ư����ⶼ���߳�����\n������һ����"), _T("��ϲ"), MB_YESNO | MB_ICONQUESTION) == IDYES)
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

//��ʱ��1
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
	settextstyle(25, 0, _T("����"));
	outtextxy(10, 5, _T("��ʱ��"));
	
	end = time(NULL);
	sprintf(str, "%d", end-start);
	outtextxy(105, 5, str);
	outtextxy(152, 5, _T("S"));

	SetWorkingImage();
	putimage(0, 531, &calculagraph);
	level(start, end);
	
	
}

//��ʱ��2
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
	settextstyle(25, 0, _T("����"));
	outtextxy(10, 5, _T("��ʱ��"));
	
	end = time(NULL);
	sprintf(str, "%d", end-*start);
	outtextxy(105, 5, str);
	outtextxy(152, 5, _T("S"));

	SetWorkingImage();
	putimage(0, 531, &calculagraph);
	flag = level2(start, end, m, n);
	
	return flag;
}
//��ʼ����Ϸ(��ͨ�Ѷ�)
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
//��ʼ����Ϸ(�����Ѷ�)
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

//������Ϸ��ͼ
void remake(int m, int n, time_t* t)
{
	InitGame(m, n);
	*t = time(NULL);
}

//�ȼ�����(��ͨ�Ѷ�)
void level(time_t start, time_t end)
{
	int a;

	a = end - start;
	SetWorkingImage(&levels);
	setbkcolor(WHITE);
	cleardevice();
	settextcolor(BLACK);
	settextstyle(23, 0, _T("����"));
	outtextxy(10, 10, _T("��ǰ�ȼ����ۣ�"));
	settextstyle(80, 0, _T("����"));
	
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

//�ȼ�����(�����Ѷ�)
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
	settextstyle(23, 0, _T("����"));
	outtextxy(10, 10, _T("��ǰ�ȼ����ۣ�"));
	settextstyle(80, 0, _T("����"));
	putimage(50, 54, &biaoqing);
	if (a > 10)
	{
		HWND hwnd = GetHWnd();
		if (MessageBox(hwnd, _T("���ź�δ�����Ŀ��\n���ٴγ�����"), _T("��ʾ"), MB_YESNO | MB_ICONQUESTION) == IDYES)
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
