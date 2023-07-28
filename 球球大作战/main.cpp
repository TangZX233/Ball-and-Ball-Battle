#include<stdio.h>
#include<easyx.h>
#include<stdbool.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>

/*
球球大作战
开发环境：VS 2022 + eazyx
b站：https://space.bilibili.com/606516570?spm_id_from=333.1007.0.0
*/

//定义
const int GridSize = 10;      //格子宽度

//玩家、食物、AI
//图形、坐标（x，y）、颜色、半径、标记（isDie）
//封装->结构体
struct Sprite
{
	double x;       //坐标
	double y;
	double r;       //半径
	DWORD color;    //颜色
	bool isDie;     //死亡？
	char name[20];  //名字
	double speed;   //速度
	int food_index; //目标食物下标
};

//操作Sprite
void sprite_init(Sprite*spr,double x,double y, double r,const char* name ="Sprite",double speed = 1)
{
	spr->x = x;
	spr->y = y;
	spr->color = RGB(rand() %256, rand() % 256, rand() % 256);//[0,255]
	spr->r = r;
	spr->isDie = FALSE;
	strcpy(spr->name, name);
	spr->food_index = -1;
	spr->speed = speed;
}

//绘制
void sprite_draw(Sprite* spr, bool showName = TRUE)
{
	//死亡？
	if (!spr->isDie)
	{
		//颜色
		setfillcolor(spr->color);
		//画圆
		solidcircle(spr->x, spr->y, spr->r);
		//名字
		if (showName)
		{
			//文字颜色
			settextcolor(RGB(216, 39, 211));
			//背景透明
			setbkmode(TRANSPARENT);
			//文字样式
			settextstyle(28, 0, "黑体");

			outtextxy(spr->x - textwidth(spr->name) / 2, spr->y - textheight(spr->name) / 2, spr->name);
		}
	}
}

//移动函数
void sprite_moveBy(Sprite* spr,double dx,double dy)
{
	spr->x += dx;
	spr->y += dy;
}

//圆心距
double sprite_distance(Sprite* spr1, Sprite* spr2)
{
	return sqrt(pow(spr1->x - spr2->x, 2) + pow(spr1->y - spr2->y, 2));
}

//定义
#define FOOD_NUM 100          //食物数量
#define AI_NUM 10             //ai数量
Sprite gamer;                 //玩家
Sprite foods[FOOD_NUM];       //食物
Sprite ais[AI_NUM];           //ai
const char* names[AI_NUM] = { "Awake","RO","wink~","aaa","bbb","ccc","ddd","eee","fff","ggg" };

//初始化
void init()
{
	//随机数种子
	srand(time(NULL));
	//玩家
	sprite_init(&gamer, 100, 100, 20, "TangZX");
	//食物
	for (int i = 0; i < FOOD_NUM; i++)
	{
		sprite_init(foods + i, rand() % getwidth(), rand() % getheight(), 1 + rand() % 5,"");
	}
	//ai
	for (int i = 0; i < AI_NUM; i++)
	{
		sprite_init(ais + i, rand() % getwidth(), rand() % getheight(), 5 + rand() % 20, names[i]);
	}

}

//食物重置
void foodReset()
{
	for (int i = 0; i < FOOD_NUM; i++)
	{
		if (foods[i].isDie)
		{
			foods[i].isDie = 0;
			sprite_init(foods + i, rand() % getwidth(), rand() % getheight(), 1 + rand() % 5, "");
		}
	}
}

//绘制界面
void draw()
{
	//颜色
	setlinecolor(RGB(194, 195, 201));
	//格子
	for (int i = 0; i < getheight() / 10; i++)
	{
		line(0, i * GridSize, getwidth(), i * GridSize);
	}
	for (int i = 0; i < getwidth() / 10; i++)
	{
		line(i * GridSize, 0, i * GridSize, getwidth());
	}
	//画玩家
	sprite_draw(&gamer);
	//食物
	for (int i = 0; i < FOOD_NUM; i++)
	{
		sprite_draw(foods+i,FALSE);
	}
	//绘制ais
	for (int i = 0; i < AI_NUM; i++)
	{
		sprite_draw(ais + i);
	}

}

//计算球速
double ballSpeed(double ballradius,bool isgamer = 0)
{
	//玩家速度
	if (isgamer)
	{
		return 25 / ballradius;
	}
	//ai速度
	else
	{
		for (int i = 0; i < AI_NUM; i++)
		{
			if (ais[i].r = 20)
			{
				return 25 / ballradius;
			}
			else
			{
				return 1.5;
			}
		}
	}
}

