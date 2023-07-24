#include<stdio.h>
#include<easyx.h>

/*
编译：VS 2022

b站：https://space.bilibili.com/606516570?spm_id_from=333.1007.0.0
*/

const int GridSize = 10;

//玩家、食物、AI

//界面
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
}

int main()
{
	//图形窗口
	initgraph(1024, 640);
	//颜色
	setbkcolor(WHITE);
	cleardevice();
	draw();

	getchar();
	return 0;
}