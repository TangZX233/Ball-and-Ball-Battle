#include<stdio.h>
#include<easyx.h>
#include<stdbool.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>

/*
�������ս
����������VS 2022 + eazyx
bվ��https://space.bilibili.com/606516570?spm_id_from=333.1007.0.0
*/

//����
const int GridSize = 10;      //���ӿ��

//��ҡ�ʳ�AI
//ͼ�Ρ����꣨x��y������ɫ���뾶����ǣ�isDie��
//��װ->�ṹ��
struct Sprite
{
	double x;       //����
	double y;
	double r;       //�뾶
	DWORD color;    //��ɫ
	bool isDie;     //������
	char name[20];  //����
	double speed;   //�ٶ�
	int food_index; //Ŀ��ʳ���±�
};

//����Sprite
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

//����
void sprite_draw(Sprite* spr, bool showName = TRUE)
{
	//������
	if (!spr->isDie)
	{
		//��ɫ
		setfillcolor(spr->color);
		//��Բ
		solidcircle(spr->x, spr->y, spr->r);
		//����
		if (showName)
		{
			//������ɫ
			settextcolor(RGB(216, 39, 211));
			//����͸��
			setbkmode(TRANSPARENT);
			//������ʽ
			settextstyle(28, 0, "����");

			outtextxy(spr->x - textwidth(spr->name) / 2, spr->y - textheight(spr->name) / 2, spr->name);
		}
	}
}

//�ƶ�����
void sprite_moveBy(Sprite* spr,double dx,double dy)
{
	spr->x += dx;
	spr->y += dy;
}

//Բ�ľ�
double sprite_distance(Sprite* spr1, Sprite* spr2)
{
	return sqrt(pow(spr1->x - spr2->x, 2) + pow(spr1->y - spr2->y, 2));
}

//����
#define FOOD_NUM 100          //ʳ������
#define AI_NUM 10             //ai����
Sprite gamer;                 //���
Sprite foods[FOOD_NUM];       //ʳ��
Sprite ais[AI_NUM];           //ai
const char* names[AI_NUM] = { "Awake","RO","wink~","aaa","bbb","ccc","ddd","eee","fff","ggg" };

//��ʼ��
void init()
{
	//���������
	srand(time(NULL));
	//���
	sprite_init(&gamer, 100, 100, 20, "TangZX");
	//ʳ��
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

//ʳ������
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

//���ƽ���
void draw()
{
	//��ɫ
	setlinecolor(RGB(194, 195, 201));
	//����
	for (int i = 0; i < getheight() / 10; i++)
	{
		line(0, i * GridSize, getwidth(), i * GridSize);
	}
	for (int i = 0; i < getwidth() / 10; i++)
	{
		line(i * GridSize, 0, i * GridSize, getwidth());
	}
	//�����
	sprite_draw(&gamer);
	//ʳ��
	for (int i = 0; i < FOOD_NUM; i++)
	{
		sprite_draw(foods+i,FALSE);
	}
	//����ais
	for (int i = 0; i < AI_NUM; i++)
	{
		sprite_draw(ais + i);
	}

}

//��������
double ballSpeed(double ballradius,bool isgamer = 0)
{
	//����ٶ�
	if (isgamer)
	{
		return 25 / ballradius;
	}
	//ai�ٶ�
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

//����
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

//��ʳ��
void eatFood()
{
	//��ҳ�ʳ��
	for (int i = 0; i < FOOD_NUM; i++)
	{
		if (!foods[i].isDie && sprite_distance(&gamer, foods + i) < gamer.r)
		{
			foods[i].isDie = TRUE;  //ʳ������
			gamer.r += foods[i].r / 8;
		}
	}
	//ai��ʳ��
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

//����
void gamerAndAiEat() 
{
	for (int i = 0; i < AI_NUM; i++)
	{
		//��ҳ�ai
		if (!gamer.isDie && !ais[i].isDie && sprite_distance(&gamer, ais + i) < gamer.r - ais[i].r && gamer.r > ais[i].r)
		{
			ais[i].isDie = TRUE;
			gamer.r += ais[i].r / 8;
		}
		//ai�����
		if (!gamer.isDie && !ais[i].isDie && sprite_distance(&gamer, ais + i) < ais[i].r - gamer.r && gamer.r < ais[i].r)
		{
			gamer.isDie = TRUE;
			ais[i].r += gamer.r / 8;
		}
		//ai��ai
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

//׷��
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

//ai�ƶ�
void aisMove()
{
	for (int i = 0; i < AI_NUM; i++)
	{
		int m = 0;
		for (int k = 0; k < AI_NUM; k++)
		{
			m++;
		}
		//׷�����
		if (!ais[i].isDie && ais[i].r > gamer.r)
		{
			chase(&gamer, ais + i);
		}
		//׷��ai
		/*else if (i != m && !ais[i].isDie && !ais[m].isDie && ais[i].r > ais[m].r)
		{
			chase(ais + m, ais + i);
		}*/
		//Ѱ��ʳ��
		else
		{
			if (!ais[i].isDie)
			{
				//�жϴ���
				if (ais[i].food_index != -1 && !foods[ais[i].food_index].isDie)
				{
					chase(foods + ais[i].food_index, ais + i);
				}
				//�����ڣ�����
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
	//ͼ�δ���
	initgraph(1024, 640);
	//��ɫ
	setbkcolor(WHITE);
	cleardevice();
	init();
	draw();
	Sleep(500);

	//��ѭ��
	while (true)
	{
		BeginBatchDraw();//˫�����ͼ

		cleardevice();
		foodReset();
		draw();
		keyDownDeal();
		eatFood();
		gamerAndAiEat();
		aisMove();

		EndBatchDraw();

		//��Ϸ�����ж�
		if (gamer.isDie || gamer.r > 150)
		{
			break;
		}
	}

	//ˢ��һ��
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