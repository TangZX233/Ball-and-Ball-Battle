#include<stdio.h>
#include<easyx.h>

/*
���룺VS 2022

bվ��https://space.bilibili.com/606516570?spm_id_from=333.1007.0.0
*/

const int GridSize = 10;

//��ҡ�ʳ�AI

//����
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
}

int main()
{
	//ͼ�δ���
	initgraph(1024, 640);
	//��ɫ
	setbkcolor(WHITE);
	cleardevice();
	draw();

	getchar();
	return 0;
}