//按键
void keyDownDeal()
{
	if (GetAsyncKeyState(VK_LEFT))
	{
		sprite_moveBy(&gamer, -ballSpeed(gamer.r,1), 0);
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		sprite_moveBy(&gamer, ballSpeed(gamer.r,1), 0);
	}
	if (GetAsyncKeyState(VK_UP))
	{
		sprite_moveBy(&gamer, 0, -ballSpeed(gamer.r,1));
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		sprite_moveBy(&gamer, 0, ballSpeed(gamer.r,1));
	}
}

//吃食物
void eatFood()
{
	//玩家吃食物
	for (int i = 0; i < FOOD_NUM; i++)
	{
		if (!foods[i].isDie && sprite_distance(&gamer, foods + i) < gamer.r)
		{
			foods[i].isDie = TRUE;  //食物死亡
			gamer.r += foods[i].r / 8;
		}
	}
	//ai吃食物
	for (int i = 0; i < FOOD_NUM; i++)
	{
		for (int k = 0; k < AI_NUM; k++)
		{
			if (!foods[i].isDie && sprite_distance(&ais[k], foods + i) < ais[k].r)
			{
				foods[i].isDie = TRUE;
				ais[k].r += foods[i].r / 8;
			}
		}
	}
}

//互吃
void gamerAndAiEat() 
{
	for (int i = 0; i < AI_NUM; i++)
	{
		//玩家吃ai
		if (!gamer.isDie && !ais[i].isDie && sprite_distance(&gamer, ais + i) < gamer.r - ais[i].r && gamer.r > ais[i].r)
		{
			ais[i].isDie = TRUE;
			gamer.r += ais[i].r / 8;
		}
		//ai吃玩家
		if (!gamer.isDie && !ais[i].isDie && sprite_distance(&gamer, ais + i) < ais[i].r - gamer.r && gamer.r < ais[i].r)
		{
			gamer.isDie = TRUE;
			ais[i].r += gamer.r / 8;
		}
		//ai吃ai
		for (int k = 0; k < AI_NUM; k++)
		{
			if (i != k && !ais[i].isDie && !ais[k].isDie && sprite_distance(ais + i, ais + k) < ais[i].r - ais[k].r && ais[i].r > ais[k].r)
			{
				ais[k].isDie = TRUE;
				ais[i].r += ais[k].r / 8;
			}
		}
	}
}

//追击
void chase(Sprite* run, Sprite* chase)
{
	if (chase->x < run->x)
	{
		chase->x += ballSpeed(chase->r);
	}
	else
	{
		chase->x -= ballSpeed(chase->r);
	}

	if (chase->y < run->y)
	{
		chase->y += ballSpeed(chase->r);
	}
	else
	{
		chase->y -= ballSpeed(chase->r);
	}
}

//ai移动
void aisMove()
{
	for (int i = 0; i < AI_NUM; i++)
	{
		int m = 0;
		for (int k = 0; k < AI_NUM; k++)
		{
			m++;
		}
		//追击玩家
		if (!ais[i].isDie && ais[i].r > gamer.r)
		{
			chase(&gamer, ais + i);
		}
		//追击ai
		/*else if (i != m && !ais[i].isDie && !ais[m].isDie && ais[i].r > ais[m].r)
		{
			chase(ais + m, ais + i);
		}*/
		//寻找食物
		else
		{
			if (!ais[i].isDie)
			{
				//判断存在
				if (ais[i].food_index != -1 && !foods[ais[i].food_index].isDie)
				{
					chase(foods + ais[i].food_index, ais + i);
				}
				//不存在，重找
				else
				{
					ais[i].food_index = rand() % FOOD_NUM;
				}
			}
		}
	}
}

int main()
{
	//图形窗口
	initgraph(1024, 640);
	//颜色
	setbkcolor(WHITE);
	cleardevice();
	init();
	draw();
	Sleep(500);

	//主循环
	while (true)
	{
		BeginBatchDraw();//双缓冲绘图

		cleardevice();
		foodReset();
		draw();
		keyDownDeal();
		eatFood();
		gamerAndAiEat();
		aisMove();

		EndBatchDraw();

		//游戏结束判断
		if (gamer.isDie || gamer.r > 150)
		{
			break;
		}
	}

	//刷新一次
	BeginBatchDraw();
	cleardevice();
	draw();
	keyDownDeal(); 
	eatFood();
	gamerAndAiEat();
	aisMove();
	EndBatchDraw();

	getchar();
	return 0;
} 